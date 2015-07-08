#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controller.hpp>
#include <kernelh.h>

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

class Kheun1:virtual public Go
{
public:
  Datah* hsa;
  Datah* h;

  Kheun1(){}
  ~Kheun1(){}
  
  void go ()
  {
    //cout<<"====START Kheun1===="<<endl;
    Controller<double> chsa(hsa);
    // Controller<double> cqsa1(qsa1);
    // Controller<double> cqsa2(qsa2);
    Controller<double> ch(h);
    // Controller<double> cq1(q1);
    // Controller<double> cq2(q2);

    //int64_t yyq=cq1.start();
    for(int64_t yy = ch.start(); yy<ch.height();yy++)//,yyq++)
    {
      //int64_t xxq=cq1.start();
      for(int64_t xx = ch.start();xx<ch.width(); xx++)//,xxq++)
      {
        ch(xx,yy)=0.5*(ch(xx,yy)+chsa(xx,yy));
        // cq1(xxq,yyq)=0.5*(cq1(xxq,yyq)+cqsa1(xxq,yyq));
        // cq2(xxq,yyq)=0.5*(cq2(xxq,yyq)+cqsa2(xxq,yyq));
      }
    }                                                                                                                                                                                  
  }

};

#include <Kernels/Kheun1.l2c>
