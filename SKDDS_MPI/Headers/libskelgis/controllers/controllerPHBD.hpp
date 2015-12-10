#ifndef CONTROLLERPHBD_H
#define CONTROLLERPHBD_H

#include "../data_structures/dmatrix_impl.hpp"

template<class T, int R,bool line=false> struct ControllerPHBD
{
private :
  skelgis::DMatrix_impl<T,R,false> * data;
  T * tab;
  int16_t border;
  int64_t wphb,hphb,swphb,shphb;
  int64_t w;
public:

  ControllerPHBD(skelgis::DMatrix_impl<T,R,false> * d):data(d)
  {
    //w = data->getWidth();
    tab = data->getData();
    swphb = data->getStartWPHBD();
    wphb = data->getWidthPHBD();
    shphb = data->getStartHPHBD();
    hphb = data->getHeightPHBD();
    w = data->getWidthLoc()+2*data->getBorder();
  }
  ~ControllerPHBD(){}

  void init(skelgis::DMatrix_impl<T,R,false> * d)
  {
    data = d;
    //w = data->getWidth();
    tab = data->getData();
    swphb = data->getStartWPHBD();
    wphb = data->getWidthPHBD();
    shphb = data->getStartHPHBD();
    hphb = data->getHeightPHBD();
    w = data->getWidthLoc()+2*data->getBorder();
  }

  inline T& operator()(int64_t x, int64_t y){return tab[x + w*y];}
  inline void set(int64_t x,int64_t y,T val){tab[x + w*y]=val;}
  inline int64_t startWidth(){return swphb;}
  inline int64_t startHeight(){return shphb;}
  inline int64_t width(){return wphb;}
  inline int64_t height(){return hphb;}
};

template<class T> struct ControllerPHBD<T,0,false>
{
private :
  skelgis::DMatrix_impl<T,0,false> * data;
  T * tab;
  int16_t border;
  int64_t wphb,hphb,swphb,shphb;
  int64_t w;
public:

  ControllerPHBD(skelgis::DMatrix_impl<T,0,false> * d):data(d)
  {
    //w = data->getWidth();
    tab = data->getData();
    swphb = data->getStartWPHBD();
    wphb = data->getWidthPHBD();
    shphb = data->getStartHPHBD();
    hphb = data->getHeightPHBD();
    w = data->getWidthLoc()+2*data->getBorder();
  }
  ~ControllerPHBD(){}

  void init(skelgis::DMatrix_impl<T,0,false> * d)
  {
    data = d;
    //w = data->getWidth();
    tab = data->getData();
    swphb = data->getStartWPHBD();
    wphb = data->getWidthPHBD();
    shphb = data->getStartHPHBD();
    hphb = data->getHeightPHBD();
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
