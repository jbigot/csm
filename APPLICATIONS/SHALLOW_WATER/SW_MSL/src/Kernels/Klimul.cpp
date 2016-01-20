#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controllerPHBL.hpp>
#include <kernelh.h>
#include <mpi.h>

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

class Klimul:virtual public Go
{
public:
  Datah* u;

  Klimul(){}
  ~Klimul(){}
  
  void go ()
  {/*
    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&(mpi_rank));

    Chronometer chrono;
    chrono.start();
   */
    //cout<<"====START Klimul===="<<endl;
  	ControllerPHBL<double> cu(u);
    
    for(int64_t yy = cu.startHeight(); yy<cu.height();yy++)                                                                                                               
      {
	     for(int64_t xx = cu.startWidth();xx<cu.width(); xx++)
	     {
	     	cu(xx,yy) = -cu(xx+1,yy);
	     }                                                                                                                                                                                           
      }/*                                                                                                                                                         
    chrono.stop();
    cout<<mpi_rank<<" compute Klimul : "<<chrono.dureeCalcule()<<endl;
       */                  
  }

};

#include <Kernels/Klimul.l2c>
