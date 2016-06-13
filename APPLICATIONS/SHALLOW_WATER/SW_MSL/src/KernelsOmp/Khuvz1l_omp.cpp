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

class Khuvz1l_omp:virtual public Go
{
public:
  Datah* h;
  Datah* u;
  Datah* v;
  Datah* z;
  Datah* z1l;

  Khuvz1l_omp(){}
  ~Khuvz1l_omp(){}
  
  void go ()
  {
    //cout<<"====START Khuvz1l===="<<endl;
    Controller<double> ch(h);
    Controller<double> cz(z);
    Controller<double> cz1l(z1l);

    double dh1,dh2,dz1,dz2;
    double zr;

    int64_t yy1 = cz1l.start();
    int64_t xx,yy,x,xx1;

//dynamic also possible
#pragma omp parallel for shared(ch,cz,cz1l) private(yy,xx,xx1,x,dh1,dh2,dz1,dz2,zr) firstprivate(yy1) schedule (static, CHUNK)
    for(yy = ch.start(); yy<ch.height();yy++)
    {
      x = ch.start();
      dh1 = ch(x-1,yy)-ch(x-2,yy);
      dz1 = cz(x-1,yy)-cz(x-2,yy);
      dh2 = ch(x,yy)-ch(x-1,yy);
      dz2 = cz(x,yy)-cz(x-1,yy);
      zr = cz(x-1,yy)+0.5*(Minmod(dz1+dh1,dz2+dh2)-Minmod(dh1,dh2));

      xx1 = cz1l.start();
      for(xx = ch.start()+1;xx<ch.width()-1; xx++)
      {
        cz1l(xx1,yy1)=zr;
        dh1 = dh2;
        dz1 = dz2;
        dh2 = ch(xx+1,yy)-ch(xx,yy);
        dz2 = cz(xx+1,yy)-cz(xx,yy);
        zr=cz(xx,yy)+0.5*(Minmod(dz1+dh1,dz2+dh2)-Minmod(dh1,dh2));

        xx1++;
      }
      xx1 = cz1l.width()-1;
      cz1l(xx1,yy1)=zr;

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

#include <KernelsOmp/Khuvz1l_omp.l2c>
