#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <iterator.hpp>
#include <spaceh.h>
#include <mpi.h>
#include <vector>

typedef llcmcpp::Go Go;

using ::std::vector;
using ::std::cout;
using ::std::endl;

#define DATA1(x,y) data1[ (x) + width * (y) ]
#define DATA2(x,y) data2[ (x) + width * (y) ]

class Space:virtual public Go,virtual public Spaceh
{
public:
  Datah* data1SV;
  Datah* data2SV;
  double * data1;
  double * data2;
  int64_t width;
  int64_t widthLoc;
  int64_t heightLoc;

  Space ()
  {
  }
  ~Space()
  {
    cout<<"[Space] destruction"<<endl;
  }

  void init()
  {
    width = data1SV->getWidth();
    widthLoc = data1SV->getWidthLoc();
    heightLoc = data1SV->getHeightLoc();
  }
  
  void go ()
  {
    data1=data1SV->getData();
    data2=data2SV->getData();

    //it.init();
    /*for(it;it<=itEnd;++it)
      {
	int64_t ir = it.getRight();
	int64_t il = it.getLeft();
	int64_t iu = it.getUp();
	int64_t id = it.getDown();

	double a=0.05;
	double val = data1[it._rank];
	data2[it._rank] = (1-4*a)*val+a*(data1[ir]+data1[il]+data1[iu]+data1[id]);
	//data2[it._rank] = (1-4*a)*val+a*(r+l+u+d);
	//double res = (1-4*a)*val+a*(t->val+t->val+t->val+t->val)*//*(data1SV->getRight(it)+
				    data1SV->getLeft(it)+
				    data1SV->getUp(it)+
				    data1SV->getDown(it));
	//data2SV->set(it,res);
	}*/

    for(int64_t yy = 1; yy<=heightLoc;yy++)                                                                                                                                                      
      { 
	for(int64_t xx = 1 ;xx<=widthLoc ; xx++)
	  {
	    double a=0.05;                                                                                                                                                                            
	    double val = DATA1(xx,yy);
	    DATA2(xx,yy) = (1-4*a)*val+a*(DATA1(xx+1,yy)+DATA1(xx-1,yy)+DATA1(xx,yy-1)+DATA1(xx,yy+1));    
	  }                                                                                                                                                                                           
      }
    
    //exchange of pointers, the output becomes the input of the next iteration
    Datah* dataTMP = data1SV;
    data1SV=data2SV;
    data2SV=dataTMP;
  }

};

#include <Space/Space.l2c>
