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

class Klimhr_omp:virtual public Go
{
public:
  Datah* h;

  Klimhr_omp(){}
  ~Klimhr_omp(){}
  
  void go ()
  {
    //cout<<"====START Klimhr===="<<endl;
  	ControllerPHBR<double> ch(h);

//dynamic also possible
#pragma omp parallel for shared(ch) private(yy,xx) schedule (static, CHUNK)
    for(int64_t yy = ch.startHeight(); yy<ch.height();yy++)                                                                                                               
      {
	     for(int64_t xx = ch.startWidth();xx<ch.width(); xx++)
	     {
	     	ch(xx,yy) = ch(xx-1,yy);
	     }                                                                                                                                                                                           
      }                   
  }

};

#include <KernelsOmp/Klimhr_omp.l2c>
