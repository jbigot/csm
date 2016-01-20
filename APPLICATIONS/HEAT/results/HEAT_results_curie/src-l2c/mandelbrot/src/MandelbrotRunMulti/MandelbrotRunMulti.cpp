#include <complex>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>

#include <llcmcpp/lifecycle.h>
#include <llcmcpp/go.h>
#include <llcmcpp/llcmcpp.h>
#include <omp.h>

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

class llcm_MandelbrotRunMulti : virtual public llcmcpp::Component, virtual public Go, virtual public LifeCycle,
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

  // The domain is already subdivided in sub component 
  typedef vector<mandelbrot::MandelbrotManagement*> umbmgs_t;
  umbmgs_t _umbmgs;

  mandelbrot::Array2DDump* _ua2dd;

  string _cmode;
  string _dmode;

public:
  llcm_MandelbrotRunMulti() : _R0(-1, -1), _R1(1,1), _Nx(256), _Ny(256),_NbIter(255), _dump(false), _data(NULL), 
										_NBx(1), _NBy(1),
										_ua2dd(NULL), _cmode("SEQ"), _dmode("ONE") {}
  virtual ~llcm_MandelbrotRunMulti() {}

  void sanity_check() {}

  void init() {
#ifdef DEBUG
	 std::cerr << "[MBRM] allocating memory Nx="<<_Nx<<" Ny="<<_Ny<<std::endl;
#endif
	 if ((_data = (unsigned int*) malloc(_Nx*_Ny*sizeof(unsigned int)))==NULL) {
		std::cerr << "[MBRM] error when allocating memory Nx="<<_Nx<<" Ny="<<_Ny<<std::endl;
		abort();
	 }
  }

  // Provides CC
  void configureComplete() {
	 sanity_check();
	 init();
#ifdef DEBUG
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
#ifdef DEBUG
	 std::cerr << "[MBRM] compute ... with mode="<<_cmode<<std::endl;
#endif
	 my_time_t tick_start, tick_end;
	 get_time ( tick_start );

	 if (_cmode.compare("SEQ")==0) {
#ifdef DEBUG
		std::cerr << "[MBRM] SEQ computing with #sub component="<<_umbmgs.size()<<endl;
#endif
		for(umbmgs_t::const_iterator it=_umbmgs.begin(); it!=_umbmgs.end(); it++)
		  (*it)->compute();
	 } else 	if (_cmode.compare("OMP")==0) {
#ifdef DEBUG
		std::cerr << "[MBRM] OMP computing with #sub component="<<_umbmgs.size()<<endl;
#endif
#pragma omp parallel
		{
#pragma omp for
		  for(unsigned int i=0; i<_umbmgs.size(); i++)
			 _umbmgs[i]->compute();
#pragma omp barrier
		}
	 }
	 
	 get_time ( tick_end );

	 long long time_usec = timing_diff_usec ( tick_start, tick_end );
	 cout << _Nx << " " << _Ny << " " << _NbIter << " " << time_usec << " " << time_usec/1000000.0 << endl;

  }

  virtual void dump() {

	 if (_dump) {
		if (_dmode.compare("EACH")==0) {
#ifdef DEBUG
		  std::cerr << "[MBRM] Each sub component dumping"<<endl;
#endif		
		  for(umbmgs_t::const_iterator it=_umbmgs.begin(); it!=_umbmgs.end(); it++)
			 (*it)->dump();
		} else if (_dmode.compare("ONE")==0) {

		  if (!_ua2dd) {
			 std::cerr << "[MBRM] **ERROR: Can not dump as ua2dd is not conntected!!"<<endl;
			 abort();
		  }

#ifdef DEBUG
		  std::cerr << "[MBRM] RunMulti component dumping"<<endl;
#endif
		  init();
		  for(umbmgs_t::const_iterator it=_umbmgs.begin(); it!=_umbmgs.end(); it++) {
			 unsigned int* ptr;
			 std::complex<double> r0, r1;
			 unsigned nx, ny;
			 (*it)->get_info(ptr, r0, r1, nx, ny);
			 int pi = (_NBx * 1.0 * (r0.real()-_R0.real()) / (_R1.real()-_R0.real()));
			 int pj = (_NBy * 1.0 * (r0.imag()-_R0.imag()) / (_R1.imag()-_R0.imag()));
#ifdef DEBUG
			 std::cerr << "[MBRM] [ "<<r0<<" - "<<r1<<"] pi="<<pi<<" pj="<<pj<<endl;
#endif
			 for(unsigned int j=0; j<ny; j++)
				for(unsigned int i=0; i<nx; i++)
				  _data[pi*nx+i+(pj*ny+j)*_Nx] = ptr[i+j*nx];
		  }
		  
		  _ua2dd->dump(_data, _Nx, _Ny, _NbIter,_dumpfile.c_str());
		}	
	 }
  }

  // Provides GO
  virtual void go() {
#ifdef DEBUG
	 std::cerr << "[MBRM] go ... "<<std::endl;
#endif

	 compute();
	 dump();

#ifdef DEBUG
	 std::cerr << "[MBRM] done "<<std::endl;
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
		std::cerr << "[MBRM] get : unknown port "<<pname<<" (returning NULL)\n";
		return 0;
	 }
  }

  virtual void set(const char* pname, void* value) {
	 if (strcmp(pname, "umbmgs")==0) {
#ifdef DEBUG
		cerr << "[MBRM] umbmgs set\n";
#endif
		_umbmgs.push_back(static_cast<mandelbrot::MandelbrotManagement*>(value));
	 } else if (strcmp(pname, "ua2dd")==0) { 
#ifdef DEBUG
		cerr << "[MBRM] ua2dd set\n";
#endif
		_ua2dd=static_cast<mandelbrot::Array2DDump*>(value);
  } else if (strcmp(pname, "CMODE")==0) {
		_cmode = * (std::string*) value;
		std::cerr << "[MBRM] CMODE set to "<<_cmode<<std::endl;
	 } else if (strcmp(pname, "DMODE")==0) { 
		_dmode = * (std::string*) value;
		std::cerr << "[MBRM] DMODE set to "<<_dmode<<std::endl;
	 } else if (strcmp(pname, "NBX")==0) { 
		_NBx = (unsigned long) (value);
		std::cerr << "[MBRM] NBX set to "<<_NBx<<std::endl;
	 } else if (strcmp(pname, "NBY")==0) { 
		_NBy = (unsigned long) (value);
		std::cerr << "[MBRM] NBY set to "<<_NBy<<std::endl;
	 }	 else if (strcmp(pname, "DUMP")==0) { 
		_dump = (unsigned long) value;
		std::cerr << "[MBRM] DUMP set to "<<_dump<<std::endl;
	 } else if (strcmp(pname, "DUMPFILE")==0) { 
		_dumpfile = * (std::string*) value;
		std::cerr << "[MBRM] DUMPFILE set to "<<_dumpfile<<std::endl;
	 } else if (strcmp(pname, "R0")==0) { 
		_R0 = * (complex<double>*) (value);
		std::cerr << "[MBRM] R0 set to "<<_R0<<std::endl;
	 } else if (strcmp(pname, "R1")==0) { 
		_R1 = * (complex<double>*) (value);
		std::cerr << "[MBRM] R1 set to "<<_R1<<std::endl;
	 } else if (strcmp(pname, "NX")==0) { 
		_Nx = (unsigned long) (value);
		std::cerr << "[MBRM] NX set to "<<_Nx<<std::endl;
	 } else if (strcmp(pname, "NY")==0) { 
		_Ny = (unsigned long) (value);
		std::cerr << "[MBRM] NY set to "<<_Ny<<std::endl;
	 } else if (strcmp(pname, "NBITER")==0) { 
		_NbIter = (unsigned long) (value);
		std::cerr << "[MBRM] NbIter set to "<<_NbIter<<std::endl;
	 } else {
		std::cerr << "[MBRM] set : unknown port "<<pname<<std::endl;
	 }
  }
};

}

}

using mandelbrot::impl::llcm_MandelbrotRunMulti;

LLCMCPP_CREATE(llcm_MandelbrotRunMulti)
