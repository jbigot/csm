#ifndef APPLY_UNARY_WITH_H
#define APPLY_UNARY_WITH_H

namespace skelgis{

  //================================================================================
  //Types of user functions
  //-------------------------------------------------------------------------------
  template <typename T,int R, typename T2,int R2,bool line=true> struct _ApplyUnaryWith_Func
  //-------------------------------------------------------------------------------
  {
    virtual void operator()(DMatrix <T,R>&, DMatrix<T2,R2>&,void *) const =0;
  };
  //-------------------------------------------------------------------------------
  template <typename T, typename T2,int R2> struct _ApplyUnaryWith_Func<T,0,T2,R2>
  //-------------------------------------------------------------------------------
  {
    virtual void operator()(DMatrix <T,0>&, DMatrix<T2,R2>&,void *) const =0;
  };
  //-------------------------------------------------------------------------------
  template <typename T,int R,typename T2,int R2> struct _ApplyUnaryWith_Func<T,R,T2,R2,false>
  //-------------------------------------------------------------------------------
  {
    virtual void operator()(DMatrix <T,R,false>&, DMatrix<T2,R2,false>&,void *) const =0;
  };
  //-------------------------------------------------------------------------------
  template <typename T, typename T2,int R2> struct _ApplyUnaryWith_Func<T,0,T2,R2,false>
  //-------------------------------------------------------------------------------
  {
    virtual void operator()(DMatrix <T,0,false>&, DMatrix<T2,R2,false>&,void *) const =0;
  };
  //-------------------------------------------------------------------------------
  //================================================================================

  //================================================================================
  //specializations of skeleton ApplyUnary
  //-------------------------------------------------------------------------------
  template<class T,int R,class T2,int R2,bool line=true,bool local=false> struct ApplyUnaryWith{};
  //-------------------------------------------------------------------------------

  //-------------------------------------------------------------------------------
  template<class T,int R,class T2,int R2> struct ApplyUnaryWith<T,R,T2,R2>
  //-------------------------------------------------------------------------------
  {
    //need an operator=(DMatrixw<...> m) in dmatrix.hpp
    /*inline static DMatrix<T2,R2> apply(const _ApplyUnaryWith_Func<T,R,T2,R2>& func, DMatrix<T,R>& m, void * object)
    {
      DMatrix<T2,R2> m2(m.getGlobalHeader(),0);
      m.getBorders();
      func(m,m2,object);
      return m2;
      }*/

    inline static void apply(const _ApplyUnaryWith_Func<T,R,T2,R2>& func, DMatrix<T,R>& m, DMatrix<T2,R2>& m2, void * object)
    {
      m.getBorders();
      func(m,m2,object);
    }
  };
  //-------------------------------------------------------------------------------

  //-------------------------------------------------------------------------------
  template<class T,class T2,int R2> struct ApplyUnaryWith<T,0,T2,R2>
  //-------------------------------------------------------------------------------
  {
    //need an operator=(DMatrixw<...> m) in dmatrix.hpp
    /*inline static DMatrix<T2,R2> apply(const _ApplyUnaryWith_Func<T,0,T2,R2>& func, DMatrix<T,0>& m, void * object)
    {
      DMatrix<T2,R2> m2(m.getGlobalHeader(),0);
      func(m,m2,object);
      return m2;
      }*/

    inline static void apply(const _ApplyUnaryWith_Func<T,0,T2,R2>& func, DMatrix<T,0>& m, DMatrix<T2,R2>& m2, void * object)
    {
      func(m,m2,object);
    }
  };
  //-------------------------------------------------------------------------------

  //-------------------------------------------------------------------------------
  template<class T,int R,class T2,int R2> struct ApplyUnaryWith<T,R,T2,R2,true,true>
  //-------------------------------------------------------------------------------
  {
    //need an operator=(DMatrixw<...> m) in dmatrix.hpp
    /*inline static DMatrix<T2,R2> apply(const _ApplyUnaryWith_Func<T,R,T2,R2>& func, DMatrix<T,R>& m, void * object)
    {
      DMatrix<T2,R2> m2(m.getGlobalHeader(),0);
      func(m,m2,object);
      return m2;
      }*/

    inline static void apply(const _ApplyUnaryWith_Func<T,R,T2,R2>& func, DMatrix<T,R>& m, DMatrix<T2,R2>& m2, void * object)
    {
      func(m,m2,object);
    }
  };
  //-------------------------------------------------------------------------------

