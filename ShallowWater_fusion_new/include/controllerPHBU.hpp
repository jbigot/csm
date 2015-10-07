#ifndef CONTROLLERPHBU_H
#define CONTROLLERPHBU_H

#include <datah.h>

template<class T> struct ControllerPHBU
{
private :
  Datah * data;
  T * tab;
  int16_t border;
  int64_t wphb,hphb,swphb,shphb;
  int64_t w;
public:

  ControllerPHBU(Datah * d):data(d)
  {
    data->getWidth(w);
    data->getData(tab);
    data->getStartWPHBU(swphb);
    data->getWidthPHBU(wphb);
    data->getStartHPHBU(shphb);
    data->getHeightPHBU(hphb);
  }
  ~ControllerPHBU(){}

  void init(Datah * d)
  {
    data = d;
    data->getWidth(w);
    data->getData(tab);
    data->getStartWPHBU(swphb);
    data->getWidthPHBU(wphb);
    data->getStartHPHBU(shphb);
    data->getHeightPHBU(hphb);
  }

  inline T& operator()(int64_t x, int64_t y){return tab[x + w*y];}
  inline void set(int64_t x,int64_t y,T val){tab[x + w*y]=val;}
  inline int64_t startWidth(){return swphb;}
  inline int64_t startHeight(){return shphb;}
  inline int64_t width(){return wphb;}
  inline int64_t height(){return hphb;}
};

#endif
