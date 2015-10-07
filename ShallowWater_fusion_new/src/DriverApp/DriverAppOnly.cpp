#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <structh.h>
#include <datah.h>
#include <chronometer.hpp>
#include <vector>
#include <mpi.h>

typedef llcmcpp::Go Go;

using ::std::vector;
using ::std::cout;
using ::std::endl;

#define D1(x,y) d1[ (x) + width * (y) ]
#define D2(x,y) d2[ (x) + width * (y) ]

class DriverAppOnly:virtual public Go
{
public:
  Datah* dh1;
  Datah* dh2;
  vector<Go *> data;

  double * d1;
  double * d2;

  int64_t width;
  int64_t widthLoc;
  int64_t heightLoc;
      
  DriverAppOnly (){}
  ~DriverAppOnly()
  {
    cout<<"[DriverAppOnly] destruction"<<endl;
  }
  
  void go ()
  {
    for(int i=0;i<data.size();i++)
      data[i]->go();

    dh1->setMiddle(1);

    dh1->getWidth(width);
    dh1->getWidthLoc(widthLoc);
    dh1->getHeightLoc(heightLoc);

    //-----Time calculation                                                                                                                                     
    Chronometer chrono;
    chrono.start();
    //----- 
    for(int i=0;i<500;i++)
      {
	// dh1->update();
	// dh2->update();
	   dh1->getData(d1);
	   dh2->getData(d2);

	for(int64_t yy = 1; yy<1+heightLoc;yy++)
	  {
	    for(int64_t xx = 1 ;xx<1+widthLoc ; xx++)
	      {
		double a=0.05;                                                                                                                                                                          
		double val = D1(xx,yy);                                                                                                                                                              
		D2(xx,yy) = (1-4*a)*val+a*(D1(xx+1,yy)+D1(xx-1,yy)+D1(xx,yy-1)+D1(xx,yy+1));
	      }
	  }
	//exchange of pointers, the output becomes the input of the next iteration
	Datah* dataTMP = dh1;
	dh1=dh2;
	dh2=dataTMP;
	//---
      }
    
    //-----Time calculation                                                                                                                                                                              
    chrono.stop();
    cout<<" Time TOTAL CALCULATION : "<<chrono.dureeCalcule()<<endl;
    //----- 
    
    /*iterator it = dh1->begin();
    iterator itEnd = dh1->end();
    std::cout<<"End result ="<<std::endl;
    for(it; it<=itEnd; ++it)
      std::cout<<dh1->get(it)<<" ";
    std::cout<<std::endl;
    */
  }

  void dataCompute(Go* port)
  {
    data.push_back(port);
  }
};

#include <DriverApp/DriverAppOnly.l2c>

