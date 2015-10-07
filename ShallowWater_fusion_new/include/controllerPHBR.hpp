#ifndef CONTROLLERPHBR_H
#define CONTROLLERPHBR_H

#include <datah.h>

template<class T> struct ControllerPHBR
{
private :
  Datah * data;
  T * tab;
  int16_t border;
  int64_t wphb,hphb,swphb,shphb;
  int64_t w;
public:

  ControllerPHBR(Datah * d):data(d)
  {
    data->getWidth(w);
    data->getData(tab);
    data->getStartWPHBR(swphb);
    data->getWidthPHBR(wphb);
    data->getStartHPHBR(shphb);
    data->getHeightPHBR(hphb);
  }
  ~ControllerPHBR(){}

  void init(Datah * d)
  {
    data = d;
    data->getWidth(w);
    data->getData(tab);
    data->getStartWPHBR(swphb);
    data->getWidthPHBR(wphb);
    data->getStartHPHBR(shphb);
    data->getHeightPHBR(hphb);
  }

  inline T& operator()(int64_t x, int64_t y){return tab[x + w*y];}
  inline void set(int64_t x,int64_t y,T val){tab[x + w*y]=val;}
  inline int64_t startWidth(){return swphb;}
  inline int64_t startHeight(){return shphb;}
  inline int64_t width(){return wphb;}
  inline int64_t height(){return hphb;}
};

#endif
