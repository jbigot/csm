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

class Klimhl_omp:virtual public Go
{
public:
  Datah* h;

  Klimhl_omp(){}
  ~Klimhl_omp(){}
  
  void go ()
  {
    //cout<<"====START Klimhl===="<<endl;
  	ControllerPHBL<double> ch(h);

    int64_t xx,yy;

//dynamic also possible
#pragma omp parallel for shared(ch) private(yy,xx) schedule (static, CHUNK)
    for(yy = ch.startHeight(); yy<ch.height();yy++)                                                                                                               
      {
	     for(xx = ch.startWidth();xx<ch.width(); xx++)
	     {
	     	ch(xx,yy) = ch(xx+1,yy);
	     }                                                                                                                                                                                           
      }           
  }

};

#include <KernelsOmp/Klimhl_omp.l2c>
