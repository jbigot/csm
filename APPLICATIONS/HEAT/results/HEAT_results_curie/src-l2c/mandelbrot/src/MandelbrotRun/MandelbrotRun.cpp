#include <cassert>
#include <complex>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>

#include <llcmcpp/lifecycle.h> /* new */
#include <llcmcpp/go.h>
#include <llcmcpp/llcmcpp.h>

#include "Mandelbrot.h"
#include "Array2DDump.h"

#include "../mytime.h"

#define DEBUG

namespace mandelbrot
{

namespace impl
{

using namespace llcmcpp;
using namespace std;

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

// Component that old information about _ONE_ domain
// Use: MB

class llcm_MandelbrotRun :  virtual public llcmcpp::Component, virtual public Go,
									 virtual public mandelbrot::MandelbrotManagement,
									 virtual public LifeCycle /* new */
{
protected:

  std::complex<double> _R0; // lower left point of the rectangle
  std::complex<double> _R1; // upper right point of the rectangle
  unsigned int _Nx;
  unsigned int _Ny;
  unsigned int _NbIter;

  bool _dump;
  std::string _dumpfile;

  unsigned int* _data;

  // use port umb
  mandelbrot::Mandelbrot* _umb;
  mandelbrot::Array2DDump* _ua2dd;

public:
  llcm_MandelbrotRun() : _R0(-1, -1), _R1(1,1), _Nx(256), _Ny(256),_NbIter(255), _dump(false), _data(NULL), _umb(NULL), _ua2dd(NULL) {}
  ~llcm_MandelbrotRun() {}

  void sanity_check() {}

  void init() {
#ifdef DEBUG
	 std::cerr << "[Mandel] allocating memory Nx="<<_Nx<<" Ny="<<_Ny<<std::endl;
#endif
	 if ((_data = (unsigned int*) malloc(_Nx*_Ny*sizeof(unsigned int)))==NULL) {
		std::cerr << "[Mandel] error when allocating memory Nx="<<_Nx<<" Ny="<<_Ny<<std::endl;
		abort();
	 }
  }

  // Provides Configuration complete
  virtual void configureComplete() {
	 this->configureComplete();
  }

  virtual void configure_complete() {
	 sanity_check();
	 init();
#ifdef DEBUG
	 std::cerr << "[MB] configureComplete done\n";
#endif
  }

  // Provides mandelbrot::MandelbrotManagement
  virtual void dump()
  {
#ifdef DEBUG
	 std::cerr << "[MBRun] dumping (_dump="<<_dump<<" ua2dd="<<_ua2dd<<")\n";
	 std::cerr << "[MBRun] dumping (NX="<<_Nx<<" NY="<<_Ny<<" NbIter="<<_NbIter<<")\n";
#endif
	 if (_dump && _ua2dd)
		_ua2dd->dump(_data, _Nx, _Ny, _NbIter,_dumpfile.c_str());
  }

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
	 std::cerr << "[Mandel] compute (umb="<<_umb<<") "<<std::endl;
#endif


	 my_time_t tick_start, tick_end;
	 get_time ( tick_start );

	 _umb->compute(_data, _R0, _R1, _Nx, _Ny, _NbIter);

	 get_time ( tick_end );

	 long long time_usec = timing_diff_usec ( tick_start, tick_end );
	 cout << _Nx << " " << _Ny << " " << _NbIter << " " << time_usec << " " << time_usec/1000000.0 << endl;
  }

  // Provides go
  virtual void go() {
#ifdef DEBUG
	 std::cerr << "[Mandel] go ... "<<std::endl;
#endif
	 sanity_check();
	 init();

	 compute();

	 dump();

#ifdef DEBUG
	 std::cerr << "[Mandel] freeing mem ... "<<std::endl;
#endif
	 free(_data);
  }

  virtual void* get(const char* pname) {
	 if (strcmp(pname, "go")==0) {
		return static_cast<Go*>(this);
	 } else if (strcmp(pname, "lifecycle")==0) {
		return static_cast<LifeCycle*>(this);
	 } else if (strcmp(pname, "pmbmg")==0) {
		return static_cast<mandelbrot::MandelbrotManagement*>(this);
	 } else {
		std::cerr << "[Mandel] get : unknown port "<<pname<<" (returning NULL)\n";
		return 0;
	 }
  }

  virtual void set(const char* pname, void* value) {
	 if (strcmp(pname, "umb")==0) { 
		_umb = static_cast<mandelbrot::Mandelbrot*>(value);
		std::cerr << "[Mandel] umb set\n";
	 } else if (strcmp(pname, "ua2dd")==0) { 
		_ua2dd = static_cast<mandelbrot::Array2DDump*>(value);
		std::cerr << "[Mandel] ua2dd set\n";
	 } else if (strcmp(pname, "DUMP")==0) { 
		_dump = (bool) value;
		std::cerr << "[Mandel] DUMP set to "<<_dump<<std::endl;
	 } else if (strcmp(pname, "DUMPFILE")==0) { 
		_dumpfile = * (std::string*) value;
		std::cerr << "[Mandel] DUMPFILE set to "<<_dumpfile<<std::endl;
	 } else if (strcmp(pname, "R0")==0) { 
		_R0 = * (complex<double>*) (value);
		std::cerr << "[Mandel] R0 set to "<<_R0<<std::endl;
	 } else if (strcmp(pname, "R1")==0) { 
		_R1 = * (complex<double>*) (value);
		std::cerr << "[Mandel] R1 set to "<<_R1<<std::endl;
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

using mandelbrot::impl::llcm_MandelbrotRun;

LLCMCPP_CREATE(llcm_MandelbrotRun)
