#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controller.hpp>
#include <kernelh.h>
#include <math.h>
#include <mpi.h>

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

#define he_ca 1.e-6
#define ve_ca 1.e-6

class Kcheckvs:virtual public Go
{
public:
  Datah* hs;
  Datah* vs;
  Datah* qs2;

  Kcheckvs(){}
  ~Kcheckvs(){}
  
  void go ()
  {/*
    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&(mpi_rank));

    Chronometer chrono;
    chrono.start();
   */
    //cout<<"====START Kcheckvs===="<<endl;
    Controller<double> chs(hs);
    Controller<double> cvs(vs);
    Controller<double> cqs2(qs2);

    int64_t yyq=cqs2.start();
    for(int64_t yy = chs.start(); yy<chs.height();yy++,yyq++)
    {
      int64_t xxq=cqs2.start();
      for(int64_t xx = chs.start();xx<chs.width(); xx++,xxq++)
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
      }
    }/*
    chrono.stop();
    cout<<mpi_rank<<" compute Kcheckvs : "<<chrono.dureeCalcule()<<endl;*/
  }

};

#include <Kernels/Kcheckvs.l2c>
