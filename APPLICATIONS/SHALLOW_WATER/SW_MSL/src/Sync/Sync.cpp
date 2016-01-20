#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>
#include <vector>
#include <datah.h>
#include <synch.h>
#include "chronometer.hpp"

#include "libskelgis/skelgis.hpp"
#include "../rdtsc.h"

typedef llcmcpp::Go Go;

using ::std::vector;
using ::std::cout;
using ::std::endl;

class Sync:virtual public Go//,virtual public Synch
{
public:
  vector<Datah *> data;

  //vector<skelgis::DMatrix_impl<double,2,false> * > dmatrices;

  Sync (){}
  ~Sync(){}

  /* void init()
  {
    for(int i=0;i<data.size();i++)
      {
	skelgis::DMatrix_impl<double,2,false> * dmatrix;
        data[i]->getDMatrix(dmatrix);
	dmatrices.push_back(dmatrix);
      }
      }*/

  void go ()
  {
    //cout<<"====START Sync===="<<endl;
    //cout<<"updates "<<n<<endl;

    //unsigned long long a,b;
    //a = rdtsc();

    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&(mpi_rank));

    Chronometer chrono;
    chrono.start();
    for(int i=0;i<data.size();i++)
      {
	//dmatrices[i]->getBorders();
	data[i]->update();
      }
    chrono.stop();
    cout<<mpi_rank<<" - Time SYNC : "<<chrono.dureeCalcule()<<endl;

    //b = rdtsc();
    //std::cout<<"Sync rdtsc = "<<b-a<<std::endl;
  }

  void dataCompute(Datah * port)
  {
    data.push_back(port);
  }
  
};

#include <Sync/Sync.l2c>
