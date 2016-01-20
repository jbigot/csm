#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <structh.h>

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

class Driver:virtual public Go
{
public:
  Structh* structInit;
  Go* driverappGo;
  int64_t width;
  int64_t height;
  int32_t nb_proc;
  
  Driver ():width(0),height(0),nb_proc(0),structInit(0)
  {
    //cout<<"[Driver] creation"<<endl;
  }
  ~Driver()
  {
    //cout<<"[Driver] destruction"<<endl;
  }
  
  void go ()
  {
    if(structInit)
      structInit->init(width,height,nb_proc);
      
    if(driverappGo)
      driverappGo->go();
    
  }
  
};

#include <Driver/Driver.l2c>

