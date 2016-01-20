#include <iostream>

#include "mandelbrotproxyserver.h"

using namespace std;
using namespace mandelbrot::impl;

MandelbrotProxyServer::MandelbrotProxyServer()
{
#ifndef NDEBUG
#endif // NDEBUG
  cerr << "[MandelbrotProxyServer] constructor...!\n";
  m_pcmbmg = static_cast<cmandelbrot::MandelbrotManagement_ptr>(this->_this());
  m_pombmg = CORBA::Object::_narrow(m_pcmbmg);
  char* NO[2] = { 0, 0 };
  NO[0] = strdup("a.out");
  int ac=1;
  _orb = CORBA::ORB_init (ac, NO);
  cerr << "[MandelbrotProxyServer] constructor...done!\n";
}

void
MandelbrotProxyServer::dump()
{
  m_umbmg->dump();
}

void
MandelbrotProxyServer::get_info(cmandelbrot::dataULong_t_out data, cmandelbrot::area_t_out area, ::CORBA::ULong& cnx, ::CORBA::ULong& cny)
{
  cerr << "[MandelbrotProxyServer] get info...\n";

  unsigned int* ptr;
  complex<double> r0, r1;
  cerr << "[MandelbrotProxyServer] get info...through m_umbmg...\n";
  // on linux x86, ULong is defined as "unsigned long" while uint32_t is defined as "unsigned" (both are 32 bits long but distinct)
  uint32_t nx=cnx, ny=cny;
  m_umbmg->get_info(ptr, r0, r1, nx, ny);
  cnx=nx; cny=ny;
  cerr << "[MandelbrotProxyServer] get info returns: r0="<<r0<<" r1="<<r1<<" nx="<<cnx<<" ny="<<cny<<endl;

  area = new cmandelbrot::area_t(4);

  area->length(4);
  area[static_cast<CORBA::ULong>(0)]=r0.real();
  area[static_cast<CORBA::ULong>(1)]=r0.imag();
  area[static_cast<CORBA::ULong>(2)]=r1.real();
  area[static_cast<CORBA::ULong>(3)]=r1.imag();

  data = new cmandelbrot::dataULong_t(cnx*cny);
  data->length(cnx*cny);
  cerr << "[MandelbrotProxyServer] get_info : copying\n";
  for(unsigned int j=0; j<cny; j++)
	 for(unsigned int i=0; i<cnx; i++)
		data[j*cnx+i]=ptr[j*cnx+i];
  cerr << "[MandelbrotProxyServer] get info will returns: nx="<<cnx<<" ny="<<cny<<endl;
  cerr << "[MandelbrotProxyServer] get info...done\n";
}
 
void
MandelbrotProxyServer::compute()
{
  m_umbmg->compute();
}

void*
MandelbrotProxyServer::get(const char* pname) {
  if (strcmp(pname, "pcmbmg")==0) {
	 return m_pombmg;
  } else {
	 std::cerr << "[MandelbrotProxyServer] get : unknown port "<<pname<<" (returning NULL)\n";
	 return 0;
  }
}

void
MandelbrotProxyServer::set(const char* pname, void* value)
{
  if (strcmp(pname, "umbmg")==0) { 
	 m_umbmg = static_cast<mandelbrot::MandelbrotManagement*>(value);
	 std::cerr << "[MandelbrotProxyServer] umbmg set\n";
  } else {
	 std::cerr << "[MandelbrotProxyServer] set : unknown port "<<pname<<std::endl;
  }
}

using namespace mandelbrot::impl;
LLCMCPP_CREATE(MandelbrotProxyServer);
