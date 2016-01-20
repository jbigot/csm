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

class Kcheckus:virtual public Go
{
public:
  Datah* hs;
  Datah* us;
  //Datah* vs;
  Datah* qs1;
  //Datah* qs2;

  Kcheckus(){}
  ~Kcheckus(){}
  
  void go ()
  {/*
    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&(mpi_rank));

    Chronometer chrono;
    chrono.start();
   */
    //cout<<"====START Kcheckus===="<<endl;
    Controller<double> chs(hs);
    Controller<double> cus(us);
    //Controller<double> cvs(vs);
    Controller<double> cqs1(qs1);
    //Controller<double> cqs2(qs2);

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
          //cvs(xx,yy)=0.;
          cqs1(xxq,yyq)=0.;
          //cqs2(xxq,yyq)=0.;
        }
        else
        {
          cus(xx,yy)=cqs1(xxq,yyq)/chs(xx,yy);
          if(fabs(cus(xx,yy))<ve_ca)
          {
            cus(xx,yy)=0.;
            cqs1(xxq,yyq)=0.;
          }
          // cvs(xx,yy)=cqs2(xxq,yyq)/chs(xx,yy);
          // if(fabs(cvs(xx,yy))<ve_ca)
          // {
          //   cvs(xx,yy)=0.;
          //   cqs2(xxq,yyq)=0.;
          // }
        }
      }
    }     /*                                                                                                                                                      
    chrono.stop();
    cout<<mpi_rank<<" compute Kcheckus : "<<chrono.dureeCalcule()<<endl;
	  */      
  }

};

#include <Kernels/Kcheckus.l2c>
