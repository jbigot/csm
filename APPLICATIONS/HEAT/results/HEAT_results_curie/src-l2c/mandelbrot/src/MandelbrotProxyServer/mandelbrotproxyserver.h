#ifndef MANDELBROT_PROXY_SERVER_IS_DEFINED_H
#define MANDELBROT_PROXY_SERVER_IS_DEFINED_H

#include <Mandelbrot.h>
#include "CorbaMandelbrot.h"

#include <llcmcpp/llcmcpp.h>

namespace mandelbrot {

namespace impl {

class MandelbrotProxyServer :
	 virtual public llcmcpp::Component,
	 virtual public POA_cmandelbrot::MandelbrotManagement /* CORBA Interface */
{
public:
  MandelbrotProxyServer ();
  virtual ~MandelbrotProxyServer() {}

  // CORBA cmandelbrot::MandelbrotManagement 
  void dump();
  void get_info(cmandelbrot::dataULong_t_out data, cmandelbrot::area_t_out area, ::CORBA::ULong& nx, ::CORBA::ULong& ny);
  void compute();
  
  // llcmcpp::Component
  virtual void* get(const char* pname);
  virtual void  set(const char* pname, void* value);

private:  
  mandelbrot::MandelbrotManagement* m_umbmg;

  cmandelbrot::MandelbrotManagement_ptr m_pcmbmg;
  CORBA::Object_ptr m_pombmg;

  CORBA::ORB_ptr _orb;
};

}

}
#endif
