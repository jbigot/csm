#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controller.hpp>
#include <kernelh.h>
#include <math.h>

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

#define he_ca 1.e-6
#define ve_ca 1.e-6

class Kcheck:virtual public Go
{
public:
  Datah* hs;
  Datah* us;
  Datah* vs;
  Datah* qs1;
  Datah* qs2;

  Kcheck(){}
  ~Kcheck(){}
  
  void go ()
  {
    //cout<<"====START Kcheck===="<<endl;
    Controller<double> chs(hs);
    Controller<double> cus(us);
    Controller<double> cvs(vs);
    Controller<double> cqs1(qs1);
    Controller<double> cqs2(qs2);

    int64_t yyq=cqs1.start();
    for(int64_t yy = chs.start(); yy<chs.height();yy++,yyq++)
    {
      int64_t xxq=cqs1.start();
      for(int64_t xx = chs.start();xx<chs.width(); xx++,xxq++)
      {
        if(chs(xx,yy)<he_ca)
        {
          chs(xx,yy)=0.;
          cus(xx,yy)=0.;
          cvs(xx,yy)=0.;
          cqs1(xxq,yyq)=0.;
          cqs2(xxq,yyq)=0.;
        }
        else
        {
          cus(xx,yy)=cqs1(xxq,yyq)/chs(xx,yy);
          if(fabs(cus(xx,yy))<ve_ca)
          {
            cus(xx,yy)=0.;
            cqs1(xxq,yyq)=0.;
          }
          cvs(xx,yy)=cqs2(xxq,yyq)/chs(xx,yy);
          if(fabs(cvs(xx,yy))<ve_ca)
          {
            cvs(xx,yy)=0.;
            cqs2(xxq,yyq)=0.;
          }
        }
      }
    }                                                                                                                                                                             
  }

};

#include <Kernels/Kcheck.l2c>
