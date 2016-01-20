#include <complex>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

#include <llcmcpp/llcmcpp.h>
#include <time.h>
#include <sys/time.h>

#include "Mandelbrot.h"

typedef struct timeval my_time_t;
#define get_time(a) gettimeofday(&a, NULL)
#define timing_diff_usec(a,b) (((long long)(b).tv_sec - (a).tv_sec)*1000000+((long long)(b).tv_usec - (a).tv_usec))

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

namespace mxn
{

namespace impl
{

class MandelbrotBase : virtual public llcmcpp::Component, virtual public Go, virtual public ConfigureComplete,
							  virtual public DC::Mandelbrot
{
protected:

  std::complex<double> _R0; // lower left point of the rectangle
  std::complex<double> _R1; // upper right point of the rectangle
  unsigned int _Nx;
  unsigned int _Ny;
  unsigned int _NbIter;

  unsigned int _dump; 
  std::string _dumpfile;

  unsigned int* _data;

public:
  MandelbrotBase() : _R0(-1, -1), _R1(1,1), _Nx(256), _Ny(256),_NbIter(255), _dump(false), _data(NULL) { }
  virtual ~MandelbrotBase() {}

  virtual void sanity_check() {}

  virtual void init() {
#ifdef DEBUG
	 std::cerr << "[Mandel] allocating memory Nx="<<_Nx<<" Ny="<<_Ny<<std::endl;
#endif
	 if ((_data = (unsigned int*) malloc(_Nx*_Ny*sizeof(unsigned int)))==NULL) {
		std::cerr << "[Mandel] error when allocating memory Nx="<<_Nx<<" Ny="<<_Ny<<std::endl;
		abort();
	 }
  }

  void configureComplete() {
	 sanity_check();
	 init();
#ifdef DEBUG
	 std::cerr << "[MB] configureComplete done\n";
#endif
  }


  virtual void get_info(unsigned int*& ptr, std::complex<double>& r0, std::complex<double>& r1, unsigned& nx, unsigned& ny)
  {
	 ptr=_data;
	 r0=_R0;
	 r1=_R1;
	 nx=_Nx;
	 ny=_Ny;
  }

  virtual void compute(unsigned int* ptr, std::complex<double> r0, std::complex<double> r1, unsigned nx, unsigned ny, unsigned int nbiter)=0;

  virtual void compute()
  {
	 compute(_data, _R0, _R1, _Nx, _Ny, _NbIter);
  }

  void dump() {
#ifdef DEBUG
	 std::cerr << "[Mandel] dumping ... "<<std::endl;
#endif
	 std::stringstream ss;
	 for ( int j=_Ny-1; j>=0; j--) {
		for(unsigned int i=0; i<_Nx; i++) {
		  if (_dump==1)
			 ss << _data[(j*_Nx)+i] << " ";
		  else {
			 if (_data[(j*_Nx)+i] >= _NbIter)
				ss << ".";
			 else 
				ss << "*";			 
		  }
		}
		ss << std::endl;
	 }
	 if (_dumpfile.size()>0) {
		std::ofstream of;
		of.open(_dumpfile.c_str(), std::ios::out | std::ios::trunc);
		// HEADER of PGM FILE
		of << "P2\n"<<_Nx<<" "<<_Ny<<"\n"<<_NbIter<<"\n";
		of << ss.str();
		of.close();
	 } else {		  
		std::cout << ss.str();
	 }
  }

  virtual void go() {
#ifdef DEBUG
	 std::cerr << "[Mandel] go ... "<<std::endl;
#endif
	 sanity_check();
	 init();

	 compute();

	 if (_dump)
		dump();

#ifdef DEBUG
	 std::cerr << "[Mandel] freeing mem ... "<<std::endl;
#endif
	 free(_data);
  }

  virtual void* get(const char* pname) {
	 if (strcmp(pname, "go")==0) {
		return static_cast<Go*>(this);
	 } else if (strcmp(pname, "cc")==0) {
		return static_cast<ConfigureComplete*>(this);
	 } else if (strcmp(pname, "pmb")==0) {
		return static_cast<DC::Mandelbrot*>(this);
	 } else {
		std::cerr << "[Mandel] get : unknown port "<<pname<<" (returning NULL)\n";
		return 0;
	 }
  }

  virtual void set(const char* pname, void* value) {
	 if (strcmp(pname, "DUMP")==0) { 
		_dump = (unsigned long) value;
		std::cerr << "[Mandel] DUMP set to "<<_dump<<std::endl;
	 } else if (strcmp(pname, "DUMPFILE")==0) { 
		_dumpfile = * (std::string*) value;
		std::cerr << "[Mandel] DUMPFILE set to "<<_dumpfile<<std::endl;
	 } else if (strcmp(pname, "RX0")==0) { 
		_R0.real() = * (double*) (value);
		std::cerr << "[Mandel] RX0 set to "<<_R0.real()<<std::endl;
	 } else if (strcmp(pname, "RY0")==0) { 
		_R0.imag() = * (double*) (value);
		std::cerr << "[Mandel] RY0 set to "<<_R0.imag()<<std::endl;
	 } else if (strcmp(pname, "RX1")==0) { 
		_R1.real() = * (double*) (value);
		std::cerr << "[Mandel] RX1 set to "<<_R1.real()<<std::endl;
	 } else if (strcmp(pname, "RY1")==0) { 
		_R1.imag() = *(double*) (value);
		std::cerr << "[Mandel] RY1 set to "<<_R1.imag()<<std::endl;
	 } else if (strcmp(pname, "NX")==0) { 
		_Nx = (unsigned long) (value);
		std::cerr << "[Mandel] NX set to "<<_Nx<<std::endl;
	 } else if (strcmp(pname, "NY")==0) { 
		_Ny = (unsigned long) (value);
		std::cerr << "[Mandel] NY set to "<<_Ny<<std::endl;
	 } else if (strcmp(pname, "NBITER")==0) { 
		_NbIter = (unsigned long) (value);
		std::cerr << "[Mandel] NbIter set to "<<_NbIter<<std::endl;
	 } else {
		std::cerr << "[Mandel] set : unknown port "<<pname<<std::endl;
	 }
  }

};

}

}
