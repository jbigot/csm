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

class Klimud:virtual public Go
{
public:
  Datah* u;

  Klimud(){}
  ~Klimud(){}
  
  void go ()
  {/*
    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&(mpi_rank));

    Chronometer chrono;
    chrono.start();
   */
    //cout<<"====START Klimud===="<<endl;
  	ControllerPHBD<double> cu(u);
    
    for(int64_t yy = cu.startHeight(); yy<cu.height();yy++)                                                                                                               
      {
	     for(int64_t xx = cu.startWidth();xx<cu.width(); xx++)
	     {
	     	cu(xx,yy) = -cu(xx,yy-1);
	     }                                                                                                                                                                                           
      }/*                                                                                                                                                         
    chrono.stop();
    cout<<mpi_rank<<" compute Klimud : "<<chrono.dureeCalcule()<<endl;
       */                   
  }

};

#include <Kernels/Klimud.l2c>
