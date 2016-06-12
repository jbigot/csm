#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controller.hpp>
#include <kernelh.h>
#include <math.h>

#include <omp.h>
#define CHUNK 5

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)
#define grav 9.81
#define grav_dem 4.905

class Kbloc1y3_bloc1y4_bloc1y5_omp:virtual public Go
{
public:
  Datah* u2l;
  Datah* u2r;
  Datah* v2l;
  Datah* v2r;
  Datah* h2g;
  Datah* h2d;
  Datah* g1;
  Datah* g2;
  Datah* g3;

  Kbloc1y3_bloc1y4_bloc1y5_omp(){}
  ~Kbloc1y3_bloc1y4_bloc1y5_omp(){}
  
  void go ()
  {
    //cout<<"====START Kbloc1y3_bloc1y4_bloc1y5===="<<endl;
    Controller<double> cu2l(u2l);
    Controller<double> cu2r(u2r);
    Controller<double> cv2l(v2l);
    Controller<double> cv2r(v2r);
    Controller<double> ch2g(h2g);
    Controller<double> ch2d(h2d);
    Controller<double> cg1(g1);
    Controller<double> cg2(g2);
    Controller<double> cg3(g3);

    int64_t xx,yy;
    double c,cd,qd,qg;

//dynamic also possible
#pragma omp parallel for shared(cu2l,cu2r,cv2l,cv2r,ch2g,ch2d,cg1,cg2,cg3) private(yy,xx,c,cd,qd,qg) schedule (static, CHUNK)
    for(yy = cu2l.start(); yy<cu2l.height();yy++)
    {
      for(xx = cu2l.start();xx<cu2l.width(); xx++)
      {
        if (ch2g(xx,yy)<=0. && ch2d(xx,yy)<=0.)
        {
          cg1(xx,yy)=0.;
          cg2(xx,yy)=0.;
          cg3(xx,yy)=0.;
        }
        else
        {
          c = max(fabs(cu2l(xx,yy))+sqrt(grav*ch2g(xx,yy)),fabs(cu2r(xx,yy))+sqrt(grav*ch2d(xx,yy)));
          cd = c*0.5;
          qd = cu2r(xx,yy)*ch2d(xx,yy);
          qg = cu2l(xx,yy)*ch2g(xx,yy);
          cg1(xx,yy)=(qg+qd)*0.5-cd*(ch2d(xx,yy)-ch2g(xx,yy));
          cg2(xx,yy)=((cu2l(xx,yy)*qg)+(grav_dem*ch2g(xx,yy)*ch2g(xx,yy))+(cu2r(xx,yy)*qd)+(grav_dem*ch2d(xx,yy)*ch2d(xx,yy)))*0.5-cd*(qd-qg);
          cg3(xx,yy)=(qg*cv2l(xx,yy)+qd*cv2r(xx,yy))*0.5-cd*(ch2d(xx,yy)*cv2r(xx,yy)-ch2g(xx,yy)*cv2l(xx,yy));
        }
      }
    }                                                                                                                                                                                    
  }

};

#include <KernelsOmp/Kbloc1y3_bloc1y4_bloc1y5_omp.l2c>
