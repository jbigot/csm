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

class Kbloc1x5:virtual public Go
{
public:
  Datah* u1l;
  Datah* u1r;
  Datah* v1l;
  Datah* v1r;
  Datah* h1g;
  Datah* h1d;
  Datah* f3;

  Kbloc1x5(){}
  ~Kbloc1x5(){}
  
  void go ()
  {/*
    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&(mpi_rank));

    Chronometer chrono;
    chrono.start();
   */
    //cout<<"====START Kbloc1x5===="<<endl;
    Controller<double> cu1l(u1l);
    Controller<double> cu1r(u1r);
    Controller<double> cv1l(v1l);
    Controller<double> cv1r(v1r);
    Controller<double> ch1g(h1g);
    Controller<double> ch1d(h1d);
    Controller<double> cf3(f3);

    for(int64_t yy = cu1l.start(); yy<cu1l.height();yy++)
    {
      for(int64_t xx = cu1l.start();xx<cu1l.width(); xx++)
      {
        if (ch1g(xx,yy)<=0. && ch1d(xx,yy)<=0.)
        {
          cf3(xx,yy)=0.;
        }
        else
        {
          double c = max(fabs(cu1l(xx,yy))+sqrt(grav*ch1g(xx,yy)),fabs(cu1r(xx,yy))+sqrt(grav*ch1d(xx,yy)));
          double cd = c*0.5;
          double qd = cu1r(xx,yy)*ch1d(xx,yy);
          double qg = cu1l(xx,yy)*ch1g(xx,yy);
          cf3(xx,yy)=(qg*cv1l(xx,yy)+qd*cv1r(xx,yy))*0.5-cd*(ch1d(xx,yy)*cv1r(xx,yy)-ch1g(xx,yy)*cv1l(xx,yy));
        }
      }
    }
    /*
    chrono.stop();
    cout<<mpi_rank<<" compute Kbloc1x5 : "<<chrono.dureeCalcule()<<endl;*/
  }

};

#include <Kernels/Kbloc1x5.l2c>
