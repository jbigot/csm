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

class Khuvz2l_omp:virtual public Go
{
public:
  Datah* h;
  Datah* u;
  Datah* v;
  Datah* z;
  Datah* z2l;

  Khuvz2l_omp(){}
  ~Khuvz2l_omp(){}
  
  void go ()
  {
    //cout<<"====START Khuvz2l===="<<endl;
    Controller<double> ch(h);
    Controller<double> cz(z);
    Controller<double> cz2l(z2l);

    double dh1,dh2,dz1,dz2;

    //other lines
    int64_t yy2l = cz2l.start();
    int64_t xx,yy,xx2l;

//dynamic also possible
#pragma omp parallel for shared(ch,cz,cz2l) private(yy,xx,xx2l,dh1,dh2,dz1,dz2) firstprivate(yy2l) schedule (static, CHUNK)
    for(yy = ch.start()+1; yy<ch.height()-1;yy++)
    {
      xx2l = cz2l.start();
      for(xx = ch.start();xx<ch.width(); xx++)
      {
        dh1 = ch(xx,yy)-ch(xx,yy+1);
        dz1 = cz(xx,yy)-cz(xx,yy+1);
        dh2 = ch(xx,yy-1)-ch(xx,yy);
        dz2 = cz(xx,yy-1)-cz(xx,yy);
        cz2l(xx2l,yy2l)=cz(xx,yy)+0.5*(Minmod(dz1+dh1,dz2+dh2)-Minmod(dh1,dh2));

        xx2l++;
      }
      yy2l++;
    }

    //last line
    int64_t y = ch.height()-1;
    int64_t yy2 = cz2l.height()-1;
    int64_t xx2 = cz2l.start();

//dynamic also possible
#pragma omp parallel for shared(ch,cz,cz2l) private(yy,xx,dh1,dh2,dz1,dz2) firstprivate(y,yy2,xx2) schedule (static, CHUNK)
    for(xx = ch.start();xx<ch.width(); xx++)
    {
      dh1 = ch(xx,y+1)-ch(xx,y+2);
      dz1 = cz(xx,y+1)-cz(xx,y+2);
      dh2 = ch(xx,y)-ch(xx,y+1);
      dz2 = cz(xx,y)-cz(xx,y+1);
      cz2l(xx2,yy2)=cz(xx,y+1)+0.5*(Minmod(dz1+dh1,dz2+dh2)-Minmod(dh1,dh2));

      xx2++;
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

#include <KernelsOmp/Khuvz2l_omp.l2c>
