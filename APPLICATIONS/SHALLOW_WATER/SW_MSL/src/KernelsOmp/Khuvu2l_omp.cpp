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

class Khuvu2l_omp:virtual public Go
{
public:
  Datah* h;
  Datah* u;
  Datah* v;
  Datah* z;
  Datah* u2l;

  Khuvu2l_omp(){}
  ~Khuvu2l_omp(){}
  
  void go ()
  {
    //cout<<"====START Khuvu2l===="<<endl;
    Controller<double> ch(h);
    Controller<double> cu(u);
    Controller<double> cu2l(u2l);

    double dh1,dh2,du1,du2;
    double hl;

    //other lines
    int64_t yy2l = cu2l.start();

//dynamic also possible
#pragma omp parallel for shared(ch,cu,cu2l) private(yy,xx,yy2l,xx2l,dh1,dh2,du1,du2,hl) schedule (static, CHUNK)
    for(int64_t yy = ch.start()+1; yy<ch.height()-1;yy++)
    {
      int64_t xx2l = cu2l.start();
      for(int64_t xx = ch.start();xx<ch.width(); xx++)
      {
        dh1 = ch(xx,yy)-ch(xx,yy+1);
        du1 = cu(xx,yy)-cu(xx,yy+1);
        dh2 = ch(xx,yy-1)-ch(xx,yy);
        du2 = cu(xx,yy-1)-cu(xx,yy);
        hl = ch(xx,yy)-0.5*Minmod(dh1,dh2);
        cu2l(xx2l,yy2l)=cu(xx,yy)+0.5*Minmod(du1,du2)*hl/ch(xx,yy);

        xx2l++;
      }
      yy2l++;
    }

    //last line
    int64_t y = ch.height()-1;
    int64_t yy2 = cu2l.height()-1;
    int64_t xx2 = cu2l.start();
//dynamic also possible
#pragma omp parallel for shared(ch,cu,cu2l) private(yy,xx,yy2,xx2,y,dh1,dh2,du1,du2,hl) schedule (static, CHUNK)
    for(int64_t xx = ch.start();xx<ch.width(); xx++)
    {
      dh1 = ch(xx,y+1)-ch(xx,y+2);
      du1 = cu(xx,y+1)-cu(xx,y+2);
      dh2 = ch(xx,y)-ch(xx,y+1);
      du2 = cu(xx,y)-cu(xx,y+1);
      hl = ch(xx,y+1)-0.5*Minmod(dh1,dh2);
      cu2l(xx2,yy2)=cu(xx,y+1)+0.5*Minmod(du1,du2)*hl/ch(xx,y+1);

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

#include <KernelsOmp/Khuvu2l_omp.l2c>
