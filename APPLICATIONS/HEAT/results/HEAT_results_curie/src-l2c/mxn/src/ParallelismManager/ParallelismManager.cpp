#include <cstring>
#include <iostream>

#include <llcmcpp/llcmcpp.h>

#include "ParallelismManagement.h"

namespace mxn
{

namespace impl
{

using namespace std;

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

class llcm_ParallelismManager : virtual public llcmcpp::Component, virtual public mxn::ParallelismManagement {
private:
  unsigned _rank;
  unsigned _size;

public:
  llcm_ParallelismManager() : _rank(0), _size(1) {} // Sequential by default
  ~llcm_ParallelismManager() {}

  unsigned getRank() { return _rank; }
  unsigned getSize() { return _size; }

  void barrier() {}

  void* get(const char* pname) {
	 if (strcmp(pname, "rank")==0) {
		return (void*) _rank;
	 } else if (strcmp(pname, "size")==0) {
		return (void*) _size;
	 } else if (strcmp(pname, "pm")==0) {
		return static_cast<mxn::ParallelismManagement*>(this);
	 } else {
		cerr << "[PM] get : unknown port "<<pname<<" (returning NULL)\n";
		return 0;
	 }
  }

  void set(const char* pname, void* value) {
	 if (strcmp(pname, "rank")==0) { 
		_rank = (unsigned long)(value);
	 } else if (strcmp(pname, "size")==0) { 
		_size = (unsigned long)(value);		
	 } else {
		cerr << "[PM] set : unknown port "<<pname<<endl;
	 }
  }

};

LLCMCPP_CREATE(llcm_ParallelismManager)

}

}
