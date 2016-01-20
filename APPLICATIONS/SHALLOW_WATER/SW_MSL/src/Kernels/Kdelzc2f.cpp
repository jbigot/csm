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

class Kdelzc2f:virtual public Go
{
public:
  Datah* z2r;
  Datah* z2l;
  Datah* delzc2;

  Kdelzc2f(){}
  ~Kdelzc2f(){}
  
  void go ()
  {/*
    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&(mpi_rank));

    Chronometer chrono;
    chrono.start();
   */
    //cout<<"====START Kdelzc2f===="<<endl;
    Controller<double> cz2l(z2l);
    Controller<double> cz2r(z2r);
    Controller<double> cdelzc2(delzc2);

    for(int64_t yy = cdelzc2.start(); yy<cdelzc2.height();yy++)
       for(int64_t xx = cdelzc2.start();xx<cdelzc2.width(); xx++)
          cdelzc2(xx,yy) = cz2r(xx,yy) - cz2l(xx,yy+1);/*
    chrono.stop();
    cout<<mpi_rank<<" compute Kdelzc2f : "<<chrono.dureeCalcule()<<endl;*/
  }

};

#include <Kernels/Kdelzc2f.l2c>
