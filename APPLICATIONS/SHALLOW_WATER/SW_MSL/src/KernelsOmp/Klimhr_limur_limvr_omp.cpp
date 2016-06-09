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

class Klimhr_limur_limvr_omp:virtual public Go
{
public:
  Datah* h;
  Datah* u;
  Datah* v;

  Klimhr_limur_limvr_omp(){}
  ~Klimhr_limur_limvr_omp(){}
  
  void go ()
  {
    //cout<<"====START Klimhr_limur_limvr===="<<endl;
    ControllerPHBR<double> ch(h);
    ControllerPHBR<double> cu(u);
    ControllerPHBR<double> cv(v);
    
//dynamic also possible
#pragma omp parallel for shared(ch,cu,cv) private(yy,xx) schedule (static, CHUNK)
    for(int64_t yy = ch.startHeight(); yy<ch.height();yy++)                                                                                                               
      {
	     for(int64_t xx = ch.startWidth();xx<ch.width(); xx++)
	     {
        //std::cout<<"ch("<<xx<<","<<yy<<")="<<ch(xx,yy)<<std::endl;
	     	ch(xx,yy) = ch(xx-1,yy);
        cu(xx,yy) = -cu(xx-1,yy);
        cv(xx,yy) = cv(xx-1,yy);
	     }                                                                                                                                                                                           
      }                                                                                                                                                                                      
  }

};

#include <Kernels/Klimhr_limur_limvr_omp.l2c>
