#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controllerPHBU.hpp>
#include <kernelh.h>

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

class Klimvu:virtual public Go
{
public:
  Datah* v;

  Klimvu(){}
  ~Klimvu(){}
  
  void go ()
  {
    //cout<<"====START Klimvu===="<<endl;
  	ControllerPHBU<double> cv(v);
    
    for(int64_t yy = cv.startHeight(); yy<cv.height();yy++)                                                                                                               
      {
	     for(int64_t xx = cv.startWidth();xx<cv.width(); xx++)
	     {
	     	cv(xx,yy) = cv(xx,yy+1);
	     }                                                                                                                                                                                           
      }                                                                                                                                                                                 
  }

};

#include <Kernels/Klimvu.l2c>
