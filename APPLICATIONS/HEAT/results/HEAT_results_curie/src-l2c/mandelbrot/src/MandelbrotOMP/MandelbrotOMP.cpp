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

class llcm_MandelbrotOMP :  virtual public llcmcpp::Component, virtual public mandelbrot::Mandelbrot
{
private:

public:
  llcm_MandelbrotOMP()  {}
  virtual ~llcm_MandelbrotOMP() {}

  virtual void compute(uint32_t* ptr, complex<double> r0, complex<double> r1, uint32_t nx, uint32_t ny, uint32_t nbiter)
  {
	 stringstream ss;
#ifdef DEBUG
	 ss << "[MandelOMP] Computing NbIter="<<nbiter<<endl;
	 cerr << ss.str();
#endif
	
#pragma omp parallel for
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
	 }
  

  virtual void* get(const char* pname) {
	 if (strcmp(pname, "pmb")==0) {
		return static_cast<mandelbrot::Mandelbrot*>(this);
	 } else {
		std::cerr << "[MandelOMP] get : unknown port "<<pname<<" (returning NULL)\n";
		return 0;
	 }
  }

  virtual void set(const char* pname, void* value) {
	 std::cerr << "[MandelOMP] set : unknown port "<<pname<<std::endl;
  }

};

}

}

using mandelbrot::impl::llcm_MandelbrotOMP;

LLCMCPP_CREATE(llcm_MandelbrotOMP)
