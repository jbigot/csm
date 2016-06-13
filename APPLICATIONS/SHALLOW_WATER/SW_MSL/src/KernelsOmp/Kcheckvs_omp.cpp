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

class Kcheckvs_omp:virtual public Go
{
public:
  Datah* hs;
  Datah* vs;
  Datah* qs2;

  Kcheckvs_omp(){}
  ~Kcheckvs_omp(){}
  
  void go ()
  {
    //cout<<"====START Kcheckvs===="<<endl;
    Controller<double> chs(hs);
    Controller<double> cvs(vs);
    Controller<double> cqs2(qs2);

    int64_t yyq=cqs2.start();
    int64_t xx,yy,xxq;

//dynamic also possible
#pragma omp parallel for shared(chs,cvs,cqs2) private(yy,xx,xxq) firstprivate(yyq) schedule (static, CHUNK)
    for(yy = chs.start(); yy<chs.height();yy++)
    {
      xxq=cqs2.start();
      for(xx = chs.start();xx<chs.width(); xx++)
      {
        if(chs(xx,yy)<he_ca)
        {
          chs(xx,yy)=0.;
          cvs(xx,yy)=0.;
          cqs2(xxq,yyq)=0.;
        }
        else
        {
          cvs(xx,yy)=cqs2(xxq,yyq)/chs(xx,yy);
          if(fabs(cvs(xx,yy))<ve_ca)
          {
            cvs(xx,yy)=0.;
            cqs2(xxq,yyq)=0.;
          }
        }
        xxq++;
      }
      yyq++;
    }
  }

};

#include <KernelsOmp/Kcheckvs_omp.l2c>
