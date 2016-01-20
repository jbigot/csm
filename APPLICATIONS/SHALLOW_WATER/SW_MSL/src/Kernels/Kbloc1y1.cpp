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

class Kbloc1y1:virtual public Go
{
public:
  Datah* h2l;
  // Datah* h2r;
  // Datah* u2l;
  // Datah* u2r;
  // Datah* v2l;
  // Datah* v2r;
  Datah* delz2;
  Datah* h2g;
  // Datah* h2d;
  // Datah* g1;
  // Datah* g2;
  // Datah* g3;

  Kbloc1y1(){}
  ~Kbloc1y1(){}
  
  void go ()
  {/*
    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&(mpi_rank));

    Chronometer chrono;
    chrono.start();
   */
    //cout<<"====START Kbloc1y1===="<<endl;
    Controller<double> ch2l(h2l);
    // Controller<double> ch2r(h2r);
    // Controller<double> cu2l(u2l);
    // Controller<double> cu2r(u2r);
    // Controller<double> cv2l(v2l);
    // Controller<double> cv2r(v2r);
    Controller<double> cdelz2(delz2);
    Controller<double> ch2g(h2g);
    // Controller<double> ch2d(h2d);
    // Controller<double> cg1(g1);
    // Controller<double> cg2(g2);
    // Controller<double> cg3(g3); 

    for(int64_t yy = ch2l.start(); yy<ch2l.height();yy++)
    {
      for(int64_t xx = ch2l.start();xx<ch2l.width(); xx++)
      {
        ch2g(xx,yy)=max(0.,ch2l(xx,yy)-max(0.,cdelz2(xx,yy)));
        // ch2d(xx,yy)=max(0.,ch2r(xx,yy)-max(0.,-cdelz2(xx,yy)));
        // if (ch2g(xx,yy)<=0. && ch2d(xx,yy)<=0.)
        // {
        //   cg1(xx,yy)=0.;
        //   cg2(xx,yy)=0.;
        //   cg3(xx,yy)=0.;
        // }
        // else
        // {
        //   double c = max(fabs(cu2l(xx,yy))+sqrt(grav*ch2g(xx,yy)),fabs(cu2r(xx,yy))+sqrt(grav*ch2d(xx,yy)));
        //   double cd = c*0.5;
        //   double qd = cu2r(xx,yy)*ch2d(xx,yy);
        //   double qg = cu2l(xx,yy)*ch2g(xx,yy);
        //   cg1(xx,yy)=(qg+qd)*0.5-cd*(ch2d(xx,yy)-ch2g(xx,yy));
        //   cg2(xx,yy)=((cu2l(xx,yy)*qg)+(grav_dem*ch2g(xx,yy)*ch2g(xx,yy))+(cu2r(xx,yy)*qd)+(grav_dem*ch2d(xx,yy)*ch2d(xx,yy)))*0.5-cd*(qd-qg);
        //   cg3(xx,yy)=(qg*cv2l(xx,yy)+qd*cv2r(xx,yy))*0.5-cd*(ch2d(xx,yy)*cv2r(xx,yy)-ch2g(xx,yy)*cv2l(xx,yy));
        // }
      }
    }              
    /*
    chrono.stop();
    cout<<mpi_rank<<" compute Kbloc1y1 : "<<chrono.dureeCalcule()<<endl;*/
  }

};

#include <Kernels/Kbloc1y1.l2c>
