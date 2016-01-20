#include <cassert>
#include <cstring>
#include <iostream>

#include <llcmcpp/llcmcpp.h>

#include "Mandelbrot.h"

#define DEBUG

using namespace std;

namespace mandelbrot
{

namespace impl
{

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

class llcm_MandelbrotSeq :  virtual public llcmcpp::Component, virtual public mandelbrot::Mandelbrot
{
private:

public:
  llcm_MandelbrotSeq()  {}
  virtual ~llcm_MandelbrotSeq() {}

  virtual void compute(unsigned int* ptr, complex<double> r0, complex<double> r1, unsigned nx, unsigned ny, unsigned int nbiter)
  {
#ifdef DEBUG
	 cerr << "[MBSeq] Computing NbIter="<<nbiter<<endl;
	 cerr << "[MBSeq] R0="<<r0<<endl;
	 cerr << "[MBSeq] R1="<<r1<<endl;
	 cerr << "[MBSeq] NX="<<nx<<endl;
	 cerr << "[MBSeq] NY="<<ny<<endl;
#endif

	 for (unsigned int j=0; j<ny; j++) {
		for(unsigned int i=0; i<nx; i++) {
		  complex<double> c0( (r1.real()-r0.real())*i/(1.0*nx)+r0.real(),
									 (r1.imag()-r0.imag())*j/(1.0*ny)+r0.imag());
		  complex<double> c=c0;
		  unsigned k=0;
		  for (; k<nbiter; k++)  {
			 complex<double> t = c*c+c0;
			 if (norm(t)>2) break;
			 c=t;
		  }
		  ptr[(j*nx)+i] = k;
		}
	 }
#ifdef DEBUG
	 cerr << "[MBSeq] Done\n";
#endif
  }

  virtual void* get(const char* pname) {
	 if (strcmp(pname, "pmb")==0) {
		return static_cast<mandelbrot::Mandelbrot*>(this);
	 } else {
		std::cerr << "[MandelSeq] get : unknown port "<<pname<<" (returning NULL)\n";
		return 0;
	 }
  }

  virtual void set(const char* pname, void* value) {
	 std::cerr << "[MandelSeq] set : unknown port "<<pname<<std::endl;
  }

};

}

}

using mandelbrot::impl::llcm_MandelbrotSeq;

LLCMCPP_CREATE(llcm_MandelbrotSeq)
