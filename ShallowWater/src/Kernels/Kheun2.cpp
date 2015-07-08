#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controller.hpp>
#include <kernelh.h>

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

class Kheun2:virtual public Go
{
public:
  Datah* qsa1;
  Datah* h;
  Datah* q1;

  Kheun2(){}
  ~Kheun2(){}
  
  void go ()
  {
    //cout<<"====START Kheun2===="<<endl;
    Controller<double> cqsa1(qsa1);
    Controller<double> ch(h);
    Controller<double> cq1(q1);

    int64_t yyq=cq1.start();
    for(int64_t yy = ch.start(); yy<ch.height();yy++,yyq++)
    {
      int64_t xxq=cq1.start();
      for(int64_t xx = ch.start();xx<ch.width(); xx++,xxq++)
      {
        cq1(xxq,yyq)=0.5*(cq1(xxq,yyq)+cqsa1(xxq,yyq));
      }
    }                                                                                                                                                                                  
  }

};

#include <Kernels/Kheun2.l2c>
