#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>

#include <time.h>
#include <sys/time.h>
#include <llcmcpp/lifecycle.h>
#include <llcmcpp/go.h>
#include <llcmcpp/llcmcpp.h>

#include "ParallelismManagement.h"
#include "BasicBC.h"
#include "VectorAdd.h"

typedef struct timeval my_time_t;
#define get_time(a) gettimeofday(&a, NULL)
#define timing_diff_usec(a,b) (((long long)(b).tv_sec - (a).tv_sec)*1000000+((long long)(b).tv_usec - (a).tv_usec))

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

class llcm_BasicBC_Vector_Client : virtual public llcmcpp::Component, virtual public Go, virtual public LifeCycle {
private:
  mxn::ParallelismManagement* _pm;
  //TODO replace by an interface
  mxn::impl::BasicBC* _bc;
  mxn::VectorAdd* _va;

  char* _Log;

  unsigned long _rank;
  unsigned long _size;

  unsigned long _NbIter;

  unsigned long _gsz;// total size
  unsigned long _bsz;// bloc size 0: bloc

  unsigned long _lsz;// local size

  long* _data;

  bool _dump; 

  std::ostream* result_out;
  std::ofstream file_out;

public:
  llcm_BasicBC_Vector_Client() : _pm(NULL),_bc(NULL), _va(NULL), _Log(NULL), _NbIter(10), _gsz(10), _bsz(0), _data(NULL), _dump(false) {
	 _Log=strdup("/tmp/results.txt");
  }
  ~llcm_BasicBC_Vector_Client() {}


  void sanity_check() {
	 //	 cerr << "[BCvC "<<_rank<<" "<<_size<<"] Sanity check ... "<<endl;
	 if (_pm==NULL) {
		cerr << "**ERROR: upm is not connected\n";
		abort();
	 }
	 if (_bc==NULL) {
		cerr << "**ERROR: ubc is not connected\n";
		abort();
	 }
	 if (_va==NULL) {
		cerr << "**ERROR: uva is not connected\n";
		abort();
	 }
	 //	 cerr << "[BCvC "<<_rank<<" "<<_size<<"] Sanity check ... ok"<<endl;
  }

  void init() {
	 _rank = _pm->getRank();
	 _size = _pm->getSize();	

	 _bc->setEltSize(sizeof(long));
	 _bc->setTotalNbElt(_gsz);
	 _bc->setBlocSize(_bsz);
	 _lsz = _bc->getLocalNbElt();
  }

  void configureComplete() {
	 sanity_check();
	 init();
#ifdef DEBUG
	 cerr << "[BCvC "<<_rank<<" "<<_size<<"] configureComplete done\n";
#endif
  }

  void test(const unsigned llen, const unsigned nbiter)
  {
	 unsigned N  = llen*_size;
	 _bc->setTotalNbElt(N);
	 _bc->setBlocSize(llen);
	 _lsz = _bc->getLocalNbElt(); // _lN in the other test

#ifdef DEBUG
	 cerr << "[BCvC-"<<_rank<<"] llen="<<llen<<" lN=" << _lsz << endl;
	 assert(llen == _lsz);
#endif
	 unsigned fN=_bc->getFirst();
	
	 _data = (long*) malloc(sizeof(long)*_lsz);
	 if (_data==NULL) {
		cerr << "[BCvC "<<_rank<<" "<<_size<<"] ** Error when allocating the vector\n";
		abort();
	 }

	 for ( unsigned i = 0; i < _lsz; i++ ) {
		_data[i] = _rank*1000+fN+i;		
	 }

	 if (_dump) {
		unsigned long first = _bc->getFirst();
		unsigned long last = _bc->getLast();
		cerr << "[BCvC "<<_rank<<" "<<_size<<"] total_size="<<N<<" local_size="<<_lsz<<" data["<<first<<":"<<last<<"]=[";
		for(unsigned i=0; i<_lsz;i++)
		  cerr << " " << _data[i];
		cerr << " ]\n";
	 }

	 my_time_t tick_start, tick_end;

	 _pm->barrier();

	 for ( unsigned i = 0; i < 10; i++ ) {
		_va->foo (N, _data );
		_pm->barrier();
	 }

	 _pm->barrier();

	 get_time ( tick_start );
	 for ( unsigned i = 0; i < nbiter; i++ ) {
		_va->foo (N, _data);
		_pm->barrier();
	 }
	 get_time ( tick_end );

	 if ( _rank == 0 ) {
		long long time_usec = timing_diff_usec ( tick_start, tick_end ) / nbiter;
		(*result_out) << llen*_size*sizeof (long ) << " " << time_usec << endl;
		result_out->flush();
	 }

	 free(_data);
	 _data=NULL;
  }

