#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>
#include <vector>
#include <pthread.h>

typedef llcmcpp::Go Go;

using ::std::vector;
using ::std::cout;
using ::std::endl;

class Parallel:virtual public Go
{
public:
  vector<Go *> computations;

  Parallel(){}
  ~Parallel(){}

  void go ()
  {
    //cout<<"====START Parallel===="<<endl;
    pthread_t threads[computations.size()];
    int rc;
    for(int i=0;i<computations.size();i++)
      rc = pthread_create(&threads[i], NULL, launchComputation, computations[i]);
    for(int i=0;i<computations.size();i++)
      rc = pthread_join(threads[i], NULL);
  }

  void Compute(Go * port)
  {
    computations.push_back(port);
  }

  static void * launchComputation(void * computation)
  {
    ((Go *)computation)->go();
  }
  
};

#include <Parallel/Parallel.l2c>
