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

class Khuvu2r_omp:virtual public Go
{
public:
  Datah* h;
  Datah* u;
  Datah* v;
  Datah* z;
  Datah* u2r;

  Khuvu2r_omp(){}
  ~Khuvu2r_omp(){}
  
  void go ()
  {
    //cout<<"====START Khuvu2r===="<<endl;
    Controller<double> ch(h);
    Controller<double> cu(u);
    Controller<double> cu2r(u2r);

    double dh1,dh2,du1,du2;
    double hr;

    //first line
    int64_t y = ch.start();
    int64_t yy2 = cu2r.start();
    int64_t xx2 = cu2r.start();

//dynamic also possible
#pragma omp parallel for shared(ch,cu,cu2r) private(yy,xx,xx2,y,yy2,dh1,dh2,du1,du2,hr) schedule (static, CHUNK)
    for(int64_t xx = ch.start();xx<ch.width(); xx++)
    {
      dh1 = ch(xx,y-1)-ch(xx,y);
      du1 = cu(xx,y-1)-cu(xx,y);
      dh2 = ch(xx,y-2)-ch(xx,y-1);
      du2 = cu(xx,y-2)-cu(xx,y-1);
      hr = ch(xx,y-1)+0.5*Minmod(dh1,dh2);
      cu2r(xx2,yy2)=cu(xx,y-1)-0.5*Minmod(du1,du2)*hr/ch(xx,y-1);

      xx2++;
    }

    //other lines
    int64_t yy2r = cu2r.start()+1;

//dynamic also possible
#pragma omp parallel for shared(ch,cu2r,cu) private(yy,xx,yy2r,xx2r,dh1,dh2,du1,du2,hr) schedule (static, CHUNK)
    for(int64_t yy = ch.start()+1; yy<ch.height()-1;yy++)
    {
      int64_t xx2r = cu2r.start();
      for(int64_t xx = ch.start();xx<ch.width(); xx++)
      {
        dh1 = ch(xx,yy)-ch(xx,yy+1);
        du1 = cu(xx,yy)-cu(xx,yy+1);
        dh2 = ch(xx,yy-1)-ch(xx,yy);
        du2 = cu(xx,yy-1)-cu(xx,yy);
        hr = ch(xx,yy)+0.5*Minmod(dh1,dh2);
        cu2r(xx2r,yy2r)=cu(xx,yy)-0.5*Minmod(du1,du2)*hr/ch(xx,yy);

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

#include <KernelsOmp/Khuvu2r_omp.l2c>
