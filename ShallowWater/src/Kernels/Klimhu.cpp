#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controllerPHBU.hpp>
#include <kernelh.h>

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

class Klimhu:virtual public Go
{
public:
  Datah* h;
  Datah* u;
  Datah* v;

  Klimhu(){}
  ~Klimhu(){}
  
  void go ()
  {
    //cout<<"====START Klimhu===="<<endl;
  	ControllerPHBU<double> ch(h);
  	ControllerPHBU<double> cu(u);
  	ControllerPHBU<double> cv(v);
    
    for(int64_t yy = ch.startHeight(); yy<ch.height();yy++)                                                                                                               
      {
	     for(int64_t xx = ch.startWidth();xx<ch.width(); xx++)
	     {
	     	ch(xx,yy) = ch(xx,yy+1);
	     	cu(xx,yy) = -cu(xx,yy+1);
	     	cv(xx,yy) = cv(xx,yy+1);
	     }                                                                                                                                                                                           
      }                                                                                                                                                                                 
  }

};

#include <Kernels/Klimhu.l2c>
