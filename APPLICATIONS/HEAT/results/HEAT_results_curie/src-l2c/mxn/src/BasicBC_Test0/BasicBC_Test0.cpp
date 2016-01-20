#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

#include <llcmcpp/go.h>
#include <llcmcpp/llcmcpp.h>

#include "basicbcconfig.h"

#undef LIBGRAPH

#ifdef LIBGRAPH
#include "kbps.h"
#endif

namespace mxn
{

namespace impl
{

using namespace llcmcpp;
using namespace std;

class llcm_BasicBC_Test0 : public llcmcpp::Component, public Go {

  // 2 uses ports
  vector<BasicClientConfig*> _bcc;
  vector<BasicBCConfig*> _bcs;

  // 1 provides ports (go)


  // Internal data
  vector<long*> sdata;
  vector<long*> ddata;

  vector<void*> vdescr;

  vector<PieceToSend*> spiece;
  vector<unsigned>     ssize;

public:
  llcm_BasicBC_Test0() {}
  ~llcm_BasicBC_Test0() {}

  void* get(const char* pname) {
	 if (strcmp(pname, "go")==0) {
		return static_cast<Go*>(this);
	 } else {
		cerr << "[BasicBC_Test] get : unknown port "<<pname<<" (returning NULL)\n";
		return 0;
	 }
  }

  void set(const char* pname, void* value) {
	 if (strcmp(pname, "bcc")==0) { 
		_bcc.push_back(static_cast<BasicClientConfig*>(value));
	 } else if (strcmp(pname, "bcs")==0) { 
		_bcs.push_back(static_cast<BasicBCConfig*>(value));
	 }
  }

