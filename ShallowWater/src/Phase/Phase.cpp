#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>
#include <vector>
#include <datah.h>

typedef llcmcpp::Go Go;

using ::std::vector;
using ::std::cout;
using ::std::endl;

class Phase:virtual public Go
{
public:
  vector<Datah *> data;
  vector<Go *> group;

  Phase (){}
  ~Phase(){}

  void go ()
  {
    //cout<<"====START Phase===="<<endl;
    //cout<<"updates "<<n<<endl; 
    for(int i=0;i<data.size();i++)
      data[i]->update();
    //cout<<"Go space "<<n<<endl;
    for(int i=0;i<group.size();i++)
      group[i]->go();
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

#include <Phase/Phase.l2c>
