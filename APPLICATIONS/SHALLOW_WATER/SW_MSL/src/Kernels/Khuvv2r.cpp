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

class Khuvv2r:virtual public Go
{
public:
  Datah* h;
  Datah* u;
  Datah* v;
  Datah* z;
  Datah* v2r;

  Khuvv2r(){}
  ~Khuvv2r(){}
  
  void go ()
  {/*
    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&(mpi_rank));

    Chronometer chrono;
    chrono.start();
   */
    //cout<<"====START Khuvv2r===="<<endl;
    Controller<double> ch(h);
    Controller<double> cu(u);
    Controller<double> cv(v);
    Controller<double> cz(z);
    Controller<double> cv2r(v2r);

    double dh1,dh2,dv1,dv2;
    double hl,hr;

    //first line
    int64_t y = ch.start();
    int64_t yy2 = cv2r.start();
    int64_t xx2 = cv2r.start();
    for(int64_t xx = ch.start();xx<ch.width(); xx++,xx2++)
    {
      dh1 = ch(xx,y-1)-ch(xx,y);
      dv1 = cv(xx,y-1)-cv(xx,y);
      dh2 = ch(xx,y-2)-ch(xx,y-1);
      dv2 = cv(xx,y-2)-cv(xx,y-1);
      hl = ch(xx,y-1)-0.5*Minmod(dh1,dh2);
      cv2r(xx2,yy2)=cv(xx,y-1)-0.5*Minmod(dv1,dv2)*hl/ch(xx,y-1);
    }

    //other lines
    int64_t yy2r = cv2r.start()+1;
    for(int64_t yy = ch.start()+1; yy<ch.height()-1;yy++,yy2r++)
    {
      int64_t xx2r = cv2r.start();
      for(int64_t xx = ch.start();xx<ch.width(); xx++,xx2r++)
      {
        dh1 = ch(xx,yy)-ch(xx,yy+1);
        dv1 = cv(xx,yy)-cv(xx,yy+1);
        dh2 = ch(xx,yy-1)-ch(xx,yy);
        dv2 = cv(xx,yy-1)-cv(xx,yy);
        hr = ch(xx,yy)+0.5*Minmod(dh1,dh2);
        cv2r(xx2r,yy2r)=cv(xx,yy)-0.5*Minmod(dv1,dv2)*hr/ch(xx,yy);
      }
    }/*
    chrono.stop();
    cout<<mpi_rank<<" compute Khuvv2r : "<<chrono.dureeCalcule()<<endl;*/
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

#include <Kernels/Khuvv2r.l2c>
