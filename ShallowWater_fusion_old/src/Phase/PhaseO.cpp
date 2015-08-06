#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>
#include <vector>
#include <datah.h>
#include <phaseoh.h>

typedef llcmcpp::Go Go;

using ::std::vector;
using ::std::cout;
using ::std::endl;

class PhaseO:virtual public Go,virtual public PhaseOh
{
public:
  vector<Datah *> data;
  vector<Go *> group;

  PhaseO (){}
  ~PhaseO(){}

  void init_comm()
  {
    for(int i=0;i<data.size();i++)
      data[i]->start_comm();
  }

  void go ()
  {
    //cout<<"====START PhaseO===="<<endl;
    //cout<<"Go space "<<n<<endl;
    for(int i=0;i<data.size();i++)
      data[i]->end_comm();
    for(int i=0;i<group.size();i++)
      group[i]->go();
    for(int i=0;i<data.size();i++)
      data[i]->start_comm();
  }

  void dataCompute(Datah * port)
  {
    data.push_back(port);
  }
  void groupCompute(Go * port)
  {
    group.push_back(port);
  }
  
};

#include <Phase/PhaseO.l2c>
