#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <datah.h>

template<class T> struct Controller
{
private :
  Datah * data;
  T * tab;
  int16_t border;
  int64_t w,wl,hl;
public:

  Controller(Datah * d):data(d)
  {
    /*w = */data->getWidth(w);
    /*wl = */data->getWidthLoc(wl);
    /*hl = */data->getHeightLoc(hl);
    /*tab = */data->getData(tab);
    /*border = */data->getBorder(border);
  }
  ~Controller(){}

  void init(Datah * d)
  {
    data = d;
    data->getWidth(w);
    data->getWidthLoc(wl);
    data->getHeightLoc(hl);
    data->getData(tab);
    data->getBorder(border);
  }

  inline T& operator()(int64_t x, int64_t y){return tab[x + w*y];}
  inline void set(int64_t x,int64_t y,T val){tab[x + w*y]=val;}
  inline int64_t start(){return border;}
  inline int64_t width(){return border+wl;}
  inline int64_t height(){return border+hl;}
};

#endif
