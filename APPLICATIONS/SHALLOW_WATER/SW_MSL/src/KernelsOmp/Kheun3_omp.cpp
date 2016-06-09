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

class Kheun3_omp:virtual public Go
{
public:
  Datah* qsa2;
  Datah* h;
  Datah* q2;

  Kheun3_omp(){}
  ~Kheun3_omp(){}
  
  void go ()
  {
    //cout<<"====START Kheun3===="<<endl;
    Controller<double> cqsa2(qsa2);
    Controller<double> ch(h);
    Controller<double> cq2(q2);

    int64_t yyq=cq2.start();

//dynamic also possible
#pragma omp parallel for shared(cqsa2,ch,cq2) private(yy,xx,yyq,xxq) schedule (static, CHUNK)
    for(int64_t yy = ch.start(); yy<ch.height();yy++)
    {
      int64_t xxq=cq2.start();
      for(int64_t xx = ch.start();xx<ch.width(); xx++)
      {
        cq2(xxq,yyq)=0.5*(cq2(xxq,yyq)+cqsa2(xxq,yyq));
        xxq++;
      }
      yyq++;
    }
                       
  }

};

#include <KernelsOmp/Kheun3_omp.l2c>
