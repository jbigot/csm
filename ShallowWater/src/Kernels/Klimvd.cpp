#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controllerPHBD.hpp>
#include <kernelh.h>

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

class Klimvd:virtual public Go
{
public:
  Datah* v;

  Klimvd(){}
  ~Klimvd(){}
  
  void go ()
  {
    //cout<<"====START Klimvd===="<<endl;
  	ControllerPHBD<double> cv(v);
    
    for(int64_t yy = cv.startHeight(); yy<cv.height();yy++)                                                                                                               
      {
	     for(int64_t xx = cv.startWidth();xx<cv.width(); xx++)
	     {
	     	cv(xx,yy) = cv(xx,yy-1);
	     }                                                                                                                                                                                           
      }                                                                                                                                                                                     
  }

};

#include <Kernels/Klimvd.l2c>
