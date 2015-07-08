#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>
#include <vector>
#include <datah.h>

typedef llcmcpp::Go Go;

using ::std::vector;
using ::std::cout;
using ::std::endl;

class Sync:virtual public Go
{
public:
  vector<Datah *> data;

  Sync (){}
  ~Sync(){}

  void go ()
  {
    //cout<<"====START Sync===="<<endl;
    //cout<<"updates "<<n<<endl; 
    for(int i=0;i<data.size();i++)
      data[i]->update();
  }

  void dataCompute(Datah * port)
  {
    data.push_back(port);
  }
  
};

#include <Sync/Sync.l2c>
