#include <cstring>
#include <cstdlib>
#include <iostream>
#include <vector>

#include <llcmcpp/lifecycle.h>
#include <llcmcpp/go.h>
#include <llcmcpp/llcmcpp.h>

#include "VectorAdd.h"
#include "VectorAdd_BC.h"

#include "ParallelismManagement.h"
#include "DistributionLibrary.h"
#include "BasicBC.h"

#undef DEBUG
#undef DEBUG2

namespace mxn
{

namespace impl
{

using namespace llcmcpp;
using namespace std;

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

class llcm_BasicBC_Vector_Redist_Server : virtual public llcmcpp::Component, virtual public mxn::VectorAddInternal, virtual public LifeCycle {
private:
  mxn::ParallelismManagement* _pm;
  //TODO replace by an interface
  mxn::impl::BasicBC* _bc;
  mxn::DistributionLibraryServer* _dls;
  mxn::VectorAdd* _va;

  unsigned _rank;
  unsigned _size;

  mxn::BasicBCDescr* _descr;

  mxn::Topology_t _sTopo;
  mxn::Topology_t _dTopo;

public:
  llcm_BasicBC_Vector_Redist_Server() : _pm(NULL),_bc(NULL), _dls(NULL), _va(NULL), _rank(0), _size(0), _descr(NULL) {
	 _sTopo.total=_dTopo.total=0;
  }
  ~llcm_BasicBC_Vector_Redist_Server() {}


  void sanity_check() {
#ifdef DEBUG2
	 cerr << "[BCvRS "<<_rank<<" "<<_size<<"] Sanity check ... "<<endl;
#endif

	 if (_pm==NULL) {
		cerr << "**ERROR: upm is not connected\n";
		abort();
	 }
	 if (_bc==NULL) {
		cerr << "**ERROR: ubc is not connected\n";
		abort();
	 }
	 if (_dls==NULL) {
		cerr << "**ERROR: udls is not connected\n";
		abort();
	 }
	 if (_va==NULL) {
		cerr << "**ERROR: uva is not connected\n";
		abort();
	 }

#ifdef DEBUG2
	 cerr << "[BCvRS "<<_rank<<" "<<_size<<"] Sanity check ... ok"<<endl;
#endif
  }

  void _init() {
	 
	 _rank = _pm->getRank();
	 _size = _pm->getSize();	 
	 _bc->setEltSize(sizeof(long));

	 _dls->setNodeRank(_rank);

	 _dls->setSourceTopology(_sTopo);

	 if (_dTopo.total==0) {
		_dTopo.total = _size;
	 }
	 _dls->setDestTopology(_dTopo);

#ifdef DEBUG
	 cerr << "[BCvRS "<<_rank<<" "<<_size<<"] init DLC topo with srcTopo="<<_sTopo.total<<" dstTopo="<<_dTopo.total <<endl;
#endif
  }

  void configureComplete() {
	 sanity_check();
	 _init();	 
  }

  unsigned getRank() {
	 return _rank;
  }

  void foo(const mxn::BasicBCDescr* d, long *ptr) {
#ifdef DEBUG
	 cerr << "[BCvRS "<<_rank<<" "<<_size<<"] foo ..."<<endl;
#endif

#ifdef DEBUG
	 cerr << "[BCvRS "<<_rank<<" "<<_size<<"] Setting descr ..."<<endl;
#endif
	 _dls->setDescr((void*)d);

#ifdef DEBUG
	 cerr << "[BCvRS "<<_rank<<" "<<_size<<"] Inserting data of #msg="<<d->ids.size()<<endl;
#endif
	 long* p=ptr;
	 bool ok;
	 for (unsigned i=0; i < d->ids.size(); i++)	{
		unsigned long len = (d->ids)[i].len;
		ok =  _dls->insertData((void *) p, len);
	 }

	 unsigned long glen=_bc->getTotalNbElt();
	 if (ok) {
#ifdef DEBUG
		cerr << "[BCvRS "<<_rank<<" "<<_size<<"] All data here -- invoking va"<<endl;
#endif
		unsigned long len;
		long* data = (long*) _dls->getServerData(len);
		_va->foo(glen, data);
		_dls->serverFree();
	 }
#ifdef DEBUG
	 else {
		cerr << "[BCvRS "<<_rank<<" "<<_size<<"] Some data lacking"<<endl;
	 }
#endif
	 
#ifdef DEBUG
	 cerr << "[BCvRS "<<_rank<<" "<<_size<<"] foo ... done"<<endl;
#endif
  }

  void* get(const char* pname) {
	 if (strcmp(pname, "lifecycle")==0) {
		return static_cast<LifeCycle*>(this);
	 } else if (strcmp(pname, "vai")==0) {
		return static_cast<mxn::VectorAddInternal*>(this);
	 } else {
		cerr << "[BCvRS "<<_rank<<" "<<_size<<"] get : unknown port "<<pname<<" (returning NULL)\n";
		return 0;
	 }
  }

  void set(const char* pname, void* value) {
	 if (strcmp(pname, "srcTopo")==0) { 
		_sTopo.total=(unsigned long) value;
	 } else if (strcmp(pname, "dstTopo")==0) { 
		_dTopo.total=(unsigned long) value;
	 } else if (strcmp(pname, "upm")==0) { 
		_pm = static_cast<mxn::ParallelismManagement*>(value);
	 } else if (strcmp(pname, "ubc")==0) { 
		 //TODO !!! this is not an interface !!!
		_bc = static_cast<mxn::impl::BasicBC*>(value);
	 } else if (strcmp(pname, "udls")==0) { 
		_dls = static_cast<mxn::DistributionLibraryServer*>(value);
	 } else if (strcmp(pname, "uva")==0) { 
		_va=static_cast<mxn::VectorAdd*>(value);
	 } else {
		cerr << "[BCvRS "<<_rank<<" "<<_size<<"] set : unknown port "<<pname<<endl;
	 }
  }

};

LLCMCPP_CREATE(llcm_BasicBC_Vector_Redist_Server)

}

}
