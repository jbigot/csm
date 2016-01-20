#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../data_structures/dmatrix_impl.hpp"


template<class T, int R,bool line=true> struct Controller
{
private :
  skelgis::DMatrix_impl<T,R,true> * data;
  T * tab;
  int16_t border;
  int64_t w,wl,hl;
public:

  Controller(skelgis::DMatrix_impl<T,R,true> * d):data(d)
  {
    w = data->getWidth();
    wl = data->getWidthLoc();
    hl = data->getHeightLoc();
    tab = data->getData();
    border = data->getBorder();
  }
  ~Controller(){}

  void init(skelgis::DMatrix_impl<T,R,true> * d)
  {
    data = d;
    w = data->getWidth();
    wl = data->getWidthLoc();
    hl = data->getHeightLoc();
    tab = data->getData();
    border = data->getBorder();
  }

  inline T& operator()(int64_t x, int64_t y){return tab[x + w*y];}
  inline void set(int64_t x,int64_t y,T val){tab[x + w*y]=val;}
  inline int64_t start(){return border;}
  inline int64_t width(){return border+wl;}
  inline int64_t height(){return border+hl;}
};

template<class T> struct Controller<T,0,true>
{
private :
  skelgis::DMatrix_impl<T,0,true> * data;
  T * tab;
  int16_t border;
  int64_t w,wl,hl;
public:

  Controller(skelgis::DMatrix_impl<T,0,true> * d):data(d)
  {
    w = data->getWidth();
    wl = data->getWidthLoc();
    hl = data->getHeightLoc();
    tab = data->getData();
    border = data->getBorder();
  }
  ~Controller(){}

  void init(skelgis::DMatrix_impl<T,0,true> * d)
  {
    data = d;
    w = data->getWidth();
    wl = data->getWidthLoc();
    hl = data->getHeightLoc();
    tab = data->getData();
    border = data->getBorder();
  }

  inline T& operator()(int64_t x, int64_t y){return tab[x + w*y];}
  inline void set(int64_t x,int64_t y,T val){tab[x + w*y]=val;}
  inline int64_t start(){return border;}
  inline int64_t width(){return border+wl;}
  inline int64_t height(){return border+hl;}
};

template<class T,int R> struct Controller<T,R,false>
{
private :
  skelgis::DMatrix_impl<T,R,false> * data;
  T * tab;
  int16_t border;
  int64_t w,wl,hl;
public:

  Controller(skelgis::DMatrix_impl<T,R,false> * d):data(d)
  {
    w = data->getWidth();
    wl = data->getWidthLoc();
    hl = data->getHeightLoc();
    tab = data->getData();
    border = data->getBorder();
  }
  ~Controller(){}

  void init(skelgis::DMatrix_impl<T,R,false> * d)
  {
    data = d;
    w = data->getWidth();
    wl = data->getWidthLoc();
    hl = data->getHeightLoc();
    tab = data->getData();
    border = data->getBorder();
  }

  inline T& operator()(int64_t x, int64_t y){return tab[x + w*y];}
  inline void set(int64_t x,int64_t y,T val){tab[x + w*y]=val;}
  inline int64_t start(){return border;}
  inline int64_t width(){return border+wl;}
  inline int64_t height(){return border+hl;}
};

template<class T> struct Controller<T,0,false>
{
private :
  skelgis::DMatrix_impl<T,0,false> * data;
  T * tab;
  int16_t border;
  int64_t w,wl,hl;
public:

  Controller(skelgis::DMatrix_impl<T,0,false> * d):data(d)
  {
    w = data->getWidth();
    wl = data->getWidthLoc();
    hl = data->getHeightLoc();
    tab = data->getData();
    border = data->getBorder();
  }
  ~Controller(){}

  void init(skelgis::DMatrix_impl<T,0,false> * d)
  {
    data = d;
    w = data->getWidth();
    wl = data->getWidthLoc();
    hl = data->getHeightLoc();
    tab = data->getData();
    border = data->getBorder();
  }

  inline T& operator()(int64_t x, int64_t y){return tab[x + w*y];}
  inline void set(int64_t x,int64_t y,T val){tab[x + w*y]=val;}
  inline int64_t start(){return border;}
  inline int64_t width(){return border+wl;}
  inline int64_t height(){return border+hl;}
};

#endif

