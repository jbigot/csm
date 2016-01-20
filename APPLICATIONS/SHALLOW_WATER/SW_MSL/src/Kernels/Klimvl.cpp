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

class Klimvl:virtual public Go
{
public:
  Datah* v;

  Klimvl(){}
  ~Klimvl(){}
  
  void go ()
  {/*
    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&(mpi_rank));

    Chronometer chrono;
    chrono.start();
   */
    //cout<<"====START Klimvl===="<<endl;
  	ControllerPHBL<double> cv(v);
    
    for(int64_t yy = cv.startHeight(); yy<cv.height();yy++)                                                                                                               
      {
	     for(int64_t xx = cv.startWidth();xx<cv.width(); xx++)
	     {
	     	cv(xx,yy) = cv(xx+1,yy);
	     }                                                                                                                                                                                           
      }/*                                                                                                                                                         
    chrono.stop();
    cout<<mpi_rank<<" compute Klimvl : "<<chrono.dureeCalcule()<<endl;
       */                  
  }

};

#include <Kernels/Klimvl.l2c>
