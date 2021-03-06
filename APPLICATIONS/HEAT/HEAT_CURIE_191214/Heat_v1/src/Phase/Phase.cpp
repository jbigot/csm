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
  vector<Go *> space;
  int n;

  Phase ()
  {
    n=0;
  }
  ~Phase()
  {
    //cout<<"[Phase] destruction"<<endl;
  }

  void go ()
  {
    //cout<<"updates "<<n<<endl; 
    for(int i=0;i<data.size();i++)
      data[i]->update();
    //cout<<"Go space "<<n<<endl;
    for(int i=0;i<space.size();i++)
      space[i]->go();
    n++;
  }

  void dataCompute(Datah * port)
  {
    data.push_back(port);
  }
  void spaceCompute(Go * port)
  {
    space.push_back(port);
  }
  
};

#include <Phase/Phase.l2c>
