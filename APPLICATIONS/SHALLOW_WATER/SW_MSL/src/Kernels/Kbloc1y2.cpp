#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controller.hpp>
#include <kernelh.h>
#include <math.h>
#include <mpi.h>

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)
#define grav 9.81
#define grav_dem 4.905

class Kbloc1y2:virtual public Go
{
public:
  Datah* h2r;
  Datah* delz2;
  Datah* h2d;
  
  Kbloc1y2(){}
  ~Kbloc1y2(){}
  
  void go ()
  {/*
    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&(mpi_rank));

    Chronometer chrono;
    chrono.start();
   */
    //cout<<"====START Kbloc1y2===="<<endl;
    Controller<double> ch2r(h2r);
    Controller<double> cdelz2(delz2);
    Controller<double> ch2d(h2d);

    for(int64_t yy = ch2r.start(); yy<ch2r.height();yy++)
    {
      for(int64_t xx = ch2r.start();xx<ch2r.width(); xx++)
      {
        ch2d(xx,yy)=max(0.,ch2r(xx,yy)-max(0.,-cdelz2(xx,yy)));
      }
    }              
    /*
    chrono.stop();
    cout<<mpi_rank<<" compute Kbloc1y2 : "<<chrono.dureeCalcule()<<endl;*/
  }

};

#include <Kernels/Kbloc1y2.l2c>
