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

#define grav_dem 4.905

class Kbloc21_omp:virtual public Go
{
public:
  Datah* h;
  Datah* f1;
  Datah* g1;
  Datah* hs;

  Kbloc21_omp(){}
  ~Kbloc21_omp(){}
  
  void go ()
  {
    //cout<<"====START Kbloc21===="<<endl;
    Controller<double> ch(h);
    Controller<double> cf1(f1);
    Controller<double> cg1(g1);
    Controller<double> chs(hs);

    double tx=0.005;
    double ty=0.005;
    double dt=0.5;

    int64_t yy1=cf1.start();
    int64_t yy2=cg1.start();

    int64_t xx,yy,xx1,xx2;

//dynamic also possible
#pragma omp parallel for shared(ch,cf1,cg1,chs) private(yy,xx,yy1,yy2,xx1,xx2) schedule (static, CHUNK)
    for(yy = ch.start(); yy<ch.height();yy++)
    {
      xx1=cf1.start();
      xx2=cg1.start();
      for(xx = ch.start();xx<ch.width(); xx++)
      {
        chs(xx,yy) = ch(xx,yy)-tx*(cf1(xx1+1,yy1)-cf1(xx1,yy1))-ty*(cg1(xx2,yy2)-cg1(xx2,yy2+1));
        xx1++;
        xx2++;
      }
      yy1++;
      yy2++;
    }
  }

};

#include <KernelsOmp/Kbloc21_omp.l2c>
