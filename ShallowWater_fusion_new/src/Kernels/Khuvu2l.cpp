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

class Khuvu2l:virtual public Go
{
public:
  Datah* h;
  Datah* u;
  Datah* v;
  Datah* z;
  Datah* u2l;

  Khuvu2l(){}
  ~Khuvu2l(){}
  
  void go ()
  {
    //cout<<"====START Khuvu2l===="<<endl;
    Controller<double> ch(h);
    Controller<double> cu(u);
    Controller<double> cv(v);
    Controller<double> cz(z);
    Controller<double> cu2l(u2l);

    double dh1,dh2,du1,du2;
    double hl;

    //other lines
    int64_t yy2l = cu2l.start();
    //int64_t yy2r = cu2r.start()+1;
    for(int64_t yy = ch.start()+1; yy<ch.height()-1;yy++,yy2l++)//,yy2r++)
    {
      int64_t xx2l = cu2l.start();
      //int64_t xx2r = cu2r.start();
      for(int64_t xx = ch.start();xx<ch.width(); xx++,xx2l++)//,xx2r++)
      {
        dh1 = ch(xx,yy)-ch(xx,yy+1);
        du1 = cu(xx,yy)-cu(xx,yy+1);
        dh2 = ch(xx,yy-1)-ch(xx,yy);
        du2 = cu(xx,yy-1)-cu(xx,yy);
        hl = ch(xx,yy)-0.5*Minmod(dh1,dh2);
        cu2l(xx2l,yy2l)=cu(xx,yy)+0.5*Minmod(du1,du2)*hl/ch(xx,yy);
      }
    }

    //last line
    int64_t y = ch.height()-1;
    int64_t yy2 = cu2l.height()-1;
    int64_t xx2 = cu2l.start();
    for(int64_t xx = ch.start();xx<ch.width(); xx++,xx2++)
    {
      dh1 = ch(xx,y+1)-ch(xx,y+2);
      du1 = cu(xx,y+1)-cu(xx,y+2);
      dh2 = ch(xx,y)-ch(xx,y+1);
      du2 = cu(xx,y)-cu(xx,y+1);
      hl = ch(xx,y+1)-0.5*Minmod(dh1,dh2);
      cu2l(xx2,yy2)=cu(xx,y+1)+0.5*Minmod(du1,du2)*hl/ch(xx,y+1);
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

#include <Kernels/Khuvu2l.l2c>