  //-------------------------------------------------------------------------------
  template<class T,class T2,int R2> struct ApplyUnaryWith<T,0,T2,R2,true,true>
  //-------------------------------------------------------------------------------
  {
    //need an operator=(DMatrixw<...> m) in dmatrix.hpp
    /*inline static DMatrix<T2,R2> apply(const _ApplyUnaryWith_Func<T,0,T2,R2>& func, DMatrix<T,0>& m, void * object)
    {
      DMatrix<T2,R2> m2(m.getGlobalHeader(),0);
      func(m,m2,object);
      return m2;
      }*/

    inline static void apply(const _ApplyUnaryWith_Func<T,0,T2,R2>& func, DMatrix<T,0>& m, DMatrix<T2,R2>& m2, void * object)
    {
      func(m,m2,object);
    }
  };
  //-------------------------------------------------------------------------------

  //================================================================================
  //if block division
  //================================================================================
  //specializations of skeleton ApplyUnary for line=false
  //================================================================================
  //-------------------------------------------------------------------------------
  template<class T,int R,class T2,int R2> struct ApplyUnaryWith<T,R,T2,R2,false>
  //-------------------------------------------------------------------------------
  {
    //need an operator=(DMatrixw<...> m) in dmatrix.hpp
    /*inline static DMatrix<T2,R2,false> apply(const _ApplyUnaryWith_Func<T,R,T2,R2,false>& func, DMatrix<T,R,false>& m, void * object)
    {
      DMatrix<T2,R2,false> & m2(m.getGlobalHeader(),0);
      m.getBorders();
      func(m,m2,object);
      return m2;
      }*/

    inline static void apply(const _ApplyUnaryWith_Func<T,R,T2,R2,false>& func, DMatrix<T,R,false>& m, DMatrix<T2,R2,false>& m2, void * object)
    {
      m.getBorders();
      func(m,m2,object);
    }
  };
  //-------------------------------------------------------------------------------

  //-------------------------------------------------------------------------------
  template<class T,class T2,int R2> struct ApplyUnaryWith<T,0,T2,R2,false>
  //-------------------------------------------------------------------------------
  {
    //need an operator=(DMatrixw<...> m) in dmatrix.hpp
    /*inline static DMatrix<T2,R2,false> apply(const _ApplyUnaryWith_Func<T,0,T2,R2,false>& func, DMatrix<T,0,false>& m, void * object)
    {
      DMatrix<T2,R2,false> & m2(m.getGlobalHeader(),0);
      func(m,m2,object);
      return m2;
      }*/

    inline static void apply(const _ApplyUnaryWith_Func<T,0,T2,R2,false>& func, DMatrix<T,0,false>& m, DMatrix<T2,R2,false>& m2, void * object)
    {
      func(m,m2,object);
    }
  };
  //-------------------------------------------------------------------------------

  //-------------------------------------------------------------------------------
  template<class T,int R,class T2,int R2> struct ApplyUnaryWith<T,R,T2,R2,false,true>
  //-------------------------------------------------------------------------------
  {
    //need an operator=(DMatrixw<...> m) in dmatrix.hpp
    /*inline static DMatrix<T2,R2,false> apply(const _ApplyUnaryWith_Func<T,R,T2,R2,false>& func, DMatrix<T,R,false>& m, void * object)
    {
      DMatrix<T2,R2,false> m2(m.getGlobalHeader(),0);
      func(m,m2,object);
      return m2;
      }*/

    inline static void apply(const _ApplyUnaryWith_Func<T,R,T2,R2,false>& func, DMatrix<T,R,false>& m, DMatrix<T2,R2,false>& m2, void * object)
    {
      func(m,m2,object);
    }
  };
  //-------------------------------------------------------------------------------

  //-------------------------------------------------------------------------------
  template<class T,class T2,int R2> struct ApplyUnaryWith<T,0,T2,R2,false,true>
  //-------------------------------------------------------------------------------
  {
    //need an operator=(DMatrixw<...> m) in dmatrix.hpp
    /*inline static DMatrix<T2,R2,false> apply(const _ApplyUnaryWith_Func<T,0,T2,R2,false>& func, DMatrix<T,0,false>& m, void * object)
    {
      DMatrix<T2,R2,false> m2(m.getGlobalHeader(),0);
      func(m,m2,object);
      return m2;
      }*/

    inline static void apply(const _ApplyUnaryWith_Func<T,0,T2,R2,false>& func, DMatrix<T,0,false>& m, DMatrix<T2,R2,false>& m2, void * object)
    {
      func(m,m2,object);
    }
  };
  //-------------------------------------------------------------------------------

}

#endif
