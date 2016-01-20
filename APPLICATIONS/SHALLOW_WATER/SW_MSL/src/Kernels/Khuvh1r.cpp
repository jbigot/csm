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

#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)

class Khuvh1r:virtual public Go
{
public:
  Datah* h;
  Datah* u;
  Datah* v;
  Datah* z;
  Datah* h1r;

  Khuvh1r(){}
  ~Khuvh1r(){}
  
  void go ()
  {/*
    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&(mpi_rank));

    Chronometer chrono;
    chrono.start();
   */
    //cout<<"====START Khuvh1r===="<<endl;
    Controller<double> ch(h);
    Controller<double> cu(u);
    Controller<double> cv(v);
    Controller<double> cz(z);
    Controller<double> ch1r(h1r);

    double dh1,dh2;
    double hl;

    int64_t yy1 = ch1r.start();
    for(int64_t yy = ch.start(); yy<ch.height();yy++,yy1++)
    {
      int64_t x = ch.start();
      dh1 = ch(x-1,yy)-ch(x-2,yy);
      dh2 = ch(x,yy)-ch(x-1,yy);
      hl = ch(x-1,yy)+0.5*Minmod(dh1,dh2);

      int64_t xx1 = ch1r.start();
      for(int64_t xx = ch.start()+1;xx<ch.width()-1; xx++,xx1++)
      {
        dh1 = dh2;
        dh2 = ch(xx+1,yy)-ch(xx,yy);
        hl = ch(xx,yy)-0.5*Minmod(dh1,dh2);
        ch1r(xx1,yy1)=hl;
      }
      x = ch.width()-1;
      xx1 = ch1r.width()-1;
      dh1 = dh2;
      dh2=ch(x+2,yy)-ch(x+1,yy);
      ch1r(xx1,yy1) = ch(x+1,yy)+0.5*Minmod(dh1,dh2);
    }/*
    chrono.stop();
    cout<<mpi_rank<<" compute Khuvh1r : "<<chrono.dureeCalcule()<<endl;*/
  }

private:
  double Minmod(double a, double b)
  {
    double rec;
    if(a>=0. && b>=0.)
      rec=min(a,b);
    else if(a<=0. && b<=0.)
      rec=max(a,b);
    else
      rec=0.;
    return rec;
  }

};

#include <Kernels/Khuvh1r.l2c>
