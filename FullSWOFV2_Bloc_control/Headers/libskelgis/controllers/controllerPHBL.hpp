#ifndef CONTROLLERPHBL_H
#define CONTROLLERPHBL_H

#include "../data_structures/dmatrix_impl.hpp"

template<class T, int R,bool line=true> struct ControllerPHBL
{
private :
  skelgis::DMatrix_impl<T,R,true> * data;
  T * tab;
  int16_t border;
  int64_t wphb,hphb,swphb,shphb;
  int64_t w;
public:

  ControllerPHBL(skelgis::DMatrix_impl<T,R,true> * d):data(d)
  {
    //w = data->getWidth();
    tab = data->getData();
    swphb = data->getStartWPHBL();
    wphb = data->getWidthPHBL();
    shphb = data->getStartHPHBL();
    hphb = data->getHeightPHBL();
    w = data->getWidthLoc()+2*data->getBorder();
  }
  ~ControllerPHBL(){}

  void init(skelgis::DMatrix_impl<T,R,true> * d)
  {
    data = d;
    //w = data->getWidth();
    tab = data->getData();
    swphb = data->getStartWPHBL();
    wphb = data->getWidthPHBL();
    shphb = data->getStartHPHBL();
    hphb = data->getHeightPHBL();
    w = data->getWidthLoc()+2*data->getBorder();
  }

  inline T& operator()(int64_t x, int64_t y){return tab[x + w*y];}
  inline void set(int64_t x,int64_t y,T val){tab[x + w*y]=val;}
  inline int64_t startWidth(){return swphb;}
  inline int64_t startHeight(){return shphb;}
  inline int64_t width(){return wphb;}
  inline int64_t height(){return hphb;}
};

template<class T> struct ControllerPHBL<T,0,true>
{
private :
  skelgis::DMatrix_impl<T,0,true> * data;
  T * tab;
  int16_t border;
  int64_t wphb,hphb,swphb,shphb;
  int64_t w;
public:

  ControllerPHBL(skelgis::DMatrix_impl<T,0,true> * d):data(d)
  {
    //w = data->getWidth();
    tab = data->getData();
    swphb = data->getStartWPHBL();
    wphb = data->getWidthPHBL();
    shphb = data->getStartHPHBL();
    hphb = data->getHeightPHBL();
    w = data->getWidthLoc()+2*data->getBorder();
  }
  ~ControllerPHBL(){}

  void init(skelgis::DMatrix_impl<T,0,true> * d)
  {
    data = d;
    //w = data->getWidth();
    tab = data->getData();
    swphb = data->getStartWPHBL();
    wphb = data->getWidthPHBL();
    shphb = data->getStartHPHBL();
    hphb = data->getHeightPHBL();
    w = data->getWidthLoc()+2*data->getBorder();
  }

  inline T& operator()(int64_t x, int64_t y){return tab[x + w*y];}
  inline void set(int64_t x,int64_t y,T val){tab[x + w*y]=val;}
  inline int64_t startWidth(){return swphb;}
  inline int64_t startHeight(){return shphb;}
  inline int64_t width(){return wphb;}
  inline int64_t height(){return hphb;}
};

template<class T,int R> struct ControllerPHBL<T,R,false>
{
private :
  skelgis::DMatrix_impl<T,R,false> * data;
  T * tab;
  int16_t border;
  int64_t wphb,hphb,swphb,shphb;
  int64_t w;
public:

  ControllerPHBL(skelgis::DMatrix_impl<T,R,false> * d):data(d)
  {
    //w = data->getWidth();
    tab = data->getData();
    swphb = data->getStartWPHBL();
    wphb = data->getWidthPHBL();
    shphb = data->getStartHPHBL();
    hphb = data->getHeightPHBL();
    w = data->getWidthLoc()+2*data->getBorder();
  }
  ~ControllerPHBL(){}

  void init(skelgis::DMatrix_impl<T,R,false> * d)
  {
    data = d;
    //w = data->getWidth();
    tab = data->getData();
    swphb = data->getStartWPHBL();
    wphb = data->getWidthPHBL();
    shphb = data->getStartHPHBL();
    hphb = data->getHeightPHBL();
    w = data->getWidthLoc()+2*data->getBorder();
  }

  inline T& operator()(int64_t x, int64_t y){return tab[x + w*y];}
  inline void set(int64_t x,int64_t y,T val){tab[x + w*y]=val;}
  inline int64_t startWidth(){return swphb;}
  inline int64_t startHeight(){return shphb;}
  inline int64_t width(){return wphb;}
  inline int64_t height(){return hphb;}
};

template<class T> struct ControllerPHBL<T,0,false>
{
private :
  skelgis::DMatrix_impl<T,0,false> * data;
  T * tab;
  int16_t border;
  int64_t wphb,hphb,swphb,shphb;
  int64_t w;
public:

  ControllerPHBL(skelgis::DMatrix_impl<T,0,false> * d):data(d)
  {
    //w = data->getWidth();
    tab = data->getData();
    swphb = data->getStartWPHBL();
    wphb = data->getWidthPHBL();
    shphb = data->getStartHPHBL();
    hphb = data->getHeightPHBL();
    w = data->getWidthLoc()+2*data->getBorder();
  }
  ~ControllerPHBL(){}

  void init(skelgis::DMatrix_impl<T,0,false> * d)
  {
    data = d;
    //w = data->getWidth();
    tab = data->getData();
    swphb = data->getStartWPHBL();
    wphb = data->getWidthPHBL();
    shphb = data->getStartHPHBL();
    hphb = data->getHeightPHBL();
    w = data->getWidthLoc()+2*data->getBorder();
  }

  inline T& operator()(int64_t x, int64_t y){return tab[x + w*y];}
  inline void set(int64_t x,int64_t y,T val){tab[x + w*y]=val;}
  inline int64_t startWidth(){return swphb;}
  inline int64_t startHeight(){return shphb;}
  inline int64_t width(){return wphb;}
  inline int64_t height(){return hphb;}
};

#endif
