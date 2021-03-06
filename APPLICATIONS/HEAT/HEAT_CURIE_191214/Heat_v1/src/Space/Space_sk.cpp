#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controller.hpp>
#include <spaceh.h>

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

class Space_sk:virtual public Go,virtual public Spaceh
{
public:
  Datah* data1SV;
  Datah* data2SV;
  //Controller<double> c1;
  //Controller<double> c2;

  Space_sk ()
  {
  }
  ~Space_sk()
  {
    //cout<<"[Space_sk] destruction"<<endl;
  }

  void init()
  {
    //c1.init(data1SV);
    //c2.init(data2SV);
  }
  
  void go ()
  {
    Controller<double> c1(data1SV);
    Controller<double> c2(data2SV);
    //start = c1.start();
    //height = c1.height();
    //width = c1.width();

    for(int64_t yy = c1.start(); yy<c1.height();yy++)                                                                                                               
      {
	for(int64_t xx = c1.start();xx<c1.width(); xx++)
	  {
	    double a=0.05;                                                                                                                                                                            
	    double val = c1(xx,yy);
	    c2(xx,yy) = (1-4*a)*val+a*(c1(xx+1,yy)+c1(xx-1,yy)+c1(xx,yy-1)+c1(xx,yy+1));
	  }                                                                                                                                                                                           
      }
    
    //exchange of pointers, the output becomes the input of the next iteration
    Datah* dataTMP = data1SV;
    data1SV=data2SV;
    data2SV=dataTMP;
  }

};

#include <Space/Space_sk.l2c>
