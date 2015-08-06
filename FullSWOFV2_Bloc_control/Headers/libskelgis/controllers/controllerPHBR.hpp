#ifndef CONTROLLERPHBR_H
#define CONTROLLERPHBR_H

#include "../data_structures/dmatrix_impl.hpp"

template<class T, int R,bool line=true> struct ControllerPHBR
{
private :
  skelgis::DMatrix_impl<T,R,true> * data;
  T * tab;
  int16_t border;
  int64_t wphb,hphb,swphb,shphb;
  int64_t w;
public:

  ControllerPHBR(skelgis::DMatrix_impl<T,R,true> * d):data(d)
  {
    w = data->getWidth();
    tab = data->getData();
    swphb = data->getStartWPHBR();
    wphb = data->getWidthPHBR();
    shphb = data->getStartHPHBR();
    hphb = data->getHeightPHBR();
  }
  ~ControllerPHBR(){}

  void init(skelgis::DMatrix_impl<T,R,true> * d)
  {
    data = d;
    w = data->getWidth();
    tab = data->getData();
    swphb = data->getStartWPHBR();
    wphb = data->getWidthPHBR();
    shphb = data->getStartHPHBR();
    hphb = data->getHeightPHBR();
  }

  inline T& operator()(int64_t x, int64_t y){return tab[x + w*y];}
  inline void set(int64_t x,int64_t y,T val){tab[x + w*y]=val;}
  inline int64_t startWidth(){return swphb;}
  inline int64_t startHeight(){return shphb;}
  inline int64_t width(){return wphb;}
  inline int64_t height(){return hphb;}
};

template<class T> struct ControllerPHBR<T,0,true>
{
private :
  skelgis::DMatrix_impl<T,0,true> * data;
  T * tab;
  int16_t border;
  int64_t wphb,hphb,swphb,shphb;
  int64_t w;
public:

  ControllerPHBR(skelgis::DMatrix_impl<T,0,true> * d):data(d)
  {
    w = data->getWidth();
    tab = data->getData();
    swphb = data->getStartWPHBR();
    wphb = data->getWidthPHBR();
    shphb = data->getStartHPHBR();
    hphb = data->getHeightPHBR();
  }
  ~ControllerPHBR(){}

  void init(skelgis::DMatrix_impl<T,0,true> * d)
  {
    data = d;
    w = data->getWidth();
    tab = data->getData();
    swphb = data->getStartWPHBR();
    wphb = data->getWidthPHBR();
    shphb = data->getStartHPHBR();
    hphb = data->getHeightPHBR();
  }

  inline T& operator()(int64_t x, int64_t y){return tab[x + w*y];}
  inline void set(int64_t x,int64_t y,T val){tab[x + w*y]=val;}
  inline int64_t startWidth(){return swphb;}
  inline int64_t startHeight(){return shphb;}
  inline int64_t width(){return wphb;}
  inline int64_t height(){return hphb;}
};

template<class T, int R> struct ControllerPHBR<T,R,false>
{
private :
  skelgis::DMatrix_impl<T,R,false> * data;
  T * tab;
  int16_t border;
  int64_t wphb,hphb,swphb,shphb;
  int64_t w;
public:

  ControllerPHBR(skelgis::DMatrix_impl<T,R,false> * d):data(d)
  {
    w = data->getWidth();
    tab = data->getData();
    swphb = data->getStartWPHBR();
    wphb = data->getWidthPHBR();
    shphb = data->getStartHPHBR();
    hphb = data->getHeightPHBR();
  }
  ~ControllerPHBR(){}

  void init(skelgis::DMatrix_impl<T,R,false> * d)
  {
    data = d;
    w = data->getWidth();
    tab = data->getData();
    swphb = data->getStartWPHBR();
    wphb = data->getWidthPHBR();
    shphb = data->getStartHPHBR();
    hphb = data->getHeightPHBR();
  }

  inline T& operator()(int64_t x, int64_t y){return tab[x + w*y];}
  inline void set(int64_t x,int64_t y,T val){tab[x + w*y]=val;}
  inline int64_t startWidth(){return swphb;}
  inline int64_t startHeight(){return shphb;}
  inline int64_t width(){return wphb;}
  inline int64_t height(){return hphb;}
};

template<class T> struct ControllerPHBR<T,0,false>
{
private :
  skelgis::DMatrix_impl<T,0,false> * data;
  T * tab;
  int16_t border;
  int64_t wphb,hphb,swphb,shphb;
  int64_t w;
public:

  ControllerPHBR(skelgis::DMatrix_impl<T,0,false> * d):data(d)
  {
    w = data->getWidth();
    tab = data->getData();
    swphb = data->getStartWPHBR();
    wphb = data->getWidthPHBR();
    shphb = data->getStartHPHBR();
    hphb = data->getHeightPHBR();
  }
  ~ControllerPHBR(){}

  void init(skelgis::DMatrix_impl<T,0,false> * d)
  {
    data = d;
    w = data->getWidth();
    tab = data->getData();
    swphb = data->getStartWPHBR();
    wphb = data->getWidthPHBR();
    shphb = data->getStartHPHBR();
    hphb = data->getHeightPHBR();
  }

  inline T& operator()(int64_t x, int64_t y){return tab[x + w*y];}
  inline void set(int64_t x,int64_t y,T val){tab[x + w*y]=val;}
  inline int64_t startWidth(){return swphb;}
  inline int64_t startHeight(){return shphb;}
  inline int64_t width(){return wphb;}
  inline int64_t height(){return hphb;}
};

#endif
