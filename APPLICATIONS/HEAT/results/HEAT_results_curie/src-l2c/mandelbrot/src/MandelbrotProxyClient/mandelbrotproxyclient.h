#ifndef MANDELBROT_PROXY_CLIENT_IS_DEFINED_H
#define MANDELBROT_PROXY_CLIENT_IS_DEFINED_H

#include "Mandelbrot.h"
#include "CorbaMandelbrot.h"
#include <llcmcpp/llcmcpp.h>

namespace mandelbrot {

namespace impl {

class MandelbrotProxyClient : 
	 virtual public llcmcpp::Component,
	 virtual public mandelbrot::MandelbrotManagement
{
public:
  MandelbrotProxyClient();
  virtual ~MandelbrotProxyClient() {}

  // MandelbrotManagement
  virtual void dump();
  virtual void get_info(uint32_t*& ptr, std::complex<double>& r0, std::complex<double>& r1, uint32_t& nx, uint32_t& ny);
  virtual void compute();

  // Component
  virtual void* get(const char* pname);
  virtual void set(const char* pname, void* value);

private:
  cmandelbrot::MandelbrotManagement_ptr m_ucmbmg;

  CORBA::ORB_ptr _orb;
};

}

}

#endif 
