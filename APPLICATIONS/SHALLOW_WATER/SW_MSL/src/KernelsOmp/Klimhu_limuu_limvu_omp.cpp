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

class Klimhu_limuu_limvu_omp:virtual public Go
{
public:
  Datah* h;
  Datah* u;
  Datah* v;

  Klimhu_limuu_limvu_omp(){}
  ~Klimhu_limuu_limvu_omp(){}
  
  void go ()
  {
    //cout<<"====START Klimhu_limuu_limvu===="<<endl;
    ControllerPHBU<double> ch(h);
    ControllerPHBU<double> cu(u);
    ControllerPHBU<double> cv(v);

    int64_t xx,yy;

//dynamic also possible
#pragma omp parallel for shared(ch,cu,cv) private(yy,xx) schedule (static, CHUNK)
    for(yy = ch.startHeight(); yy<ch.height();yy++)                                                                                                               
      {
	     for(xx = ch.startWidth();xx<ch.width(); xx++)
	     {
	     	ch(xx,yy) = ch(xx,yy+1);
        cu(xx,yy) = -cu(xx,yy+1);
        cv(xx,yy) = cv(xx,yy+1);
	     }                                                                                                                                                                                           
      }                                                                                                                                                                                 
  }

};

#include <KernelsOmp/Klimhu_limuu_limvu_omp.l2c>
