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

class Khuvz2l:virtual public Go
{
public:
  Datah* h;
  Datah* u;
  Datah* v;
  Datah* z;
  Datah* z2l;

  Khuvz2l(){}
  ~Khuvz2l(){}
  
  void go ()
  {
    //cout<<"====START Khuvz2l===="<<endl;
    Controller<double> ch(h);
    Controller<double> cu(u);
    Controller<double> cv(v);
    Controller<double> cz(z);
    Controller<double> cz2l(z2l);

    double dh1,dh2,dz1,dz2;

    //other lines
    int64_t yy2l = cz2l.start();
    for(int64_t yy = ch.start()+1; yy<ch.height()-1;yy++,yy2l++)
    {
      int64_t xx2l = cz2l.start();
      for(int64_t xx = ch.start();xx<ch.width(); xx++,xx2l++)
      {
        dh1 = ch(xx,yy)-ch(xx,yy+1);
        dz1 = cz(xx,yy)-cz(xx,yy+1);
        dh2 = ch(xx,yy-1)-ch(xx,yy);
        dz2 = cz(xx,yy-1)-cz(xx,yy);
        cz2l(xx2l,yy2l)=cz(xx,yy)+0.5*(Minmod(dz1+dh1,dz2+dh2)-Minmod(dh1,dh2));
      }
    }

    //last line
    int64_t y = ch.height()-1;
    int64_t yy2 = cz2l.height()-1;
    int64_t xx2 = cz2l.start();
    for(int64_t xx = ch.start();xx<ch.width(); xx++,xx2++)
    {
      dh1 = ch(xx,y+1)-ch(xx,y+2);
      dz1 = cz(xx,y+1)-cz(xx,y+2);
      dh2 = ch(xx,y)-ch(xx,y+1);
      dz2 = cz(xx,y)-cz(xx,y+1);
      cz2l(xx2,yy2)=cz(xx,y+1)+0.5*(Minmod(dz1+dh1,dz2+dh2)-Minmod(dh1,dh2));
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

#include <Kernels/Khuvz2l.l2c>