  void barrier_latency()
  {
	 my_time_t tick_start, tick_end;

	 for ( unsigned i = 0; i < 10; i++ ) _pm->barrier();

	 get_time ( tick_start );
	 for ( unsigned i = 0; i < 1000; i++ ) _pm->barrier();
	 get_time ( tick_end );

	 if ( _rank == 0 ) {
		long long time_usec = timing_diff_usec ( tick_start, tick_end );
		(*result_out) << "#Barrier latency (ns)" << endl;
		(*result_out) << time_usec << endl;
		result_out->flush();
	 }
  }

  void go() {
#ifdef DEBUG
	 cerr << "[BCvC "<<_rank<<" "<<_size<<"] go ... "<<endl;
#endif

	if ( _rank == 0 ) {
		file_out.open ( _Log );
		if ( !file_out ) {
			cerr << "impossible to open output file of name " << _Log << endl;
			result_out = &cout;
		} else {
			cerr << "[client] results go to " << _Log << endl;
			result_out = &file_out;
		}
	}

#ifdef DEBUG
	 cerr << "[BCvC "<<_rank<<" "<<_size<<"] start ... "<<endl;
#endif

	barrier_latency();

#ifdef DEBUG
	 cerr << "[BCvC "<<_rank<<" "<<_size<<"] test ... "<<endl;
#endif

	if ( _rank == 0 ) {
		(*result_out) << "# Test nbiter=" << _NbIter << " len=" << _gsz << " schedule=direct - no cache" << endl;
		result_out->flush();
	}

	for ( unsigned l = 1; l <= _gsz; l *= 2 ) test ( l, _NbIter );

	if ( _rank == 0 ) {
		(*result_out) << "# EOF" << endl;
		result_out->flush();
	}

#ifdef DEBUG
	 cerr << "[BCvC "<<_rank<<" "<<_size<<"] all done... "<<endl;
#endif
  }

  void* get(const char* pname) {
	 if (strcmp(pname, "go")==0) {
		return static_cast<Go*>(this);
	 } else if (strcmp(pname, "lifecycle")==0) {
		return static_cast<LifeCycle*>(this);
	 } else {
		cerr << "[BCvC "<<_rank<<" "<<_size<<"] get : unknown port "<<pname<<" (returning NULL)\n";
		return 0;
	 }
  }

  void set(const char* pname, void* value) {
	 if (strcmp(pname, "DUMP")==0) { 
		_dump = (unsigned long) value;
	 } else if (strcmp(pname, "N")==0) { 
		_gsz = (unsigned long) (value);
	 } else if (strcmp(pname, "NBITER")==0) { 
		_NbIter = (unsigned long) (value);
	 } else if (strcmp(pname, "OUTPUT")==0) { 
		free(_Log);
		_Log = strdup(((char*) value));
	 } else if (strcmp(pname, "BSZ")==0) { 
		_bsz = (unsigned long) (value);
	 } else if (strcmp(pname, "upm")==0) { 
		_pm = static_cast<mxn::ParallelismManagement*>(value);
	 } else if (strcmp(pname, "ubc")==0) { 
		 //TODO !!! this is not an interface !!!
		_bc = static_cast<mxn::impl::BasicBC*>(value);
	 } else if (strcmp(pname, "uva")==0) { 
		_va = static_cast<mxn::VectorAdd*>(value);		
	 } else {
		cerr << "[BCvC "<<_rank<<" "<<_size<<"] set : unknown port "<<pname<<endl;
	 }
  }

};

LLCMCPP_CREATE(llcm_BasicBC_Vector_Client)

}

}
