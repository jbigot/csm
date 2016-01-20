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

class Khuvz1l:virtual public Go
{
public:
  Datah* h;
  Datah* u;
  Datah* v;
  Datah* z;
  Datah* z1l;

  Khuvz1l(){}
  ~Khuvz1l(){}
  
  void go ()
  {/*
    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&(mpi_rank));

    Chronometer chrono;
    chrono.start();
   */
    //cout<<"====START Khuvz1l===="<<endl;
    Controller<double> ch(h);
    Controller<double> cu(u);
    Controller<double> cv(v);
    Controller<double> cz(z);
    Controller<double> cz1l(z1l);

    double dh1,dh2,dz1,dz2;
    double zr;

    int64_t yy1 = cz1l.start();
    for(int64_t yy = ch.start(); yy<ch.height();yy++,yy1++)
    {
      int64_t x = ch.start();
      dh1 = ch(x-1,yy)-ch(x-2,yy);
      dz1 = cz(x-1,yy)-cz(x-2,yy);
      dh2 = ch(x,yy)-ch(x-1,yy);
      dz2 = cz(x,yy)-cz(x-1,yy);
      zr = cz(x-1,yy)+0.5*(Minmod(dz1+dh1,dz2+dh2)-Minmod(dh1,dh2));

      int64_t xx1 = cz1l.start();
      for(int64_t xx = ch.start()+1;xx<ch.width()-1; xx++,xx1++)
      {
        cz1l(xx1,yy1)=zr;
        dh1 = dh2;
        dz1 = dz2;
        dh2 = ch(xx+1,yy)-ch(xx,yy);
        dz2 = cz(xx+1,yy)-cz(xx,yy);
        zr=cz(xx,yy)+0.5*(Minmod(dz1+dh1,dz2+dh2)-Minmod(dh1,dh2));
      }
      xx1 = cz1l.width()-1;
      cz1l(xx1,yy1)=zr;
    }/*
    chrono.stop();
    cout<<mpi_rank<<" compute Khuvz1l : "<<chrono.dureeCalcule()<<endl;*/
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

#include <Kernels/Khuvz1l.l2c>
