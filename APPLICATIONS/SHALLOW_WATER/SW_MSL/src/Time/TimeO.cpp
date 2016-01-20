#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>
#include <vector>
#include <chronometer.hpp>
#include <phaseoh.h>

typedef llcmcpp::Go Go;

using ::std::vector;
using ::std::cout;
using ::std::endl;

class TimeO:virtual public Go
{
public:
  int16_t iter;
  vector<Go *> phase;
  vector<PhaseOh *> phase_init;

  TimeO ():iter(0){}
  ~TimeO(){}

  void go ()
  {
    //cout<<"iter = "<<iter<<endl;
    //-----Time calculation                                                                                                                                     
    Chronometer chrono;
    chrono.start();
    //-----
    //first iteration                                                                                                                                         
    for(int j=0;j<phase.size();j++)
      {
        phase_init[j]->init_comm();
        phase[j]->go();
      }                                                                                                                                                                                              
    for(int i=1;i<iter;i++)
      for(int j=0;j<phase.size();j++)
        phase[j]->go();

    //-----Time calculation                                                                                                                                                                              
    chrono.stop();
    cout<<" Time TOTAL CALCULATION : "<<chrono.dureeCalcule()<<endl;
    //-----*/ 
  }

  void phaseCompute(Go * port)
  {
    phase.push_back(port);
  }

  void phaseInit(PhaseOh * port)
  {
    phase_init.push_back(port);
  }
  
};

#include <Time/TimeO.l2c>
