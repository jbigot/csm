#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>
#include <vector>
#include <datah.h>

#include "../rdtsc.h"

typedef llcmcpp::Go Go;

using ::std::vector;
using ::std::cout;
using ::std::endl;

class Sequence:virtual public Go
{
public:
  vector<Go *> computations;

  Sequence (){}
  ~Sequence(){}

  void go ()
  {
    //cout<<"====START Sequence===="<<endl;
    for(int i=0;i<computations.size();i++)
      {
	//unsigned long long a,b;
	//a=rdtsc();
	computations[i]->go();
	//b=rdtsc();
	//std::cout<<"go in  sequence = "<<b-a<<std::endl;
      }
  }
  void Compute(Go * port)
  {
    computations.push_back(port);
  }
  
};

#include <Sequence/Sequence.l2c>
