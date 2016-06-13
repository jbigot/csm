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

class Khuvu1r_omp:virtual public Go
{
public:
  Datah* h;
  Datah* u;
  Datah* v;
  Datah* z;
  Datah* u1r;

  Khuvu1r_omp(){}
  ~Khuvu1r_omp(){}
  
  void go ()
  {
    //cout<<"====START Khuvu1r===="<<endl;
    Controller<double> ch(h);
    Controller<double> cu(u);
    Controller<double> cv(v);
    Controller<double> cz(z);
    Controller<double> cu1r(u1r);

    double dh1,dh2,du1,du2;
    double hl,hr;

    int64_t yy1 = cu1r.start();
    int64_t xx,yy,x,xx1;

//dynamic also possible
#pragma omp parallel for shared(ch,cu,cv,cz,cu1r) private(yy,xx,x,xx1,dh1,dh2,du1,du2,hl,hr) firstprivate(yy1) schedule (static, CHUNK)
    for(yy = ch.start(); yy<ch.height();yy++)
    {
      x = ch.start();
      dh1 = ch(x-1,yy)-ch(x-2,yy);
      du1 = cu(x-1,yy)-cu(x-2,yy);
      dh2 = ch(x,yy)-ch(x-1,yy);
      du2 = cu(x,yy)-cu(x-1,yy);
      hr = ch(x-1,yy)+0.5*Minmod(dh1,dh2);

      xx1 = cu1r.start();
      for(xx = ch.start()+1;xx<ch.width()-1; xx++)
      {
        dh1 = dh2;
        du1 = du2;
        dh2 = ch(xx+1,yy)-ch(xx,yy);
        du2 = cu(xx+1,yy)-cu(xx,yy);
        hl = ch(xx,yy)-0.5*Minmod(dh1,dh2);
        cu1r(xx1,yy1)=cu(xx,yy)-0.5*Minmod(du1,du2)*hr/ch(xx,yy);

        xx1++;
      }
      x = ch.width()-1;
      xx1 = cu1r.width()-1;
      du1 = du2;
      du2=ch(x+2,yy)-cu(x+1,yy);
      cu1r(xx1,yy1) = cu(x+1,yy)+0.5*Minmod(du1,du2)*hl/ch(x+1,yy);

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

#include <KernelsOmp/Khuvu1r_omp.l2c>
