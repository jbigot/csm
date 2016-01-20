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

class Kbloc1x2:virtual public Go
{
public:
  Datah* h1r;
  Datah* delz1;
  Datah* h1d;

  Kbloc1x2(){}
  ~Kbloc1x2(){}
  
  void go ()
  {
    /*   int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&(mpi_rank));

    Chronometer chrono;
    chrono.start();*/

    //cout<<"====START Kbloc1x2===="<<endl;
    Controller<double> ch1r(h1r);
    Controller<double> cdelz1(delz1);
    Controller<double> ch1d(h1d);

    for(int64_t yy = ch1r.start(); yy<ch1r.height();yy++)
    {
      for(int64_t xx = ch1r.start();xx<ch1r.width(); xx++)
      {
        ch1d(xx,yy)=max(0.,ch1r(xx,yy)-max(0.,-cdelz1(xx,yy)));
      }
    }              

    /*    chrono.stop();
	  cout<<mpi_rank<<" compute Kbloc1x2 : "<<chrono.dureeCalcule()<<endl;*/
  }

};

#include <Kernels/Kbloc1x2.l2c>
