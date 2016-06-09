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

class Kheun2_omp:virtual public Go
{
public:
  Datah* qsa1;
  Datah* h;
  Datah* q1;

  Kheun2_omp(){}
  ~Kheun2_omp(){}
  
  void go ()
  {
    //cout<<"====START Kheun2===="<<endl;
    Controller<double> cqsa1(qsa1);
    Controller<double> ch(h);
    Controller<double> cq1(q1);

    int64_t yyq=cq1.start();

//dynamic also possible
#pragma omp parallel for shared(cqsa1,ch,cq1) private(yy,xx,xxq,yyq) schedule (static, CHUNK)
    for(int64_t yy = ch.start(); yy<ch.height();yy++)
    {
      int64_t xxq=cq1.start();
      for(int64_t xx = ch.start();xx<ch.width(); xx++)
      {
        cq1(xxq,yyq)=0.5*(cq1(xxq,yyq)+cqsa1(xxq,yyq));
        xxq++;
      }
      yyq++;
    }                       
  }

};

#include <KernelsOmp/Kheun2_omp.l2c>
