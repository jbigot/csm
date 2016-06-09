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

class Klimhd_omp:virtual public Go
{
public:
  Datah* h;

  Klimhd_omp(){}
  ~Klimhd_omp(){}
  
  void go ()
  {
    //cout<<"====START Klimhd===="<<endl;
  	ControllerPHBD<double> ch(h);
    
//dynamic also possible
#pragma omp parallel for shared(ch) private(yy,xx) schedule (static, CHUNK)
    for(int64_t yy = ch.startHeight(); yy<ch.height();yy++)                                                                                                               
      {
	     for(int64_t xx = ch.startWidth();xx<ch.width(); xx++)
	     {
	     	ch(xx,yy) = ch(xx,yy-1);
	     }                                                                                                                                                                                           
      }                  
  }

};

#include <KernelsOmp/Klimhd_omp.l2c>
