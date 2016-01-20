#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <structh.h>
#include <datah.h>
#include <kernelh.h>
#include <synch.h>
#include <chronometer.hpp>
#include <vector>
#include <mpi.h>

typedef llcmcpp::Go Go;

using ::std::vector;
using ::std::cout;
using ::std::endl;

class DriverApp:virtual public Go
{
public:
  Go* timeGo;
  vector<Go *> data;
  vector<Kernelh *> kernel;
  //vector<Synch *> sync;
      
  DriverApp (){}
  ~DriverApp()
  {
    //cout<<"[DriverApp] destruction"<<endl;
  }
  
  void go ()
  {
    for(int i=0;i<data.size();i++)
      data[i]->go();
    //for(int i=0;i<sync.size();i++)
    //sync[i]->init();

    timeGo->go();
  }

  /*void syncInit(Synch * port)
  {
    sync.push_back(port);
    }*/
  void dataCompute(Go* port)
  {
    data.push_back(port);
  }
  void kernelCompute(Kernelh * port)
  {
    kernel.push_back(port);
  }
};

#include <DriverApp/DriverApp.l2c>

