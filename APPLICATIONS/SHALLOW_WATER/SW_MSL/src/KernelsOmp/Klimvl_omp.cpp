#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controllerPHBL.hpp>
#include <kernelh.h>

#include <omp.h>
#define CHUNK 5

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

class Klimvl_omp:virtual public Go
{
public:
  Datah* v;

  Klimvl_omp(){}
  ~Klimvl_omp(){}
  
  void go ()
  {
    //cout<<"====START Klimvl===="<<endl;
  	ControllerPHBL<double> cv(v);

//dynamic also possible
#pragma omp parallel for shared(cv) private(yy,xx) schedule (static, CHUNK)
    for(int64_t yy = cv.startHeight(); yy<cv.height();yy++)                                                                                                               
      {
	     for(int64_t xx = cv.startWidth();xx<cv.width(); xx++)
	     {
	     	cv(xx,yy) = cv(xx+1,yy);
	     }                                                                                                                                                                                           
      }                 
  }

};

#include <KernelsOmp/Klimvl_omp.l2c>
