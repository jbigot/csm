#ifndef APPLY_BINARY_WITH_H
#define APPLY_BINARY_WITH_H

namespace skelgis{

  //================================================================================
  //Types of user functions
  //-------------------------------------------------------------------------------
  template <typename T,int R, typename T2, typename T3,int R3,bool line=true> struct _ApplyBinaryWith_Func
  //-------------------------------------------------------------------------------
  {
    virtual void operator()(DMatrix <T,R>&, DMatrix<T2,R>&, DMatrix<T3,R3>&,void *) const =0;
  };
  //-------------------------------------------------------------------------------
  template <typename T, typename T2, typename T3,int R3> struct _ApplyBinaryWith_Func<T,0,T2,T3,R3>
  //-------------------------------------------------------------------------------
  {
    virtual void operator()(DMatrix <T,0>&, DMatrix<T2,0>&,DMatrix<T3,R3>&,void *) const =0;
  };
  //-------------------------------------------------------------------------------
  template <typename T,int R, typename T2, typename T3,int R3> struct _ApplyBinaryWith_Func<T,R,T2,T3,R3,false>
  //-------------------------------------------------------------------------------
  {
    virtual void operator()(DMatrix <T,R,false>&, DMatrix<T2,R,false>&, DMatrix<T3,R3,false>&,void *) const =0;
  };
  //-------------------------------------------------------------------------------
  template <typename T, typename T2, typename T3,int R3> struct _ApplyBinaryWith_Func<T,0,T2,T3,R3,false>
  //-------------------------------------------------------------------------------
  {
    virtual void operator()(DMatrix <T,0,false>&, DMatrix<T2,0,false>&,DMatrix<T3,R3,false>&,void *) const =0;
  };
  //-------------------------------------------------------------------------------
  //================================================================================

  //================================================================================
  //specializations of skeleton ApplyBinary
  //-------------------------------------------------------------------------------
  template<class T,int R,class T2,class T3,int R3,bool line=true,bool local=false> struct ApplyBinaryWith{};
  //-------------------------------------------------------------------------------

  //-------------------------------------------------------------------------------
  template<class T,int R,class T2,class T3,int R3> struct ApplyBinaryWith<T,R,T2,T3,R3>
  //-------------------------------------------------------------------------------
  {
    //need an operator=(DMatrixw<...> m) in dmatrix.hpp
    /*inline static DMatrix<T3,R3> apply(const _ApplyBinaryWith_Func<T,R,T2,T3,R3>& func, DMatrix<T,R>& m, DMatrix<T2,R>& m2,void * object)
    {
      DMatrix<T3,R3> m3(m.getGlobalHeader(),0);
      m.getBorders();
      m2.getBorders();
      func(m,m2,m3,object);
      return m3;
      }*/

    inline static void apply(const _ApplyBinaryWith_Func<T,R,T2,T3,R3>& func, DMatrix<T,R>& m, DMatrix<T2,R>& m2, DMatrix<T3,R3>& m3,void * object)
    {
      m.getBorders();
      m2.getBorders();
      func(m,m2,m3,object);
    }
  };
  //-------------------------------------------------------------------------------

  //-------------------------------------------------------------------------------
  template<class T,class T2,class T3,int R3> struct ApplyBinaryWith<T,0,T2,T3,R3>
  //-------------------------------------------------------------------------------
  {
    //need an operator=(DMatrixw<...> m) in dmatrix.hpp
    /*inline static DMatrix<T3,R3> apply(const _ApplyBinaryWith_Func<T,0,T2,T3,R3>& func, DMatrix<T,0>& m, DMatrix<T2,0>& m2,void * object)
    {
      DMatrix<T3,R3> m3(m.getGlobalHeader(),0);
      func(m,m2,m3,object);
      return m3;
      }*/

    inline static void apply(const _ApplyBinaryWith_Func<T,0,T2,T3,R3>& func, DMatrix<T,0>& m, DMatrix<T2,0>& m2, DMatrix<T3,R3>& m3,void * object)
    {
      func(m,m2,m3,object);
    }
  };
  //-------------------------------------------------------------------------------

  //-------------------------------------------------------------------------------
  template<class T,int R,class T2,class T3,int R3> struct ApplyBinaryWith<T,R,T2,T3,R3,true,true>
  //-------------------------------------------------------------------------------
  {
    //need an operator=(DMatrixw<...> m) in dmatrix.hpp
    /*inline static DMatrix<T3,R3> apply(const _ApplyBinaryWith_Func<T,R,T2,T3,R3>& func, DMatrix<T,R>& m, DMatrix<T2,R>& m2,void * object)
    {
      DMatrix<T3,R3> m3(m.getGlobalHeader(),0);
      func(m,m2,m3,object);
      return m3;
      }*/

    inline static void apply(const _ApplyBinaryWith_Func<T,R,T2,T3,R3>& func, DMatrix<T,R>& m, DMatrix<T2,R>& m2, DMatrix<T3,R3>& m3,void * object)
    {
      func(m,m2,m3,object);
    }
  };
  //-------------------------------------------------------------------------------

