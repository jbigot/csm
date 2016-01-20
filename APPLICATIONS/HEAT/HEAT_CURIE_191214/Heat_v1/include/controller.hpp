#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <datah.h>

template<class T> struct Controller
{
private :
  Datah * data;
  T * tab;
  int64_t w,wl,hl;
public:

  Controller(Datah * d):data(d)
  {
    w = data->getWidth();
    wl = data->getWidthLoc();
    hl = data->getHeightLoc();
    tab = data->getData();
  }
  ~Controller(){}

  void init(Datah * d)
  {
    data = d;
    w = data->getWidth();
    wl = data->getWidthLoc();
    hl = data->getHeightLoc();
    tab = data->getData();
  }

  inline T& operator()(int64_t x, int64_t y){return tab[x + w*y];}
  inline void set(int64_t x,int64_t y,T val){tab[x + w*y]=val;}
  inline int64_t start(){return 1;}
  inline int64_t width(){return 1+wl;}
  inline int64_t height(){return 1+hl;}
};

#endif
