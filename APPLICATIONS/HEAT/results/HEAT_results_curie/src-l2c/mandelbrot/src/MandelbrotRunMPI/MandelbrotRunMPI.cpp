#include <complex>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include <llcmcpp/lifecycle.h>
#include <llcmcpp/go.h>
#include <llcmcpp/llcmcpp.h>
#include <mpi.h>

#include "Mandelbrot.h"
#include "Array2DDump.h"
#include "../mytime.h"

#define DEBUG

using namespace llcmcpp;
using namespace std;

namespace mandelbrot
{

namespace impl
{

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

class llcm_MandelbrotRunMPI : virtual public llcmcpp::Component, virtual public Go, virtual public LifeCycle,
										virtual public mandelbrot::MandelbrotManagement
{
private:
  std::complex<double> _R0; // lower left point of the rectangle
  std::complex<double> _R1; // upper right point of the rectangle
  unsigned int _Nx;
  unsigned int _Ny;
  unsigned int _NbIter;

  bool _dump;
  std::string _dumpfile;

  unsigned int* _data;

  unsigned _NBx, _NBy; // nb sub component alongs X & Y axis (total = _NBx*_NBy)

  MPI_Comm _comm;
  int _rank;
  int _size;

  mandelbrot::MandelbrotManagement* _umbmg;
  mandelbrot::Array2DDump* _ua2dd;

  string _cmode;
  string _dmode;

public:
  llcm_MandelbrotRunMPI() : _R0(-1, -1), _R1(1,1), _Nx(256), _Ny(256),_NbIter(255), _dump(false), _data(NULL), 
										_NBx(1), _NBy(1),
										_umbmg(NULL), _ua2dd(NULL) {}
  virtual ~llcm_MandelbrotRunMPI() {}

  void sanity_check() {
	 if (_umbmg==NULL) {
		std::cerr << "[MBRMPI] **ERROR CONFIG**: port umbmg of node "<<_rank<<" not set\n";
		abort();
	 }
  }

  void alloc_data() {
#ifndef NDEBUG
	 std::cerr << "[MBRMPI] allocating memory Nx="<<_Nx<<" Ny="<<_Ny<<std::endl;
#endif
	 if (_data) {
		std::cerr << "[MBRMPI] **Warning: memory already allocated !!!\n";
	 }
	 if ((_data = (unsigned int*) malloc(_Nx*_Ny*sizeof(unsigned int)))==NULL) {
		std::cerr << "[MBRMPI] **ERROR** when allocating memory Nx="<<_Nx<<" Ny="<<_Ny<<std::endl;
		abort();
	 }
  }

  // Provides CC
  void configure_complete() {	 
	 MPI_Comm_rank (_comm, &_rank );
	 MPI_Comm_size (_comm, &_size );

	 sanity_check();
#ifndef NDEBUG
	 std::cerr << "[MB] configureComplete done\n";
#endif
  }

  // Provides MandelbrotManagement
  virtual void get_info(unsigned int*& ptr, std::complex<double>& r0, std::complex<double>& r1, unsigned& nx, unsigned& ny)
  {
	 ptr=_data;
	 r0=_R0;
	 r1=_R1;
	 nx=_Nx;
	 ny=_Ny;
  }

  virtual void compute()
  {	
#ifndef NDEBUG
	 std::cerr << "[MBRMPI] compute ... with mode="<<_cmode<<std::endl;
#endif
	 my_time_t tick_start, tick_gend, tick_lend;

	 MPI_Barrier(_comm);
	 get_time ( tick_start );

	 _umbmg->compute();
	 
	 get_time ( tick_lend );
	 MPI_Barrier(_comm);
	 get_time ( tick_gend );

	 long long time_usec = timing_diff_usec ( tick_start, tick_lend );
	 cout << _rank << " " <<_Nx << " " << _Ny << " " << _NbIter << " " << time_usec << " " << time_usec/1000000.0 << endl;

	 if (_rank==0) {
	 long long gtime_usec = timing_diff_usec ( tick_start, tick_gend );
	 cout << _rank << " " <<_Nx << " " << _Ny << " " << _NbIter << " " << gtime_usec << " " << gtime_usec/1000000.0 << endl;
	 }

  }

  void CHECK_MPI_ERR(int err, const char* msg) {
	 if (err!=MPI_SUCCESS) {
		cerr << "[MBRMPI] **MPI ERROR** err="<<err<<" with msg: "<<msg<<endl;
	 }
  }

  virtual void dump() {

	 MPI_Barrier(_comm);

	 if (_dump) {
		if (_dmode.compare("EACH")==0) {
#ifdef DEBUG
		  std::cerr << "[MBRMPI] Each sub component dumping"<<endl;
#endif		
		  _umbmg->dump();
		} else if (_dmode.compare("ONE")==0) {
		  
#ifdef DEBUG
		  std::cerr << "[MBRMPI] RunMulti component dumping"<<endl;
#endif
		  if (_rank==0) {
			 if (!_ua2dd) {
				std::cerr << "[MBRMPI] **ERROR: Can not dump as ua2dd is not conntected!!"<<endl;
				abort();
			 }
			 alloc_data();

			 // local contribution
			 std::complex<double> r0, r1;
			 unsigned nx, ny;
			 unsigned* ptr;
			 
			 _umbmg->get_info(ptr, r0, r1, nx, ny);

			 int pi = (_NBx * 1.0 * (r0.real()-_R0.real()) / (_R1.real()-_R0.real()));
			 int pj = (_NBy * 1.0 * (r0.imag()-_R0.imag()) / (_R1.imag()-_R0.imag()));
#ifdef DEBUG
			 std::cerr << "[MBRMPI] local copy [ "<<r0<<" - "<<r1<<"] pi="<<pi<<" pj="<<pj<<endl;
#endif
			 for(unsigned int j=0; j<ny; j++)
				for(unsigned int i=0; i<nx; i++)
				  _data[pi*nx+i+(pj*ny+j)*_Nx] = ptr[i+j*nx];				  
#ifdef DEBUG
			 std::cerr << "[MBRMPI] local copy done\n";
#endif

		  }

		  MPI_Barrier(_comm);
		  
		  // Each MPI process sends its contribution
		  for(int r=1; r<_size; r++) {
			 std::cerr << "[MBRMPI] process r="<<r<<endl;
			 if (_rank==r) {
				MPI_Request rq[4];
				MPI_Status statuses[4];
				int err;

				std::complex<double> r0, r1;
				unsigned nx, ny;
				unsigned* ptr;

				_umbmg->get_info(ptr, r0, r1, nx, ny);
				cerr << "[MBRMPI] sending: r0="<<r0<<" r1="<<r1<<" nx="<<nx<<" ny="<<ny<<endl;

				err=MPI_Isend(&r0, 1, MPI::DOUBLE_COMPLEX, 0, 123, _comm, &rq[0]);
				CHECK_MPI_ERR(err, "Error when sending r0 to node 0");

				err=MPI_Isend(&r1, 1, MPI::DOUBLE_COMPLEX, 0, 123, _comm, &rq[1]);
				CHECK_MPI_ERR(err, "Error when sending rx to node 0");

				err=MPI_Isend(&nx, 1, MPI_UNSIGNED, 0, 123, _comm, &rq[2]);
				CHECK_MPI_ERR(err, "Error when sending nx to node 0");

				err=MPI_Isend(&ny, 1, MPI_UNSIGNED, 0, 123, _comm, &rq[3]);
				CHECK_MPI_ERR(err, "Error when sending ny to node 0");				

				err=MPI_Waitall(2, &rq[2], statuses);
				CHECK_MPI_ERR(err, "Error when waiting Isend");				

				err=MPI_Send(ptr, nx*ny, MPI_UNSIGNED, 0, 123, _comm );
				CHECK_MPI_ERR(err, "Error when sending data to node 0");
			 }
			 if (_rank==0) {
				MPI_Request rq[4];
				MPI_Status statuses[4];
				int err;

				std::complex<double> r0, r1;
				unsigned nx, ny;

				err=MPI_Irecv(&r0, 1, MPI::DOUBLE_COMPLEX, r, 123, _comm, &rq[0]);
				CHECK_MPI_ERR(err, "Error when sending r0 to node 0");
				err=MPI_Irecv(&r1, 1, MPI::DOUBLE_COMPLEX, r, 123, _comm, &rq[1]);
				CHECK_MPI_ERR(err, "Error when sending rx to node 0");

				err=MPI_Irecv(&nx, 1, MPI_UNSIGNED, r, 123, _comm, &rq[2]);
				CHECK_MPI_ERR(err, "Error when sending nx to node 0");
				err=MPI_Irecv(&ny, 1, MPI_UNSIGNED, r, 123, _comm, &rq[3]);
				CHECK_MPI_ERR(err, "Error when sending ny to node 0");

				err=MPI_Waitall(4, rq, statuses) ;

#ifdef DEBUG
				cerr << "[MBRMPI] receive: r0="<<r0<<" r1="<<r1<<" nx="<<nx<<" ny="<<ny<<endl;
#endif

				unsigned int* ptr;
				if ((ptr = (unsigned int*) malloc(nx*ny*sizeof(unsigned int)))==NULL) {
				  std::cerr << "[MBRMPI] **ERROR** when temporaru allocating memory nx="<<nx<<" ny="<<ny<<std::endl;
				  abort();
				}

				err=MPI_Recv(ptr, nx*ny, MPI_UNSIGNED, r, 123, _comm, &statuses[0]);

				int pi = (_NBx * 1.0 * (r0.real()-_R0.real()) / (_R1.real()-_R0.real()));
				int pj = (_NBy * 1.0 * (r0.imag()-_R0.imag()) / (_R1.imag()-_R0.imag()));
#ifdef DEBUG
				std::cerr << "[MBRMPI] [ "<<r0<<" - "<<r1<<"] pi="<<pi<<" pj="<<pj<<endl;
#endif
				for(unsigned int j=0; j<ny; j++)
				  for(unsigned int i=0; i<nx; i++)
					 _data[pi*nx+i+(pj*ny+j)*_Nx] = ptr[i+j*nx];
				free(ptr);
			 }
		  }

		  MPI_Barrier(_comm);
		  if (_rank==0) {
			 _ua2dd->dump(_data, _Nx, _Ny, _NbIter,_dumpfile.c_str());
		  }
		}
	 }
  }

  // Provides GO
  virtual void go() {
#ifndef NDEBUG
	 std::cerr << "[MBRMPIPI] go ... "<<std::endl;
#endif

	 compute();
	 dump();

#ifndef NDEBUG
	 std::cerr << "[MBRMPI] done "<<std::endl;
#endif
  }

  virtual void* get(const char* pname) {
	 if (strcmp(pname, "go")==0) {
		return static_cast<Go*>(this);
	 } else if (strcmp(pname, "lifecycle")==0) {
		return static_cast<LifeCycle*>(this);
	 } else if (strcmp(pname, "pmbmg")==0) {
		return static_cast<mandelbrot::MandelbrotManagement*>(this);
	 } else {
		std::cerr << "[MBRMPI] get : unknown port "<<pname<<" (returning NULL)\n";
		return 0;
	 }
  }

  virtual void set(const char* pname, void* value) {
	 if (strcmp(pname, "comm")==0) {
#ifndef NDEBUG
		cerr << "[MBRMPI] comm set\n";
#endif
		_comm = * static_cast<MPI_Comm*>(value);
	 } else if (strcmp(pname, "umbmg")==0) { 
#ifndef NDEBUG
		cerr << "[MBRMPI] umbmg set\n";
#endif
		_umbmg=static_cast<mandelbrot::MandelbrotManagement*>(value);
  } else if (strcmp(pname, "ua2dd")==0) { 
#ifndef NDEBUG
		cerr << "[MBRMPI] ua2dd set\n";
#endif
		_ua2dd=static_cast<mandelbrot::Array2DDump*>(value);
  } else if (strcmp(pname, "CMODE")==0) {
		_cmode = * (std::string*) value;
		std::cerr << "[MBRMPI] CMODE set to "<<_cmode<<std::endl;
	 } else if (strcmp(pname, "DMODE")==0) { 
		_dmode = * (std::string*) value;
		std::cerr << "[MBRMPI] DMODE set to "<<_dmode<<std::endl;
	 } else if (strcmp(pname, "NBX")==0) { 
		_NBx = (unsigned long) (value);
		std::cerr << "[MBRMPI] NBX set to "<<_NBx<<std::endl;
	 } else if (strcmp(pname, "NBY")==0) { 
		_NBy = (unsigned long) (value);
		std::cerr << "[MBRMPI] NBY set to "<<_NBy<<std::endl;
	 }	 else if (strcmp(pname, "DUMP")==0) { 
		_dump = (unsigned long) value;
		std::cerr << "[MBRMPI] DUMP set to "<<_dump<<std::endl;
	 } else if (strcmp(pname, "DUMPFILE")==0) { 
		_dumpfile = * (std::string*) value;
		std::cerr << "[MBRMPI] DUMPFILE set to "<<_dumpfile<<std::endl;
	 } else if (strcmp(pname, "R0")==0) { 
		_R0 = * (complex<double>*) (value);
		std::cerr << "[MBRMPI] R0 set to "<<_R0<<std::endl;
	 } else if (strcmp(pname, "R1")==0) { 
		_R1 = * (complex<double>*) (value);
		std::cerr << "[MBRMPI] R1 set to "<<_R1<<std::endl;
	 } else if (strcmp(pname, "NX")==0) { 
		_Nx = (unsigned long) (value);
		std::cerr << "[MBRMPI] NX set to "<<_Nx<<std::endl;
	 } else if (strcmp(pname, "NY")==0) { 
		_Ny = (unsigned long) (value);
		std::cerr << "[MBRMPI] NY set to "<<_Ny<<std::endl;
	 } else if (strcmp(pname, "NBITER")==0) { 
		_NbIter = (unsigned long) (value);
		std::cerr << "[MBRMPI] NbIter set to "<<_NbIter<<std::endl;
	 } else {
		std::cerr << "[MBRMPI] set : unknown port "<<pname<<std::endl;
	 }
  }
};

}

}

using mandelbrot::impl::llcm_MandelbrotRunMPI;

LLCMCPP_CREATE(llcm_MandelbrotRunMPI)
