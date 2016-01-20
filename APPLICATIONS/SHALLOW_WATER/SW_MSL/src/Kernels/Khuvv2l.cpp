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

class Khuvv2l:virtual public Go
{
public:
  Datah* h;
  Datah* u;
  Datah* v;
  Datah* z;
  Datah* v2l;

  Khuvv2l(){}
  ~Khuvv2l(){}
  
  void go ()
  {/*
    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&(mpi_rank));

    Chronometer chrono;
    chrono.start();
   */
    //cout<<"====START Khuvv2l===="<<endl;
    Controller<double> ch(h);
    Controller<double> cu(u);
    Controller<double> cv(v);
    Controller<double> cz(z);
    Controller<double> cv2l(v2l);

    double dh1,dh2,dv1,dv2;
    double hl;

    //other lines
    int64_t yy2l = cv2l.start();
    for(int64_t yy = ch.start()+1; yy<ch.height()-1;yy++,yy2l++)
    {
      int64_t xx2l = cv2l.start();
      for(int64_t xx = ch.start();xx<ch.width(); xx++,xx2l++)
      {
        dh1 = ch(xx,yy)-ch(xx,yy+1);
        dv1 = cv(xx,yy)-cv(xx,yy+1);
        dh2 = ch(xx,yy-1)-ch(xx,yy);
        dv2 = cv(xx,yy-1)-cv(xx,yy);
        hl = ch(xx,yy)-0.5*Minmod(dh1,dh2);
        cv2l(xx2l,yy2l)=cv(xx,yy)+0.5*Minmod(dv1,dv2)*hl/ch(xx,yy);
      }
    }

    //last line
    int64_t y = ch.height()-1;
    int64_t yy2 = cv2l.height()-1;
    int64_t xx2 = cv2l.start();
    for(int64_t xx = ch.start();xx<ch.width(); xx++,xx2++)
    {
      dh1 = ch(xx,y+1)-ch(xx,y+2);
      dv1 = cv(xx,y+1)-cv(xx,y+2);
      dh2 = ch(xx,y)-ch(xx,y+1);
      dv2 = cv(xx,y)-cv(xx,y+1);
      hl = ch(xx,y+1)-0.5*Minmod(dh1,dh2);
      cv2l(xx2,yy2)=cv(xx,y+1)+0.5*Minmod(dv1,dv2)*hl/ch(xx,y+1);
    }/*
    chrono.stop();
    cout<<mpi_rank<<" compute Khuvv2l : "<<chrono.dureeCalcule()<<endl;*/
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

#include <Kernels/Khuvv2l.l2c>