  //-------------------------------------------------------------------------------
  template<class T,class T2,class T3,int R3> struct ApplyBinaryWith<T,0,T2,T3,R3,true,true>
  //-------------------------------------------------------------------------------
  {
    //need an operator=(DMatrixw<...> m) in dmatrix.hpp
    /*inline static DMatrix<T3,R3> apply(const _ApplyBinaryWith_Func<T,0,T2,T3,R3>& func, DMatrix<T,0>& m, DMatrix<T2,0>& m2,void * object)
    {
      DMatrix<T3,R3> m3(m.getGlobalHeader(),0);
      func(m,m2,m3,object);
      return m3;
      }*/

    inline static void apply(const _ApplyBinaryWith_Func<T,0,T2,T3,R3>& func, DMatrix<T,0>& m, DMatrix<T2,0>& m2, DMatrix<T3,R3>& m3,void * object)
    {
      func(m,m2,m3,object);
    }
  };
  //-------------------------------------------------------------------------------

  //================================================================================
  //if block division
  //================================================================================
  //specializations of skeleton ApplyUnary for line=false
  //================================================================================
  //-------------------------------------------------------------------------------
  template<class T,int R,class T2,class T3,int R3> struct ApplyBinaryWith<T,R,T2,T3,R3,false>
  //-------------------------------------------------------------------------------
  {
    //need an operator=(DMatrixw<...> m) in dmatrix.hpp
    /*inline static DMatrix<T3,R3,false> apply(const _ApplyBinaryWith_Func<T,R,T2,T3,R3,false>& func, DMatrix<T,R,false>& m, DMatrix<T2,R,false>& m2,void * object)
    {
      DMatrix<T3,R3,false> m3(m.getGlobalHeader(),0);
      m.getBorders();
      m2.getBorders();
      func(m,m2,m3,object);
      return m3;
      }*/

    inline static void apply(const _ApplyBinaryWith_Func<T,R,T2,T3,R3,false>& func, DMatrix<T,R,false>& m, DMatrix<T2,R,false>& m2, DMatrix<T3,R3,false>& m3,void * object)
    {
      m.getBorders();
      m2.getBorders();
      func(m,m2,m3,object);
    }
  };
  //-------------------------------------------------------------------------------

  //-------------------------------------------------------------------------------
  template<class T,class T2,class T3,int R3> struct ApplyBinaryWith<T,0,T2,T3,R3,false>
  //-------------------------------------------------------------------------------
  {
    //need an operator=(DMatrixw<...> m) in dmatrix.hpp
    /*inline static DMatrix<T3,R3,false> apply(const _ApplyBinaryWith_Func<T,0,T2,T3,R3,false>& func, DMatrix<T,0,false>& m, DMatrix<T2,0,false>& m2,void * object)
    {
      DMatrix<T3,R3,false> m3(m.getGlobalHeader(),0);
      func(m,m2,m3,object);
      return m3;
      }*/

    inline static void apply(const _ApplyBinaryWith_Func<T,0,T2,T3,R3,false>& func, DMatrix<T,0,false>& m, DMatrix<T2,0,false>& m2, DMatrix<T3,R3,false>& m3,void * object)
    {
      func(m,m2,m3,object);
    }
  };
  //-------------------------------------------------------------------------------

  //-------------------------------------------------------------------------------
  template<class T,int R,class T2,class T3,int R3> struct ApplyBinaryWith<T,R,T2,T3,R3,false,true>
  //-------------------------------------------------------------------------------
  {
    //need an operator=(DMatrixw<...> m) in dmatrix.hpp
    /*inline static DMatrix<T3,R3,false> apply(const _ApplyBinaryWith_Func<T,R,T2,T3,R3,false>& func, DMatrix<T,R,false>& m, DMatrix<T2,R,false>& m2,void * object)
    {
      DMatrix<T3,R3,false> m3(m.getGlobalHeader(),0);
      func(m,m2,m3,object);
      return m3;
      }*/

    inline static void apply(const _ApplyBinaryWith_Func<T,R,T2,T3,R3,false>& func, DMatrix<T,R,false>& m, DMatrix<T2,R,false>& m2, DMatrix<T3,R3,false>& m3,void * object)
    {
      func(m,m2,m3,object);
    }
  };
  //-------------------------------------------------------------------------------

  //-------------------------------------------------------------------------------
  template<class T,class T2,class T3,int R3> struct ApplyBinaryWith<T,0,T2,T3,R3,false,true>
  //-------------------------------------------------------------------------------
  {
    //need an operator=(DMatrixw<...> m) in dmatrix.hpp
    /*inline static DMatrix<T3,R3,false> apply(const _ApplyBinaryWith_Func<T,0,T2,T3,R3,false>& func, DMatrix<T,0,false>& m, DMatrix<T2,0,false>& m2,void * object)
    {
      DMatrix<T3,R3,false> m3(m.getGlobalHeader(),0);
      func(m,m2,m3,object);
      return m3;
      }*/

    inline static void apply(const _ApplyBinaryWith_Func<T,0,T2,T3,R3,false>& func, DMatrix<T,0,false>& m, DMatrix<T2,0,false>& m2, DMatrix<T3,R3,false>& m3,void * object)
    {
      func(m,m2,m3,object);
    }
  };
  //-------------------------------------------------------------------------------

}

#endif
