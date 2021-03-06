#ifndef CONTROLLERPHBR_H
#define CONTROLLERPHBR_H

#include "../data_structures/Data.hpp"

namespace skelgis{

template<class T> struct ControllerPHBR
{
private :
  Data* data;
  T * tab;
  int16_t border;
  int64_t wphb,hphb,swphb,shphb;
  int64_t w;
public:

  ControllerPHBR(Data* d):data(d)
  {
    w = data->getWidth();
    tab = data->getData();
    swphb = data->getStartWPHBR();
    wphb = data->getWidthPHBR();
    shphb = data->getStartHPHBR();
    hphb = data->getHeightPHBR();
  }
  ~ControllerPHBR(){}

  void init(Data* d)
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
}
#endif
