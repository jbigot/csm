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

class Kbloc1y3:virtual public Go
{
public:
  Datah* u2l;
  Datah* u2r;
  Datah* h2g;
  Datah* h2d;
  Datah* g1;

  Kbloc1y3(){}
  ~Kbloc1y3(){}
  
  void go ()
  {/*
    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&(mpi_rank));

    Chronometer chrono;
    chrono.start();
   */
    //cout<<"====START Kbloc1y3===="<<endl;
    Controller<double> cu2l(u2l);
    Controller<double> cu2r(u2r);
    Controller<double> ch2g(h2g);
    Controller<double> ch2d(h2d);
    Controller<double> cg1(g1);

    for(int64_t yy = cu2l.start(); yy<cu2l.height();yy++)
    {
      for(int64_t xx = cu2l.start();xx<cu2l.width(); xx++)
      {
        if (ch2g(xx,yy)<=0. && ch2d(xx,yy)<=0.)
        {
          cg1(xx,yy)=0.;
        }
        else
        {
          double c = max(fabs(cu2l(xx,yy))+sqrt(grav*ch2g(xx,yy)),fabs(cu2r(xx,yy))+sqrt(grav*ch2d(xx,yy)));
          double cd = c*0.5;
          double qd = cu2r(xx,yy)*ch2d(xx,yy);
          double qg = cu2l(xx,yy)*ch2g(xx,yy);
          cg1(xx,yy)=(qg+qd)*0.5-cd*(ch2d(xx,yy)-ch2g(xx,yy));
        }
      }
    }              
    /*
    chrono.stop();
    cout<<mpi_rank<<" compute Kbloc1y3 : "<<chrono.dureeCalcule()<<endl;*/
  }

};

#include <Kernels/Kbloc1y3.l2c>
