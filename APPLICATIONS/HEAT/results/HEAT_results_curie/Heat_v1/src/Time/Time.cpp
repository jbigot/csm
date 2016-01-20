#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <chronometer.hpp>
#include <spaceh.h>

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

class Time:virtual public Go
{
public:
  int16_t iter;
  Go* phaseGo;
  //Spaceh * sp;

  Time ():iter(0)
  {
    //cout<<"[Time] creation"<<endl;
  }
  ~Time()
  {
    //cout<<"[Time] destruction"<<endl;
  }

  void go ()
  {
    //cout<<"iter = "<<iter<<endl;
    //-----Time calculation                                                                                                                                     
    Chronometer chrono;
    chrono.start();
    //-----                                                                                                                                                                                              
    for(int i=0;i<iter;i++)
      phaseGo->go();

    //-----Time calculation                                                                                                                                                                              
    chrono.stop();
    cout<<" Time TOTAL CALCULATION : "<<chrono.dureeCalcule()<<endl;
    //-----*/ 
  }
  
};

#include <Time/Time.l2c>