  void go() {

	 Topology_t stopo ;
	 stopo.total = _bcc.size();
	 Topology_t dtopo ;
	 dtopo.total = _bcs.size();
	 unsigned long glen = 10;
	 unsigned long bz = 0;
  
	 cerr << "stopo: " << stopo.total << endl;
	 cerr << "dtopo: " << dtopo.total << endl;


	 // ???
	 sdata.resize(stopo.total);
	 ddata.resize(dtopo.total);
	 vdescr.resize(stopo.total*dtopo.total);
	 spiece.resize(stopo.total);
	 ssize.resize(stopo.total);

	 cerr << " glen: " << glen << endl;

	 // Allocating & initializing comm matrix
	 unsigned ** comm_matrix = new unsigned *[stopo.total];
	 unsigned* comm_area = (unsigned*)calloc(stopo.total*dtopo.total, sizeof(unsigned)); // ZEROED !
	 if (comm_area == NULL) {
		cerr << "Can not allocate memory of size " << stopo.total*dtopo.total*sizeof(unsigned) << endl;
		abort();
	 }

	 for(unsigned i=0; i<stopo.total; i++)
		{
		  comm_matrix[i] = &comm_area[i*dtopo.total];
		}
  
	 // Allocating & initializing info matrix : 1 => number of argument
	 void *** info_matrix = new void **[stopo.total];
	 void ** info_area = (void**) calloc(stopo.total*dtopo.total*1, sizeof(void*)); // zeroed
	 if (info_area == NULL) {
		cerr << "Can not allocate memory of size " << stopo.total*dtopo.total*sizeof(unsigned) << endl;
		abort();
	 }
	 for(unsigned i=0; i<stopo.total; i++)
		{
		  info_matrix[i] = &info_area[i*dtopo.total*1];
		}
	 
	 cerr<<endl<<"##############################"<<endl;
  
	 for(unsigned i=0; i<stopo.total; i++) {
		
		cerr<<endl<<"###### Sender "<<i<<endl<<endl;
		
		_bcc[i]->setSourceTopology(stopo);
		_bcc[i]->setDestTopology(dtopo);
		_bcc[i]->setNodeRank(i);
		_bcc[i]->setEltSize(sizeof(long));
		_bcc[i]->setTotalNbElt(glen);
		_bcc[i]->setBlocSize(bz);

		long* ptr = sdata[i] = new long[glen];
		_bcc[i]->setDataPtr((void*) ptr);
		for(unsigned j=0;j<glen;j++)
		  ptr[j] = i*1000+j;

		spiece[i]=_bcc[i]->computePiecesToSend(ssize[i]);

		for ( unsigned j = 0; j <  ssize[i]; j++) 
		  {
			 PieceToSend& p = spiece[i][j];
			 long dest = p.destNode;
			 comm_matrix[i][dest]    += p.size;
			 info_matrix[i][dest+0]   = p.id;
			 cerr << "p.id: " << p.id<<endl;
		  }
	 }
	 cerr<<endl<<"##############################"<<endl;

	 for(unsigned long i=0; i<stopo.total; i++) {
		cerr<<endl<<"###### Sender "<<i<<endl;
		if (ssize[i] > dtopo.total) {
		  cerr << "*** case not yet supported (cf descr)\n";
		  abort();
		}
		for(unsigned long j=0; j<ssize[i]; j++) {
		  cerr<<"   Message "<<j;
		  fprintf(stderr," , %d -> %2d : %ld octets (%p)\n",
					 spiece[i][j].sourceNode, spiece[i][j].destNode, spiece[i][j].size, spiece[i][j].id);
		  bool b=false;
		  unsigned long msz= spiece[i][j].size;
		  while(!b) {
			 unsigned long mlen;
			 void* ptr = _bcc[i]->getClientData(spiece[i][j].id, i, msz, mlen, b);
			 fprintf(stderr,"   #element: %lu ptr:%p -- ", mlen, ptr);
			 for(unsigned long k=0; k<mlen; k++)
				fprintf(stderr," %ld", ((long*)ptr)[k]);
			 fprintf(stderr,"\n");
		  }
		  if (msz) {
			 cerr << "BIIIGGGG TROUBLE: msz is not 0: " <<msz<<endl;
			 exit(-1);
		  }
		  vdescr[i*dtopo.total+j] = _bcc[i]->getDescr();
		  cerr<<endl;
		  //cerr << "    descr: "<<_bcc[i]->getDescr()<<endl;
		}
	 }  
	 
	 cerr<<endl<<"##############################"<<endl;

#ifdef LIBGRAPH
	 //build our graph
	 bigraph g;

	 //add nodes
	 for(unsigned int i = 0 ; i < stopo.total ; i++)
		g.add_node(true);
	 for(unsigned int i = 0 ; i < dtopo.total ; i++)
		g.add_node(false);
	 
	 // 2. fill the bigraph
	 for (unsigned i = 0; i < (unsigned) stopo.total; i++)
		{
		  for (unsigned j = 0; j < dtopo.total; j++)
			 {
				edge*e = g.add_edge(g.get_node(i, true),
										  g.get_node(j, false),
										  comm_matrix[i][j],
										  info_matrix[i][j]);
				cerr << "id: " << info_matrix[i][j]<<" "<<e->getDecoration()<<endl;
			 }
		}
	 g.remove_empty_nodes();  
	 g.set_beta(10);
	 g.set_k(100);
	 
	 kbps_approximation*  _schedule_info = new kbps_approximation(&g); 
	 _schedule_info->compute_poly();
	 _schedule_info->serialize();
	 
	 _schedule_info->serial_display();

	 // Display result
	 unsigned int phases = _schedule_info->infophases[0];
	 unsigned int k=0;
	 for(unsigned int i = 0; i < phases; i++) 
		{
		  for(unsigned int j = 0 ; j < _schedule_info->infophases[i+1] ; j++)
			 {
				kbps_approximation::_comm_t& s= _schedule_info->comms[k];
				fprintf(stderr," %d -> %d : %d %p\n", s.src, s.dst, s.weight, s.info);
				k++;
			 }
		}

#endif

	 // Init receiver
	 for(unsigned i=0; i<dtopo.total; i++) {

		cerr<<endl<<"##############################"<<endl;
		cerr<<"###### Receiver "<<i<<endl<<endl;

		_bcs[i]->setSourceTopology(stopo);
		_bcs[i]->setDestTopology(dtopo);
		_bcs[i]->setNodeRank(i);
		_bcs[i]->setEltSize(sizeof(long));
		_bcs[i]->setBlocSize(bz);

		// Receive data
    
	 }

	 // Freeing memory

	 for(unsigned i=0; i<spiece.size(); i++)
		delete[] spiece[i];

	 for(unsigned i=0;i<sdata.size();i++)
		delete[] sdata[i];
	 free(comm_area);
	 free(info_area);
	 delete[] comm_matrix;
	 delete[] info_matrix;
  }
};

/////////////////////////

LLCMCPP_CREATE(llcm_BasicBC_Test0)

}

}
