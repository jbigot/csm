#include <cstdlib>
#include <cstring>
#include <iostream>

#include <llcmcpp/lifecycle.h>
#include <llcmcpp/llcmcpp.h>

#include "VectorAdd.h"

#include "ParallelismManagement.h"
#include "BasicBC.h"

#undef DEBUG

namespace mxn
{

namespace impl
{

using namespace llcmcpp;
using namespace std;

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

class llcm_BasicBC_Vector_Server : virtual public llcmcpp::Component, virtual public mxn::VectorAdd, virtual public LifeCycle {
private:
  mxn::ParallelismManagement* _pm;
  //TODO replace by an interface
  mxn::impl::BasicBC* _bc;

  unsigned long _rank;
  unsigned long _size;

  bool _dump;  // flag to control display

public:
  llcm_BasicBC_Vector_Server() : _pm(NULL),_bc(NULL), _rank(0), _size(0), _dump(false)  {}
  ~llcm_BasicBC_Vector_Server() {}


  void sanity_check() {
#ifdef DEBUG
	 cerr << "[BCvS "<<_rank<<" "<<_size<<"] Sanity check ... "<<endl;
#endif
	 if (_pm==NULL) {
		cerr << "**ERROR: upm is not connected\n";
		abort();
	 }
	 if (_bc==NULL) {
		cerr << "**ERROR: ubc is not connected\n";
		abort();
	 }
#ifdef DEBUG
	 cerr << "[BCvS "<<_rank<<" "<<_size<<"] Sanity check ... ok"<<endl;
#endif
  }

  void init() {
#ifdef DEBUG
	 cerr << "[BCvS "<<_rank<<" "<<_size<<"] Init ..."<<endl;
#endif
	 _rank = _pm->getRank();
	 _size = _pm->getSize();
#ifdef DEBUG
	 cerr << "[BCvS "<<_rank<<" "<<_size<<"] Init ... done"<<endl;
#endif
  }

  void configureComplete() {
	 sanity_check();
	 init();
  }

  void foo(unsigned long gsz, long* ptr) {
#ifdef DEBUG
	 cerr << "[BCvS "<<_rank<<" "<<_size<<"] foo ... "<<endl;
#endif
		
	 if (_dump) {
		unsigned long lsz=_bc->getLocalNbElt();
		unsigned long first = _bc->getFirst();
		unsigned long last = _bc->getLast();
		//		unsigned long step = _bc->getStep(); BUGGY
		cerr << "[BCvS "<<_rank<<" "<<_size<<"] foo ... total_size="<<gsz<<" local_size="<<lsz<<" data["<<first<<":"<<last<<"]=[";
		for(unsigned i=0; i<lsz;i++)
		  cerr << " " << ptr[i];
		cerr << " ]\n";
	 }

#ifdef DEBUG
	 cerr << "[BCvS "<<_rank<<" "<<_size<<"] foo ... done\n";
#endif
  }

  void* get(const char* pname) {
	 if (strcmp(pname, "lifecycle")==0) {
		return static_cast<LifeCycle*>(this);
	 } else if (strcmp(pname, "va")==0) {
		return static_cast<mxn::VectorAdd*>(this);
	 } else {
		cerr << "[BCvS "<<_rank<<" "<<_size<<"] get : unknown port "<<pname<<" (returning NULL)\n";
		return 0;
	 }
  }

  void set(const char* pname, void* value) {
	 if (strcmp(pname, "DUMP")==0) { 
		_dump = (unsigned long) value;
	 } else if (strcmp(pname, "upm")==0) { 
		_pm = static_cast<mxn::ParallelismManagement*>(value);
	 } else if (strcmp(pname, "ubc")==0) { 
		 //TODO !!! this is not an interface !!!
		_bc = static_cast<mxn::impl::BasicBC*>(value);
	 } else {
		cerr << "[BCvS "<<_rank<<" "<<_size<<"] set : unknown port "<<pname<<endl;
	 }
  }

};

LLCMCPP_CREATE(llcm_BasicBC_Vector_Server)

}

}
