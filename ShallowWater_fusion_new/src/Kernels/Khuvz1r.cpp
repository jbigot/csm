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

class Khuvz1r:virtual public Go
{
public:
  Datah* h;
  Datah* u;
  Datah* v;
  Datah* z;
  Datah* z1r;

  Khuvz1r(){}
  ~Khuvz1r(){}
  
  void go ()
  {
    //cout<<"====START Khuvz1r===="<<endl;
    Controller<double> ch(h);
    Controller<double> cu(u);
    Controller<double> cv(v);
    Controller<double> cz(z);
    Controller<double> cz1r(z1r);

    double dh1,dh2,dz1,dz2;
    //double hl,ul,vl,zl,hr,ur,vr,zr;

    int64_t yy1 = cz1r.start();
    for(int64_t yy = ch.start(); yy<ch.height();yy++,yy1++)
    {
      int64_t x = ch.start();
      dh1 = ch(x-1,yy)-ch(x-2,yy);
      dz1 = cz(x-1,yy)-cz(x-2,yy);
      dh2 = ch(x,yy)-ch(x-1,yy);
      dz2 = cz(x,yy)-cz(x-1,yy);

      int64_t xx1 = cz1r.start();
      for(int64_t xx = ch.start()+1;xx<ch.width()-1; xx++,xx1++)
      {
        dh1 = dh2;
        dz1 = dz2;
        dh2 = ch(xx+1,yy)-ch(xx,yy);
        dz2 = cz(xx+1,yy)-cz(xx,yy);
        cz1r(xx1,yy1)=cz(xx,yy)-0.5*(Minmod(dz1+dh1,dz2+dh2)-Minmod(dh1,dh2));
      }
      x = ch.width()-1;
      xx1 = cz1r.width()-1;
      dh1 = dh2;
      dz1 = dz2;
      dh2=ch(x+2,yy)-ch(x+1,yy);
      dz2=ch(x+2,yy)-cz(x+1,yy);
      cz1r(xx1,yy1) = cz(x+1,yy)+0.5*(Minmod(dz1+dh1,dz2+dh2)-Minmod(dh1,dh2));
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

#include <Kernels/Khuvz1r.l2c>
