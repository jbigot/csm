#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controllerPHBD.hpp>
#include <kernelh.h>

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

class Klimhd:virtual public Go
{
public:
  Datah* h;

  Klimhd(){}
  ~Klimhd(){}
  
  void go ()
  {
    //cout<<"====START Klimhd===="<<endl;
  	ControllerPHBD<double> ch(h);
    
    for(int64_t yy = ch.startHeight(); yy<ch.height();yy++)                                                                                                               
      {
	     for(int64_t xx = ch.startWidth();xx<ch.width(); xx++)
	     {
	     	ch(xx,yy) = ch(xx,yy-1);
	     }                                                                                                                                                                                           
      }                                                                                                                                                                                     
  }

};

#include <Kernels/Klimhd.l2c>
