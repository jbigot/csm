#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

#include <llcmcpp/lifecycle.h>
#include <llcmcpp/go.h>
#include <llcmcpp/llcmcpp.h>

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

// Component that dump :>

class llcm_Dumper :  virtual public llcmcpp::Component, virtual public LifeCycle,
							virtual public mandelbrot::Array2DDump
{
protected:

  bool _dump; 
  std::string _dumpfile;

public:
  llcm_Dumper() : _dump(true) {}
  ~llcm_Dumper() {}

  void configureComplete() {
#ifdef DEBUG
	 std::cerr << "[Dumper] configureComplete done\n";
#endif
  }

  virtual void dump(const unsigned* ptr, unsigned nx, unsigned ny, unsigned nbcolor, const char* fname) {

	 if (!_dump)
		return;

#ifdef DEBUG
	 std::cerr << "[Dumper] dumping ... "<<std::endl;
#endif

	 ostream* os;
	 std::ofstream of;

	 if (fname!=NULL) {
		of.open(fname, std::ios::out | std::ios::trunc);
		os = &of;
	 } else {		  
		os = &std::cout;
	 }

	 // HEADER of PGM FILE
	 *os << "P2\n"<<nx<<" "<<ny<<"\n"<<nbcolor<<"\n";

	 for ( int j=ny-1; j>=0; j--) {
		for(unsigned i=0; i<nx; i++) {
			 *os << ptr[(j*nx)+i] << " ";
		}
		*os << std::endl;
	 }

	 // closing
	 if (fname!=NULL)
		of.close();
  }

  virtual void dump(const unsigned* ptr, unsigned nx, unsigned ny, unsigned nbcolor) {
	 this->dump(ptr, nx, ny, nbcolor, _dumpfile.c_str());
  }


  virtual void* get(const char* pname) {
	 if (strcmp(pname, "pa2dd")==0) {
		return static_cast<mandelbrot::Array2DDump*>(this);
	 } else if (strcmp(pname, "cc")==0) {
		return static_cast<LifeCycle*>(this);
	 } else {
		std::cerr << "[Dumper] get : unknown port "<<pname<<" (returning NULL)\n";
		return 0;
	 }
  }

  virtual void set(const char* pname, void* value) {
	 if (strcmp(pname, "DUMP")==0) { 
		_dump = (unsigned long) value;
		std::cerr << "[Dumper] DUMP set to "<<_dump<<std::endl;
	 } else if (strcmp(pname, "DUMPFILE")==0) { 
		_dumpfile = * (std::string*) value;
		std::cerr << "[Dumper] DUMPFILE set to "<<_dumpfile<<std::endl;
	 } else {
		std::cerr << "[Dumper] set : unknown port "<<pname<<std::endl;
	 }
  }

};

}

}

using mandelbrot::impl::llcm_Dumper;

LLCMCPP_CREATE(llcm_Dumper)
