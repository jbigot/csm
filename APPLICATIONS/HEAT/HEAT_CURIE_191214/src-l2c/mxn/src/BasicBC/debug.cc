#include "BasicBC.h"
#include <vector>
#include <iostream>
#include <stdio.h>

#define LIBGRAPH

#ifdef LIBGRAPH
#include "kbps.h"
#endif

using namespace std;

vector<BasicBC> sd;
vector<BasicBC> dd;

vector<long*> sdata;
vector<long*> ddata;

vector<void*> vdescr;

vector<PieceToSend*> spiece;
vector<unsigned>     ssize;

void emuleSrc(int id) {
}

int main(int argc, char *argv[]) {

  PaCO::PacoTopology_t stopo;
  PaCO::PacoTopology_t dtopo;
  unsigned long glen = 10;
  unsigned long bz = 0;
  
  switch(argc) {
  case 5:
    sscanf(argv[4],"%lu",&bz);
  case 4:
    sscanf(argv[3],"%lu",&glen);
  case 3:	
  case 2:
    sscanf(argv[1],"%lu",&stopo.total);
    sscanf(argv[2],"%lu",&dtopo.total);
    break;
  default:
    cerr << "Usage: " << argv[0] << " sd.total dd.total [glen [sz=0 bloc, else bloccyclic(sz)]]\n";
    exit(-1);
  }

  cerr << "stopo: " << stopo.total << endl;
  cerr << "dtopo: " << dtopo.total << endl;

  // Allocating memory
  sd.resize(stopo.total);
  dd.resize(dtopo.total);
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

    sd[i].setSourceTopology(stopo);
    sd[i].setDestTopology(dtopo);
    sd[i].setNodeRank(i);
    sd[i].setEltSize(sizeof(long));
    sd[i].setTotalNbElt(glen);
    sd[i].setBlocSize(bz);

    long* ptr = sdata[i] = (long*) malloc(glen);
    sd[i].setDataPtr((void*) ptr);
    for(unsigned j=0;j<glen;j++)
      ptr[j] = i*0x1000+j;

    spiece[i]=sd[i].computePiecesToSend(ssize[i]);

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

  for(unsigned i=0; i<stopo.total; i++) {
    cerr<<endl<<"###### Sender "<<i<<endl;
    if (ssize[i] > dtopo.total) {
      cerr << "*** case not yet supported (cf descr)\n";
      abort();
    }
    for(unsigned j=0; j<ssize[i]; j++) {
      cerr<<"   Message "<<j;
      fprintf(stderr," , %2d -> %2d : %4d octets (%p)\n",
	      spiece[i][j].sourceNode, spiece[i][j].destNode, spiece[i][j].size, spiece[i][j].id);
      bool b=false;
      long msz= spiece[i][j].size;
      while(!b) {
	long mlen;
	void* ptr = sd[i].getClientData(spiece[i][j].id, i, msz, mlen, b);
	fprintf(stderr,"   #element: %2u ptr:%p -- ", mlen, ptr);
	for(int k=0; k<mlen; k++)
	  fprintf(stderr," %d", ((int*)ptr)[k]);
	fprintf(stderr,"\n");
      }
      if (msz) {
	cerr << "BIIIGGGG TROUBLE: msz is not 0: " <<msz<<endl;
	exit(-1);
      }
      vdescr[i*dtopo.total+j] = sd[i].getDescr();
      cerr<<endl;
      //cerr << "    descr: "<<sd[i].getDescr()<<endl;
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

    dd[i].setSourceTopology(stopo);
    dd[i].setDestTopology(dtopo);
    dd[i].setNodeRank(i);
    dd[i].setEltSize(sizeof(long));
    dd[i].setBlocSize(bz);

    // Receive data
    
  }



}
