#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controller.hpp>
#include <kernelh.h>

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)

class Khuvv1l:virtual public Go
{
public:
  Datah* h;
  Datah* u;
  Datah* v;
  Datah* z;
  Datah* v1l;

  Khuvv1l(){}
  ~Khuvv1l(){}
  
  void go ()
  {
    //cout<<"====START Khuvv1l===="<<endl;
    Controller<double> ch(h);
    Controller<double> cu(u);
    Controller<double> cv(v);
    Controller<double> cz(z);
    Controller<double> cv1l(v1l);

    double dh1,dh2,dv1,dv2;
    double hl,vr;

    int64_t yy1 = cv1l.start();
    for(int64_t yy = ch.start(); yy<ch.height();yy++,yy1++)
    {
      int64_t x = ch.start();
      dh1 = ch(x-1,yy)-ch(x-2,yy);
      dv1 = cv(x-1,yy)-cv(x-2,yy);
      dh2 = ch(x,yy)-ch(x-1,yy);
      dv2 = cv(x,yy)-cv(x-1,yy);
      hl = ch(x-1,yy)+0.5*Minmod(dh1,dh2);
      vr = cv(x-1,yy)+0.5*Minmod(dv1,dv2)*hl/ch(x-1,yy);

      int64_t xx1 = cv1l.start();
      for(int64_t xx = ch.start()+1;xx<ch.width()-1; xx++,xx1++)
      {
        cv1l(xx1,yy1)=vr;
        dh1 = dh2;
        dv1 = dv2;
        dh2 = ch(xx+1,yy)-ch(xx,yy);
        dv2 = cv(xx+1,yy)-cv(xx,yy);
        hl = ch(xx,yy)-0.5*Minmod(dh1,dh2);
        vr=cv(xx,yy)+0.5*Minmod(dv1,dv2)*hl/ch(xx,yy);
      }
      xx1 = cv1l.width()-1;
      cv1l(xx1,yy1)=vr;
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

#include <Kernels/Khuvv1l.l2c>
