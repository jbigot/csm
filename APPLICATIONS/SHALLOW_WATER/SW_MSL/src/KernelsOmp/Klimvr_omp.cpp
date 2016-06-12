#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controllerPHBR.hpp>
#include <kernelh.h>

#include <omp.h>
#define CHUNK 5

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

class Klimvr_omp:virtual public Go
{
public:
  Datah* v;

  Klimvr_omp(){}
  ~Klimvr_omp(){}
  
  void go ()
  {
    //cout<<"====START Klimvr===="<<endl;
  	ControllerPHBR<double> cv(v);

    int64_t xx,yy;

//dynamic also possible
#pragma omp parallel for shared(cv) private(yy,xx) schedule (static, CHUNK)
    for(yy = cv.startHeight(); yy<cv.height();yy++)                                                                                                               
      {
	     for(xx = cv.startWidth();xx<cv.width(); xx++)
	     {
	     	cv(xx,yy) = cv(xx-1,yy);
	     }                                                                                                                                                                                           
      }                   
  }

};

#include <KernelsOmp/Klimvr_omp.l2c>
