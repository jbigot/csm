#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controller.hpp>
#include <kernelh.h>
#include <mpi.h>

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

class Kdelzc1f:virtual public Go
{
public:
  Datah* z1l;
  Datah* z1r;
  Datah* delzc1;

  Kdelzc1f(){}
  ~Kdelzc1f(){}
  
  void go ()
  {/*
    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&(mpi_rank));

    Chronometer chrono;
    chrono.start();
   */
    //cout<<"====START Kdelzc1f===="<<endl;
    Controller<double> cz1l(z1l);
    Controller<double> cz1r(z1r);
    Controller<double> cdelzc1(delzc1);

    for(int64_t yy = cdelzc1.start(); yy<cdelzc1.height();yy++)
       for(int64_t xx = cdelzc1.start();xx<cdelzc1.width(); xx++)
          cdelzc1(xx,yy) = cz1r(xx+1,yy) - cz1l(xx,yy);                                                                                                             /*
    chrono.stop();
    cout<<mpi_rank<<" compute Kdelzc1f : "<<chrono.dureeCalcule()<<endl;*/
  }

};

#include <Kernels/Kdelzc1f.l2c>
