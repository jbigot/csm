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

class Khuvh2r:virtual public Go
{
public:
  Datah* h;
  Datah* u;
  Datah* v;
  Datah* z;
  Datah* h2r;

  Khuvh2r(){}
  ~Khuvh2r(){}
  
  void go ()
  {/*
    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&(mpi_rank));

    Chronometer chrono;
    chrono.start();
   */
    //cout<<"====START Khuvh2r===="<<endl;
    Controller<double> ch(h);
    Controller<double> cu(u);
    Controller<double> cv(v);
    Controller<double> cz(z);
    Controller<double> ch2r(h2r);

    double dh1,dh2;
    double hl;

    //first line
    int64_t y = ch.start();
    int64_t yy2 = ch2r.start();
    int64_t xx2 = ch2r.start();
    for(int64_t xx = ch.start();xx<ch.width(); xx++,xx2++)
    {
      dh1 = ch(xx,y-1)-ch(xx,y);
      dh2 = ch(xx,y-2)-ch(xx,y-1);
      hl = ch(xx,y-1)-0.5*Minmod(dh1,dh2);
      ch2r(xx2,yy2)=hl;
    }

    //other lines
    int64_t yy2r = ch2r.start()+1;
    for(int64_t yy = ch.start()+1; yy<ch.height()-1;yy++,yy2r++)
    {
      int64_t xx2r = ch2r.start();
      for(int64_t xx = ch.start();xx<ch.width(); xx++,xx2r++)
      {
        dh1 = ch(xx,yy)-ch(xx,yy+1);
        dh2 = ch(xx,yy-1)-ch(xx,yy);
        hl = ch(xx,yy)-0.5*Minmod(dh1,dh2);
        ch2r(xx2r,yy2r)=hl;
      }
    }/*
    chrono.stop();
    cout<<mpi_rank<<" compute Khuvh2r : "<<chrono.dureeCalcule()<<endl;
     */
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

#include <Kernels/Khuvh2r.l2c>
