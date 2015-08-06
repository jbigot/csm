#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>
#include <vector>
#include <pthread.h>

typedef llcmcpp::Go Go;

using ::std::vector;
using ::std::cout;
using ::std::endl;

class GroupThread:virtual public Go
{
public:
  vector<Go *> kernel;

  GroupThread(){}
  ~GroupThread(){}

  void go ()
  {
    //cout<<"====START GroupThread===="<<endl;
    pthread_t threads[kernel.size()];
    int rc;
    for(int i=0;i<kernel.size();i++)
      rc = pthread_create(&threads[i], NULL, launchKernel, kernel[i]);
    for(int i=0;i<kernel.size();i++)
      rc = pthread_join(threads[i], NULL);
  }

  void kernelCompute(Go * port)
  {
    kernel.push_back(port);
  }

  static void * launchKernel(void * kernel)
  {
    ((Go *)kernel)->go();
  }
  
};

#include <Group/GroupThread.l2c>
