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

#define he_ca 1.e-6
#define ve_ca 1.e-6

class Kcheckus_omp:virtual public Go
{
public:
  Datah* hs;
  Datah* us;
  Datah* qs1;

  Kcheckus_omp(){}
  ~Kcheckus_omp(){}
  
  void go ()
  {
    //cout<<"====START Kcheckus===="<<endl;
    Controller<double> chs(hs);
    Controller<double> cus(us);
    Controller<double> cqs1(qs1);

    int64_t yyq=cqs1.start();

    int64_t xx,yy,xxq;

//dynamic also possible
#pragma omp parallel for shared(chs,cus,cqs1) private(yy,xx,xxq) firstprivate(yyq) schedule (static, CHUNK)
    for(yy = chs.start(); yy<chs.height();yy++)
    {
      xxq=cqs1.start();
      for(xx = chs.start();xx<chs.width(); xx++)
      {
        if(chs(xx,yy)<he_ca)
        {
          chs(xx,yy)=0.;
          cus(xx,yy)=0.;
          cqs1(xxq,yyq)=0.;
        }
        else
        {
          cus(xx,yy)=cqs1(xxq,yyq)/chs(xx,yy);
          if(fabs(cus(xx,yy))<ve_ca)
          {
            cus(xx,yy)=0.;
            cqs1(xxq,yyq)=0.;
          }
        }
        xxq++;
      }
      yyq++;
    } 
  }

};

#include <KernelsOmp/Kcheckus_omp.l2c>
