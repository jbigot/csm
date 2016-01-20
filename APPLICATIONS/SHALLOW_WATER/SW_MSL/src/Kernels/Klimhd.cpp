#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controllerPHBD.hpp>
#include <kernelh.h>
#include <mpi.h>

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

class Klimhd:virtual public Go
{
public:
  Datah* h;

  Klimhd(){}
  ~Klimhd(){}
  
  void go ()
  {/*
    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&(mpi_rank));

    Chronometer chrono;
    chrono.start();
   */
    //cout<<"====START Klimhd===="<<endl;
  	ControllerPHBD<double> ch(h);
    
    for(int64_t yy = ch.startHeight(); yy<ch.height();yy++)                                                                                                               
      {
	     for(int64_t xx = ch.startWidth();xx<ch.width(); xx++)
	     {
	     	ch(xx,yy) = ch(xx,yy-1);
	     }                                                                                                                                                                                           
      }/*                                                                                                                                                         
    chrono.stop();
    cout<<mpi_rank<<" compute Klimhd : "<<chrono.dureeCalcule()<<endl;
       */                   
  }

};

#include <Kernels/Klimhd.l2c>
