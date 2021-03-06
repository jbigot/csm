#ifndef APPLYLISTWITH_H
#define APPLYLISTWITH_H

#include <vector>

namespace skelgis{

  //================================================================================
  //Types of user functions
  //-------------------------------------------------------------------------------
  struct _ApplyListWith_Func
  //-------------------------------------------------------------------------------
  {
    virtual void operator()(std::vector<Data *>&,std::vector<Data *>&,void *) const =0;
  };
  //================================================================================

  //================================================================================
  //specializations of skeleton ApplyList

  //-------------------------------------------------------------------------------
  struct ApplyListWith
  //-------------------------------------------------------------------------------
  {
    static void apply(const _ApplyListWith_Func& func, std::vector<Data *>& inputs, std::vector<Data *>& outputs,void * object)
    {
      for(int i=0;i<inputs.size();i++)
	{
	  Data * m = inputs[i];
	  m->update();
	}
      func(inputs,outputs,object);
    }
  };
  //-------------------------------------------------------------------------------
}

#endif
