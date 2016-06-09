#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controller.hpp>
#include <kernelh.h>

#include <omp.h>
#define CHUNK 5

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)

class Khuvu1l_omp:virtual public Go
{
public:
  Datah* h;
  Datah* u;
  Datah* v;
  Datah* z;
  Datah* u1l;

  Khuvu1l_omp(){}
  ~Khuvu1l_omp(){}
  
  void go ()
  {/*
    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&(mpi_rank));

    Chronometer chrono;
    chrono.start();
   */
    //cout<<"====START Khuvu1l===="<<endl;
    Controller<double> ch(h);
    Controller<double> cu(u);
    Controller<double> cv(v);
    Controller<double> cz(z);
    Controller<double> cu1l(u1l);

    double dh1,dh2,du1,du2;
    double hl,ur;

    int64_t yy1 = cu1l.start();

//dynamic also possible
#pragma omp parallel for shared(ch,cu,cv,cz,cu1l) private(yy,xx,yy1,x,xx1,dh1,dh2,du1,du2,hr,ur) schedule (static, CHUNK)
    for(int64_t yy = ch.start(); yy<ch.height();yy++)
    {
      int64_t x = ch.start();
      dh1 = ch(x-1,yy)-ch(x-2,yy);
      du1 = cu(x-1,yy)-cu(x-2,yy);
      dh2 = ch(x,yy)-ch(x-1,yy);
      du2 = cu(x,yy)-cu(x-1,yy);
      hl = ch(x-1,yy)+0.5*Minmod(dh1,dh2);
      ur = cu(x-1,yy)+0.5*Minmod(du1,du2)*hl/ch(x-1,yy);

      int64_t xx1 = cu1l.start();
      for(int64_t xx = ch.start()+1;xx<ch.width()-1; xx++)
      {
        cu1l(xx1,yy1)=ur;
        dh1 = dh2;
        du1 = du2;
        dh2 = ch(xx+1,yy)-ch(xx,yy);
        du2 = cu(xx+1,yy)-cu(xx,yy);
        hl = ch(xx,yy)-0.5*Minmod(dh1,dh2);
        ur = cu(xx,yy)+0.5*Minmod(du1,du2)*hl/ch(xx,yy);

        xx1++;
      }
      xx1 = cu1l.width()-1;
      cu1l(xx1,yy1)=ur;

      yy1++;
    }
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

#include <KernelsOmp/Khuvu1l_omp.l2c>
