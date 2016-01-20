#ifndef APPLYLIST_H
#define APPLYLIST_H

#include "../../util/dmatrix_vector.hpp"

namespace skelgis{

  //================================================================================
  //Types of user functions
  //-------------------------------------------------------------------------------
  template <typename T,int R, typename T2, int R2,bool line=true> struct _ApplyList_Func
  //-------------------------------------------------------------------------------
  {
    virtual void operator()(DMatrix_vector<T,R>&,DMatrix_vector<T2,R2>&) const =0;
  };
  //-------------------------------------------------------------------------------
  template <typename T, typename T2, int R2> struct _ApplyList_Func<T,0,T2,R2>
  //-------------------------------------------------------------------------------
  {
    virtual void operator()(DMatrix_vector<T,0>&, DMatrix_vector<T2,R2>&) const =0;
  };
  //-------------------------------------------------------------------------------
  template <typename T,int R, typename T2,int R2> struct _ApplyList_Func<T,R,T2,R2,false>
  //-------------------------------------------------------------------------------
  {
    virtual void operator()(DMatrix_vector<T,R,false>&,DMatrix_vector<T2,R2,false>&) const =0;
  };
  //-------------------------------------------------------------------------------
  template <typename T, typename T2,int R2> struct _ApplyList_Func<T,0,T2,R2,false>
  //-------------------------------------------------------------------------------
  {
    virtual void operator()(DMatrix_vector<T,0,false>&, DMatrix_vector<T2,R2,false>&) const =0;
  };
  //-------------------------------------------------------------------------------
  //================================================================================

  //================================================================================
  //specializations of skeleton ApplyList
  //-------------------------------------------------------------------------------
  template<class T,int R,class T2, int R2,bool line=true,bool local=false> struct ApplyList{};
  //-------------------------------------------------------------------------------

  //-------------------------------------------------------------------------------
  template<class T,int R,class T2,int R2> struct ApplyList<T,R,T2,R2>
  //-------------------------------------------------------------------------------
  {
    static void apply(const _ApplyList_Func<T,R,T2,R2>& func,DMatrix_vector<T,R>& inputs, DMatrix_vector<T2,R2>& outputs)
    {
      for(int i=0;i<inputs.size();i++)
	{
	  DMatrix_impl<T,R> * m = inputs[i];
	  m->getBorders();
	}
      func(inputs,outputs);
    }
  };
  //-------------------------------------------------------------------------------

  //-------------------------------------------------------------------------------
  template<class T,class T2,int R2> struct ApplyList<T,0,T2,R2>
  //-------------------------------------------------------------------------------
  {
    inline static void apply(const _ApplyList_Func<T,0,T2,R2>& func,DMatrix_vector<T,0>& inputs, DMatrix_vector<T2,R2>& outputs){func(inputs,outputs);}
  };
  //-------------------------------------------------------------------------------

  //-------------------------------------------------------------------------------
  template<class T,int R,class T2,int R2> struct ApplyList<T,R,T2,R2,true,true>
  //-------------------------------------------------------------------------------
  {
    inline static void apply(const _ApplyList_Func<T,R,T2,R2>& func, DMatrix_vector<T,R>& inputs, DMatrix_vector<T2,R2>& outputs){func(inputs,outputs);}
  };
  //-------------------------------------------------------------------------------

  //-------------------------------------------------------------------------------
  template<class T,class T2,int R2> struct ApplyList<T,0,T2,R2,true,true>
  //-------------------------------------------------------------------------------
  {
    inline static void apply(const _ApplyList_Func<T,0,T2,R2>& func, DMatrix_vector<T,0>& inputs, DMatrix_vector<T2,R2>& outputs){func(inputs,outputs);}
  };
  //-------------------------------------------------------------------------------

  //================================================================================
  //if block division
  //================================================================================
  //specializations of skeleton ApplyUnary for line=false
  //================================================================================
  //-------------------------------------------------------------------------------
  template<class T,int R,class T2,int R2> struct ApplyList<T,R,T2,R2,false>
  //-------------------------------------------------------------------------------
  {
    static void apply(const _ApplyList_Func<T,R,T2,R2,false>& func,DMatrix_vector<T,R,false>& inputs, DMatrix_vector<T2,R2,false>& outputs)
    {
      for(int i=0;i<inputs.size();i++)
	{
	  DMatrix_impl<T,R,false> * m = inputs[i];
	  m->getBorders();
	}
      func(inputs,outputs);
    }
  };
  //-------------------------------------------------------------------------------

  //-------------------------------------------------------------------------------
  template<class T,class T2,int R2> struct ApplyList<T,0,T2,R2,false>
  //-------------------------------------------------------------------------------
  {
    inline static void apply(const _ApplyList_Func<T,0,T2,R2,false>& func,DMatrix_vector<T,0,false>& inputs, DMatrix_vector<T2,R2,false>& outputs){func(inputs,outputs);}
  };
  //-------------------------------------------------------------------------------

  //-------------------------------------------------------------------------------
  template<class T,int R,class T2,int R2> struct ApplyList<T,R,T2,R2,false,true>
  //-------------------------------------------------------------------------------
  {
    inline static void apply(const _ApplyList_Func<T,R,T2,R2,false>& func, DMatrix_vector<T,R,false>& inputs, DMatrix_vector<T2,R2,false>& outputs){func(inputs,outputs);}
  };
  //-------------------------------------------------------------------------------

  //-------------------------------------------------------------------------------
  template<class T,class T2,int R2> struct ApplyList<T,0,T2,R2,false,true>
  //-------------------------------------------------------------------------------
  {
    inline static void apply(const _ApplyList_Func<T,0,T2,R2,false>& func, DMatrix_vector<T,0,false>& inputs, DMatrix_vector<T2,R2,false>& outputs){func(inputs,outputs);}
  };
  //-------------------------------------------------------------------------------
  
}

#endif
