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

class Klimhd_limud_limvd_omp:virtual public Go
{
public:
  Datah* h;
  Datah* u;
  Datah* v;

  Klimhd_limud_limvd_omp(){}
  ~Klimhd_limud_limvd_omp(){}
  
  void go ()
  {
    //cout<<"====START Klimhd_limud_limvd===="<<endl;
    ControllerPHBD<double> ch(h);
    ControllerPHBD<double> cu(u);
    ControllerPHBD<double> cv(v);

//dynamic also possible
#pragma omp parallel for shared(ch,cu,cv) private(yy,xx) schedule (static, CHUNK)
    for(int64_t yy = ch.startHeight(); yy<ch.height();yy++)                                                                                                               
      {
	     for(int64_t xx = ch.startWidth();xx<ch.width(); xx++)
	     {
	     	ch(xx,yy) = ch(xx,yy-1);
        cu(xx,yy) = -cu(xx,yy-1);
        cv(xx,yy) = cv(xx,yy-1);
	     }                                                                                                                                                                                           
      }                                                                                                                                                                                     
  }

};

#include <KernelsOmp/Klimhd_limud_limvd_omp.l2c>
