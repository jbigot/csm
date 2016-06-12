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

class Khuvz2r_omp:virtual public Go
{
public:
  Datah* h;
  Datah* u;
  Datah* v;
  Datah* z;
  Datah* z2r;

  Khuvz2r_omp(){}
  ~Khuvz2r_omp(){}
  
  void go ()
  {
    //cout<<"====START Khuvz2r===="<<endl;
    Controller<double> ch(h);
    Controller<double> cz(z);
    Controller<double> cz2r(z2r);

    double dh1,dh2,dz1,dz2;

    //first line
    int64_t y = ch.start();
    int64_t yy2 = cz2r.start();
    int64_t xx2 = cz2r.start();
    int64_t xx,yy,xx2r;

//dynamic also possible
#pragma omp parallel for shared(ch,cz,cz2r) private(yy,xx,y,yy2,xx2,dh1,dh2,dz1,dz2) schedule (static, CHUNK)
    for(xx = ch.start();xx<ch.width(); xx++)
    {
      dh1 = ch(xx,y-1)-ch(xx,y);
      dz1 = cz(xx,y-1)-cz(xx,y);
      dh2 = ch(xx,y-2)-ch(xx,y-1);
      dz2 = cz(xx,y-2)-cz(xx,y-1);
      cz2r(xx2,yy2)=cz(xx,y-1)-0.5*(Minmod(dz1+dh1,dz2+dh2)-Minmod(dh1,dh2));

      xx2++;
    }

    //other lines
    int64_t yy2r = cz2r.start()+1;

//dynamic also possible
#pragma omp parallel for shared(ch,cz,cz2r) private(yy,xx,yy2r,xx2r,dh1,dh2,dz1,dz2) schedule (static, CHUNK)
    for(yy = ch.start()+1; yy<ch.height()-1;yy++)
    {
      xx2r = cz2r.start();
      for(xx = ch.start();xx<ch.width(); xx++)
      {
        dh1 = ch(xx,yy)-ch(xx,yy+1);
        dz1 = cz(xx,yy)-cz(xx,yy+1);
        dh2 = ch(xx,yy-1)-ch(xx,yy);
        dz2 = cz(xx,yy-1)-cz(xx,yy);
        cz2r(xx2r,yy2r)=cz(xx,yy)-0.5*(Minmod(dz1+dh1,dz2+dh2)-Minmod(dh1,dh2));

        xx2r++;
      }
      yy2r++;
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

#include <KernelsOmp/Khuvz2r_omp.l2c>
