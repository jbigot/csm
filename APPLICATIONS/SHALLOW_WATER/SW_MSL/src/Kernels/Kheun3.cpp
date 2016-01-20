#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controller.hpp>
#include <kernelh.h>
#include <mpi.h>

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

class Kheun3:virtual public Go
{
public:
  Datah* qsa2;
  Datah* h;
  Datah* q2;

  Kheun3(){}
  ~Kheun3(){}
  
  void go ()
  {/*
    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&(mpi_rank));

    Chronometer chrono;
    chrono.start();
   */
    //cout<<"====START Kheun3===="<<endl;
    Controller<double> cqsa2(qsa2);
    Controller<double> ch(h);
    Controller<double> cq2(q2);

    int64_t yyq=cq2.start();
    for(int64_t yy = ch.start(); yy<ch.height();yy++,yyq++)
    {
      int64_t xxq=cq2.start();
      for(int64_t xx = ch.start();xx<ch.width(); xx++,xxq++)
      {
        cq2(xxq,yyq)=0.5*(cq2(xxq,yyq)+cqsa2(xxq,yyq));
      }
    }/*                                                                                                                                                           
    chrono.stop();
    cout<<mpi_rank<<" compute Kheun3 : "<<chrono.dureeCalcule()<<endl;*/
                       
  }

};

#include <Kernels/Kheun3.l2c>
