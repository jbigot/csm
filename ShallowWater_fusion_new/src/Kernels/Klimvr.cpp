#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controllerPHBR.hpp>
#include <kernelh.h>

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

class Klimvr:virtual public Go
{
public:
  Datah* v;

  Klimvr(){}
  ~Klimvr(){}
  
  void go ()
  {
    //cout<<"====START Klimvr===="<<endl;
  	ControllerPHBR<double> cv(v);
    
    for(int64_t yy = cv.startHeight(); yy<cv.height();yy++)                                                                                                               
      {
	     for(int64_t xx = cv.startWidth();xx<cv.width(); xx++)
	     {
	     	cv(xx,yy) = cv(xx-1,yy);
	     }                                                                                                                                                                                           
      }                                                                                                                                                                                      
  }

};

#include <Kernels/Klimvr.l2c>
