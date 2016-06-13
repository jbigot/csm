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

class Khuvh1l_omp:virtual public Go
{
public:
  Datah* h;
  Datah* u;
  Datah* v;
  Datah* z;
  Datah* h1l;

  Khuvh1l_omp(){}
  ~Khuvh1l_omp(){}
  
  void go ()
  {
    //cout<<"====START Khuvh1l===="<<endl;
    Controller<double> ch(h);
    Controller<double> cu(u);
    Controller<double> cv(v);
    Controller<double> cz(z);
    Controller<double> ch1l(h1l);

    double dh1,dh2;
    double hr;

    int64_t yy1 = ch1l.start();
    int64_t xx,yy,x,xx1;

//dynamic also possible
#pragma omp parallel for shared(ch,cu,cv,cz,ch1l) private(yy,xx,xx1,dh1,dh2,hr) firstprivate(yy1) schedule (static, CHUNK)
    for(yy = ch.start(); yy<ch.height();yy++)
    {
      x = ch.start();
      dh1 = ch(x-1,yy)-ch(x-2,yy);
      dh2 = ch(x,yy)-ch(x-1,yy);
      hr = ch(x-1,yy)+0.5*Minmod(dh1,dh2);

      xx1 = ch1l.start();
      for(xx = ch.start()+1;xx<ch.width()-1; xx++)
      {
        ch1l(xx1,yy1)=hr;
        dh1 = dh2;
        dh2 = ch(xx+1,yy)-ch(xx,yy);
        hr = ch(xx,yy)+0.5*Minmod(dh1,dh2);

        xx1++;
      }
      xx1 = ch1l.width()-1;
      ch1l(xx1,yy1)=hr;

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

#include <KernelsOmp/Khuvh1l_omp.l2c>
