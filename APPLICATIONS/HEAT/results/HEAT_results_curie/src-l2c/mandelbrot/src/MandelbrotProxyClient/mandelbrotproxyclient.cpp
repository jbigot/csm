#include <stdlib.h>
#include <iostream>

#include <llcmcpp/llcmcpp.h>

#include "mandelbrotproxyclient.h"

using namespace std;
using namespace mandelbrot::impl;

MandelbrotProxyClient::MandelbrotProxyClient() : m_ucmbmg(NULL)
{
  cerr << "[MandelbrotProxyClient] creating...!\n";
  char* NO[2] = { 0, 0 };
  NO[0] = strdup("a.out");
  int ac=1;
  _orb = CORBA::ORB_init (ac, NO);
}

void
MandelbrotProxyClient::dump()
{
  m_ucmbmg->dump();
}

void
MandelbrotProxyClient::get_info(uint32_t*& ptr, std::complex<double>& r0, std::complex<double>& r1, uint32_t& nx, uint32_t& ny)
{
  cerr << "[MandelbrotProxyClient] get info...\n";
  
  cmandelbrot::dataULong_t_var data;
  cmandelbrot::area_t_var area;
  CORBA::ULong cnx, cny;
  m_ucmbmg->get_info(data, area, cnx, cny);
  //  cerr << "[MandelbrotProxyClient] get info  returns: data.len="<<data->length()<<" nx="<<cnx<<" ny="<<cny<<endl;
  r0 = complex<double>(area[0], area[1]);
  r1 = complex<double>(area[2], area[3]);
  nx=cnx;
  ny=cny;
  ptr=(unsigned int*) malloc(sizeof(unsigned int)*nx*ny);
  cerr << __LINE__ <<endl;
  for(unsigned int j=0; j<ny; j++)
	 for(unsigned int i=0; i<nx; i++)
		ptr[j*nx+i]=data[j*nx+i];
  cerr << "[MandelbrotProxyClient] get info...done\n";
}

void
MandelbrotProxyClient::compute()
{
  m_ucmbmg->compute();
}

void*
MandelbrotProxyClient::get(const char* pname) {
  if (strcmp(pname, "pmbmg")==0) {
	 return static_cast<mandelbrot::MandelbrotManagement*>(this);
  } else {
	 std::cerr << "[MandelbrotProxyClient] get : unknown port "<<pname<<" (returning NULL)\n";
	 return 0;
  }
}

void
MandelbrotProxyClient::set(const char* pname, void* value)
{
  if (strcmp(pname, "ucmbmg")==0) { 
	 std::cerr << "[MandelbrotProxyClient] ucmbmg set...\n";
	 CORBA::Object_ptr obj = static_cast<CORBA::Object_ptr>(value);
	 m_ucmbmg = cmandelbrot::MandelbrotManagement::_narrow(obj);
	 std::cerr << "[MandelbrotProxyClient] ucmbmg set...done\n";
  } else {
	 std::cerr << "[MandelbrotProxyClient] set : unknown port "<<pname<<std::endl;
  }
}

using namespace mandelbrot::impl;
LLCMCPP_CREATE(MandelbrotProxyClient);
