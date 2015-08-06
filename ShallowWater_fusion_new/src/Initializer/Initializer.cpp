#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <inith.h>

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

class Initializer:virtual public Inith
{
public:
  //double init_val;

  //-------------------------------------------------------------------------------
  Initializer(){}//:init_val(0)
  //-------------------------------------------------------------------------------
  ~Initializer(){}
  //-------------------------------------------------------------------------------
  
  //-------------------------------------------------------------------------------
  void initData(double* data,int64_t size,double init_val)
  //-------------------------------------------------------------------------------
  {
    for(int64_t i=0;i<size;i++)
      data[i]=init_val;
  }
  //-------------------------------------------------------------------------------

};

#include<Initializer/Initializer.l2c>
