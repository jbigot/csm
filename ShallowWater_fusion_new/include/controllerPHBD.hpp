#ifndef CONTROLLERPHBD_H
#define CONTROLLERPHBD_H

#include <datah.h>

template<class T> struct ControllerPHBD
{
private :
  Datah * data;
  T * tab;
  int16_t border;
  int64_t wphb,hphb,swphb,shphb;
  int64_t w;
public:

  ControllerPHBD(Datah * d):data(d)
  {
    data->getWidth(w);
    /*tab = */data->getData(tab);
    /*swphb = */data->getStartWPHBD(swphb);
    /*wphb = */data->getWidthPHBD(wphb);
    /*shphb = */data->getStartHPHBD(shphb);
    /*hphb = */data->getHeightPHBD(hphb);
  }
  ~ControllerPHBD(){}

  void init(Datah * d)
  {
    data = d;
    data->getWidth(w);
    data->getData(tab);
    data->getStartWPHBD(swphb);
    data->getWidthPHBD(wphb);
    data->getStartHPHBD(shphb);
    data->getHeightPHBD(hphb);
  }

  inline T& operator()(int64_t x, int64_t y){return tab[x + w*y];}
  inline void set(int64_t x,int64_t y,T val){tab[x + w*y]=val;}
  inline int64_t startWidth(){return swphb;}
  inline int64_t startHeight(){return shphb;}
  inline int64_t width(){return wphb;}
  inline int64_t height(){return hphb;}
};

#endif
