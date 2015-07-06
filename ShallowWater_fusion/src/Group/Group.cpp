#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>
#include <vector>

typedef llcmcpp::Go Go;

using ::std::vector;
using ::std::cout;
using ::std::endl;

class Group:virtual public Go
{
public:
  vector<Go *> kernel;

  Group(){}
  ~Group(){}

  void go ()
  {
    //cout<<"====START Group===="<<endl;
    for(int i=0;i<kernel.size();i++)
      kernel[i]->go();
  }

  void kernelCompute(Go * port)
  {
    kernel.push_back(port);
  }
  
};

#include <Group/Group.l2c>
