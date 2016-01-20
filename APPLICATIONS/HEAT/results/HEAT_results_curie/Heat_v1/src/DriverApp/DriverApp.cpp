#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <structh.h>
#include <datah.h>
#include <spaceh.h>
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
  vector<Spaceh *> sp;
  //Datah * dh1;
      
  DriverApp (){}
  ~DriverApp()
  {
    //cout<<"[DriverApp] destruction"<<endl;
  }
  
  void go ()
  {
    for(int i=0;i<data.size();i++)
      data[i]->go();

    //dh1->setMiddle(1);

    for(int i=0;i<sp.size();i++)
      sp[i]->init();

    timeGo->go();

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
  void spaceCompute(Spaceh * port)
  {
    sp.push_back(port);
  }
};

#include <DriverApp/DriverApp.l2c>

