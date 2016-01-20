#ifndef _ITF_MANDELBROT_H
#define _ITF_MANDELBROT_H

#include<complex>
#include <stdint.h>

namespace mandelbrot {

  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  
  class Mandelbrot
  {
  public:
	 virtual void compute(uint32_t* ptr, std::complex<double> r0, std::complex<double> r1, uint32_t nx, uint32_t ny, uint32_t nbiter)=0;	 
	 /*
	 virtual void compute(unsigned int* ptr, 
								 std::complex<double> r0, std::complex<double> r1, 
								 unsigned nx, unsigned ny,  // Gloabl domain
								 unsigned lx, unsigned ly,  // Local domain to use for this computation
								 unsigned nbiter)=0;	 
	 */
  };

  class MandelbrotManagement
  {
  public:
	 virtual void dump()=0;
	 virtual void get_info(uint32_t*& ptr, std::complex<double>& r0, std::complex<double>& r1, uint32_t& nx, uint32_t& ny)=0;	 
	 virtual void compute()=0;
	 /*
	 virtual void compute(unsigned int* ptr, 
								 std::complex<double> r0, std::complex<double> r1, 
								 unsigned nx, unsigned ny,  // Gloabl domain
								 unsigned lx, unsigned ly,  // Local domain to use for this computation
								 unsigned nbiter)=0;	 
	 */
  };

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

} // namespace

#endif
