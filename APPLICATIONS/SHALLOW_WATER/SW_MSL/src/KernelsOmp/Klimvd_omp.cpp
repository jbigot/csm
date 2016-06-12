#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controllerPHBD.hpp>
#include <kernelh.h>

#include <omp.h>
#define CHUNK 5

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

class Klimvd_omp:virtual public Go
{
public:
  Datah* v;

  Klimvd_omp(){}
  ~Klimvd_omp(){}
  
  void go ()
  {
    //cout<<"====START Klimvd===="<<endl;
  	ControllerPHBD<double> cv(v);

    int64_t xx,yy;

//dynamic also possible
#pragma omp parallel for shared(cv) private(yy,xx) schedule (static, CHUNK)
    for(yy = cv.startHeight(); yy<cv.height();yy++)                                                                                                               
      {
	     for(xx = cv.startWidth();xx<cv.width(); xx++)
	     {
	     	cv(xx,yy) = cv(xx,yy-1);
	     }                                                                                                                                                                                           
      }                   
  }

};

#include <KernelsOmp/Klimvd_omp.l2c>
