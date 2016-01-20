#include <cstdlib>
#include <cstring>
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

class llcm_BasicBC_Vector_Redist_Client : virtual public llcmcpp::Component, virtual public mxn::VectorAdd, virtual public LifeCycle {
private:
  mxn::ParallelismManagement* _pm;
  //TODO replace by an interface
  mxn::impl::BasicBC* _bc;
  mxn::DistributionLibraryClient* _dlc;
  typedef vector<mxn::VectorAddInternal*> vais_ns_t; // not sorted
  typedef vector<mxn::VectorAddInternal*> vais_t; // ordered by rank
  vais_t _vais_ns; // original connection
  vais_t _vais;    // sorted during _init

  unsigned _rank;
  unsigned _size;

  long* _data;

  mxn::Topology_t _srcTopo;
  mxn::Topology_t _dstTopo;

public:
  llcm_BasicBC_Vector_Redist_Client() :  _pm(NULL),_bc(NULL), _dlc(NULL),  _rank(0), _size(0), _data(NULL) {
	 _srcTopo.total=_dstTopo.total=0;
  }

  ~llcm_BasicBC_Vector_Redist_Client() {}


  void sanity_check() {
#ifdef DEBUG2
	 cerr << "[BCvRC "<<_rank<<" "<<_size<<"] Sanity check ... "<<endl;
#endif
	 if (_pm==NULL) {
		cerr << "**ERROR: upm is not connected\n";
		abort();
	 }
	 if (_bc==NULL) {
		cerr << "**ERROR: ubc is not connected\n";
		abort();
	 }
	 if (_dlc==NULL) {
		cerr << "**ERROR: udlc is not connected\n";
		abort();
	 }
	 if (_vais_ns.size()==0) {
		cerr << "**ERROR: uvai is not connected\n";
		abort();
	 }
#ifdef DEBUG2
	 cerr << "[BCvRC "<<_rank<<" "<<_size<<"] Sanity check ... ok"<<endl;
#endif
  }

  void _init() {
	 _rank = _pm->getRank();
	 _size = _pm->getSize();

	 _dlc->setNodeRank(_rank);

	 if (_srcTopo.total==0)
		_srcTopo.total = _size;	
	 if  (_dstTopo.total==0)
		_dstTopo.total = _vais_ns.size();

#ifdef DEBUG
	 cerr << "[BCvRC "<<_rank<<" "<<_size<<"] init DLC topo with srcTopo="<<_srcTopo.total<<" dstTopo="<<_dstTopo.total <<endl;
#endif
	 _dlc->setSourceTopology(_srcTopo);
	 _dlc->setDestTopology(_dstTopo);

	 // Sorting vais_ns
	 _vais.resize(_vais_ns.size());
	 for(vais_ns_t::const_iterator it=_vais_ns.begin(); it!=_vais_ns.end(); it++) {
		mxn::VectorAddInternal* vai = *it;
		unsigned rank = vai->getRank();
		_vais[rank]=vai;
	 }
  }

  void configureComplete() {
	 sanity_check();
	 _init();
  }

  void foo(unsigned long gsz, long *vec) {
#ifdef DEBUG
	 cerr << "[BCvRC "<<_rank<<" "<<_size<<"] foo ..."<<endl;
#endif

#ifdef DEBUG
	 cerr << "[BCvRC "<<_rank<<" "<<_size<<"] compute pieces to send ..."<<endl;
#endif
	 unsigned pclen;
	 mxn::PieceToSend* pcs = _dlc->computePiecesToSend(pclen);

#ifdef DEBUG
	 cerr << "[BCvRC "<<_rank<<" "<<_size<<"] Dumping pieces to send ..."<<endl;
	 for(unsigned i=0; i<pclen; i++) {
		mxn::PieceToSend& p=pcs[i];
		cerr << "[BCvRC "<<_rank<<" "<<_size<<"] i="<<i<<" src="<<p.sourceNode<<" dst="<<p.destNode<<" bytes="<<p.size<<endl;
	 }
#endif

#ifdef DEBUG
	 cerr << "[BCvRC "<<_rank<<" "<<_size<<"] sending pieces..."<<endl;
#endif
	 _dlc->setDataPtr(vec);
	 for(unsigned i=0; i<pclen; i++) {
		mxn::PieceToSend& p=pcs[i];
		if (_rank == p.sourceNode) {
		  unsigned dst=p.destNode;
		  unsigned long size=p.size;
		  unsigned long rsize=p.size;
		  bool end;


		  long* ptr = (long*) _dlc->getClientData(p.id, dst, rsize, size, end);
		  mxn::BasicBCDescr* descr = (mxn::BasicBCDescr*) _dlc->getDescr();
#ifdef DEBUG
		  cerr << "[BCvRC "<<_rank<<" "<<_size<<"] actual send to dst "<<dst<<"..."<<endl;
#endif
		  _vais[dst]->foo(descr, ptr);
#ifdef DEBUG
		  cerr << "[BCvRC "<<_rank<<" "<<_size<<"] actual send to dst "<<dst<<"...done"<<endl;
#endif
		}
	 }

#ifdef DEBUG
	 cerr << "[BCvRC "<<_rank<<" "<<_size<<"] foo ... done"<<endl;
#endif
  }

  void* get(const char* pname) {
	 if (strcmp(pname, "lifecycle")==0) {
		return static_cast<LifeCycle*>(this);
	 } else if (strcmp(pname, "va")==0) {
		return static_cast<mxn::VectorAdd*>(this);
	 } else {
		cerr << "[BCvRC "<<_rank<<" "<<_size<<"] get : unknown port "<<pname<<" (returning NULL)\n";
		return 0;
	 }
  }

  void set(const char* pname, void* value) {
	 if (strcmp(pname, "srcTopo")==0) { 
		_srcTopo.total=(unsigned long) value;
	 } else if (strcmp(pname, "dstTopo")==0) { 
		_dstTopo.total=(unsigned long) value;
	 } else if (strcmp(pname, "upm")==0) { 
		_pm = static_cast<mxn::ParallelismManagement*>(value);
	 } else if (strcmp(pname, "ubc")==0) { 
		 //TODO !!! this is not an interface !!!
		_bc = static_cast<mxn::impl::BasicBC*>(value);
	 } else if (strcmp(pname, "udlc")==0) { 
		_dlc = static_cast<mxn::DistributionLibraryClient*>(value);
	 } else if (strcmp(pname, "uvai")==0) { 
		_vais_ns.push_back(static_cast<mxn::VectorAddInternal*>(value));
	 } else {
		cerr << "[BCvRC "<<_rank<<" "<<_size<<"] set : unknown port "<<pname<<endl;
	 }
  }

};

LLCMCPP_CREATE(llcm_BasicBC_Vector_Redist_Client)

}

}
