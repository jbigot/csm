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

class Khuvv1r_omp:virtual public Go
{
public:
  Datah* h;
  Datah* u;
  Datah* v;
  Datah* z;
  Datah* v1r;

  Khuvv1r_omp(){}
  ~Khuvv1r_omp(){}
  
  void go ()
  {
    //cout<<"====START Khuvv1r===="<<endl;
    Controller<double> ch(h);
    Controller<double> cv(v);
    Controller<double> cv1r(v1r);

    double dh1,dh2,dv1,dv2;
    double hl,hr;

    int64_t yy1 = cv1r.start();
    int64_t xx,yy,x,xx1;

//dynamic also possible
#pragma omp parallel for shared(ch,cv,cv1r) private(yy,xx,yy1,xx1,x,dh1,dh2,dv1,dv2,hl,hr) schedule (static, CHUNK)
    for(yy = ch.start(); yy<ch.height();yy++)
    {
      x = ch.start();
      dh1 = ch(x-1,yy)-ch(x-2,yy);
      dv1 = cv(x-1,yy)-cv(x-2,yy);
      dh2 = ch(x,yy)-ch(x-1,yy);
      dv2 = cv(x,yy)-cv(x-1,yy);
      hr = ch(x-1,yy)+0.5*Minmod(dh1,dh2);

      xx1 = cv1r.start();
      for(xx = ch.start()+1;xx<ch.width()-1; xx++)
      {
        dh1 = dh2;
        dv1 = dv2;
        dh2 = ch(xx+1,yy)-ch(xx,yy);
        dv2 = cv(xx+1,yy)-cv(xx,yy);
        hl = ch(xx,yy)-0.5*Minmod(dh1,dh2);
        cv1r(xx1,yy1)=cv(xx,yy)-0.5*Minmod(dv1,dv2)*hr/ch(xx,yy);

        xx1++;
      }
      x = ch.width()-1;
      xx1 = cv1r.width()-1;
      dv1 = dv2;
      dv2=ch(x+2,yy)-cv(x+1,yy);
      cv1r(xx1,yy1) = cv(x+1,yy)+0.5*Minmod(dv1,dv2)*hl/ch(x+1,yy);

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

#include <KernelsOmp/Khuvv1r_omp.l2c>
