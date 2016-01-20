#ifndef CONTROLLERPHBD_H
#define CONTROLLERPHBD_H

#include "../data_structures/Data.hpp"

namespace skelgis{

template<class T> struct ControllerPHBD
{
private :
  Data * data;
  T * tab;
  int16_t border;
  int64_t wphb,hphb,swphb,shphb;
  int64_t w;
public:

  ControllerPHBD(Data * d):data(d)
  {
    w = data->getWidth();
    tab = data->getData();
    swphb = data->getStartWPHBD();
    wphb = data->getWidthPHBD();
    shphb = data->getStartHPHBD();
    hphb = data->getHeightPHBD();
  }
  ~ControllerPHBD(){}

  void init(Data * d)
  {
    data = d;
    w = data->getWidth();
    tab = data->getData();
    swphb = data->getStartWPHBD();
    wphb = data->getWidthPHBD();
    shphb = data->getStartHPHBD();
    hphb = data->getHeightPHBD();
  }

  inline T& operator()(int64_t x, int64_t y){return tab[x + w*y];}
  inline void set(int64_t x,int64_t y,T val){tab[x + w*y]=val;}
  inline int64_t startWidth(){return swphb;}
  inline int64_t startHeight(){return shphb;}
  inline int64_t width(){return wphb;}
  inline int64_t height(){return hphb;}
};
}
#endif
