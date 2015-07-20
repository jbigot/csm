#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>
#include <vector>
#include <chronometer.hpp>

typedef llcmcpp::Go Go;

using ::std::vector;
using ::std::cout;
using ::std::endl;

class Time:virtual public Go
{
public:
  int16_t iter;
  vector<Go *> phase;

  Time ():iter(0){}
  ~Time(){}

  void go ()
  {
    //cout<<"iter = "<<iter<<endl;
    //-----Time calculation                                                                                                                                     
    Chronometer chrono;
    chrono.start();
    //-----                                                                                                                                                                                              
    for(int i=0;i<iter;i++)
    {
      //std::cout<<"========= ITERATION "<<i<<"==========="<<std::endl;
      for(int j=0;j<phase.size();j++)
        phase[j]->go();
    }

    //-----Time calculation                                                                                                                                                                              
    chrono.stop();
    cout<<" Time TOTAL CALCULATION : "<<chrono.dureeCalcule()<<endl;
    //-----*/ 
  }

  void phaseCompute(Go * port)
  {
    phase.push_back(port);
  }
  
};

#include <Time/Time.l2c>
