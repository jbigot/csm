#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controllerPHBU.hpp>
#include <kernelh.h>

#include <omp.h>
#define CHUNK 5

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

class Klimhu_omp:virtual public Go
{
public:
  Datah* h;

  Klimhu_omp(){}
  ~Klimhu_omp(){}
  
  void go ()
  {
    //cout<<"====START Klimhu===="<<endl;
  	ControllerPHBU<double> ch(h);

//dynamic also possible
#pragma omp parallel for shared(ch) private(yy,xx) schedule (static, CHUNK)
    for(int64_t yy = ch.startHeight(); yy<ch.height();yy++)                                                                                                               
      {
	     for(int64_t xx = ch.startWidth();xx<ch.width(); xx++)
	     {
	     	ch(xx,yy) = ch(xx,yy+1);
	     }                                                                                                                                                                                           
      }             
  }

};

#include <KernelsOmp/Klimhu_omp.l2c>
