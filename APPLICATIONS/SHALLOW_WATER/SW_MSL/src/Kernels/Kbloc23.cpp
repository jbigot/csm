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

#define grav_dem 4.905

class Kbloc23:virtual public Go
{
public:
  Datah* u;
  Datah* v;
  Datah* q2;
  Datah* h2g;
  Datah* h2d;
  Datah* h2l;
  Datah* h2r;
  Datah* delzc2;
  Datah* f3;
  Datah* g3;
  Datah* hs;
  Datah* qs2;

  Kbloc23(){}
  ~Kbloc23(){}
  
  void go ()
  {/*
    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&(mpi_rank));

    Chronometer chrono;
    chrono.start();
   */
    //cout<<"====START Kbloc23===="<<endl;
    Controller<double> cu(u);
    Controller<double> cv(v);
    Controller<double> cq2(q2);
    Controller<double> ch2g(h2g);
    Controller<double> ch2d(h2d);
    Controller<double> ch2l(h2l);
    Controller<double> ch2r(h2r);
    Controller<double> cdelzc2(delzc2);
    Controller<double> cf3(f3);
    Controller<double> cg3(g3);
    Controller<double> chs(hs);
    Controller<double> cqs2(qs2);

    double tx=0.005;
    double ty=0.005;
    double dt=0.5;

    int64_t yyq=cq2.start();
    int64_t yy1=cf3.start();
    int64_t yy2=ch2r.start();
    for(int64_t yy = chs.start(); yy<chs.height();yy++,yyq++,yy1++,yy2++)
    {
      int64_t xxq=cq2.start();
      int64_t xx1=cf3.start();
      int64_t xx2=ch2r.start();
      for(int64_t xx = chs.start();xx<chs.width(); xx++,xxq++,xx1++,xx2++)
      {
        cqs2(xxq,yyq)= cq2(xxq,yyq)-tx*(cf3(xx1+1,yy1)-cf3(xx1,yy1))-ty*(cg3(xx2,yy2)-cg3(xx2,yy2+1)+grav_dem*(ch2d(xx2,yy2+1)
          *ch2d(xx2,yy2+1)-ch2r(xx2,yy2+1)*ch2r(xx2,yy2+1)+ch2l(xx2,yy2)*ch2l(xx2,yy2)-ch2g(xx2,yy2)*ch2g(xx2,yy2)+(ch2r(xx2,yy2+1)
          +ch2l(xx2,yy2))*cdelzc2(xxq,yyq)));
        //cqs2(xxq,yyq)= cqs2(xxq,yyq)/(1.+cmcf(xxq,yyq)*sqrt(cu(xx,yy)*cu(xx,yy)+cv(xx,yy)*cv(xx,yy))*dt/(8.*chs(xx,yy)));
      }
    }  
    /*
    chrono.stop();
    cout<<mpi_rank<<" compute Kbloc23 : "<<chrono.dureeCalcule()<<endl;*/
                           
  }

};

#include <Kernels/Kbloc23.l2c>
