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

class Khuvh2r_omp:virtual public Go
{
public:
  Datah* h;
  Datah* u;
  Datah* v;
  Datah* z;
  Datah* h2r;

  Khuvh2r_omp(){}
  ~Khuvh2r_omp(){}
  
  void go ()
  {
    //cout<<"====START Khuvh2r===="<<endl;
    Controller<double> ch(h);
    Controller<double> ch2r(h2r);

    double dh1,dh2;
    double hl;

    //first line
    int64_t y = ch.start();
    int64_t yy2 = ch2r.start();
    int64_t xx2 = ch2r.start();
    int64_t xx,yy,yy2r,xx2r;

//dynamic also possible
#pragma omp parallel for shared(ch,ch2r) private(yy,xx,dh1,dh2,hl) firstprivate(y,yy2,xx2) schedule (static, CHUNK)
    for(xx = ch.start();xx<ch.width(); xx++)
    {
      dh1 = ch(xx,y-1)-ch(xx,y);
      dh2 = ch(xx,y-2)-ch(xx,y-1);
      hl = ch(xx,y-1)-0.5*Minmod(dh1,dh2);
      ch2r(xx2,yy2)=hl;

      xx2++;
    }

    //other lines
    yy2r = ch2r.start()+1;

//dynamic also possible
#pragma omp parallel for shared(ch,ch2r) private(yy,xx,xx2r,dh1,dh2,hl) firstprivate(yy2r) schedule (static, CHUNK)
    for(yy = ch.start()+1; yy<ch.height()-1;yy++)
    {
      xx2r = ch2r.start();
      for(xx = ch.start();xx<ch.width(); xx++)
      {
        dh1 = ch(xx,yy)-ch(xx,yy+1);
        dh2 = ch(xx,yy-1)-ch(xx,yy);
        hl = ch(xx,yy)-0.5*Minmod(dh1,dh2);
        ch2r(xx2r,yy2r)=hl;

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

#include <KernelsOmp/Khuvh2r_omp.l2c>
