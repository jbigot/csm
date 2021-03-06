#ifndef CONTROLLERPHBL_H
#define CONTROLLERPHBL_H

#include "../src/rdtsc.h"
#include <datah.h>

template<class T> struct ControllerPHBL
{
private :
  Datah * data;
  T * tab;
  int16_t border;
  int64_t wphb,hphb,swphb,shphb;
  int64_t w;
public:

  ControllerPHBL(Datah * d):data(d)
  {
    //unsigned long long a,b;
    //a=rdtsc();
    data->getData(tab);
    data->getStartWPHBL(swphb);
    data->getWidthPHBL(wphb);
    data->getStartHPHBL(shphb);
    data->getHeightPHBL(hphb);
    int64_t wl;
    data->getBorder(border);
    data->getWidthLoc(wl);
    //b=rdtsc();
    //std::cout<<"virtual calls in controller = "<<b-a<<std::endl;
    w = wl + 2*border;
  }
  ~ControllerPHBL(){}

  void init(Datah * d)
  {
    data = d;
    //unsigned long long a,b;
    //a=rdtsc();
    data->getData(tab);
    data->getStartWPHBL(swphb);
    data->getWidthPHBL(wphb);
    data->getStartHPHBL(shphb);
    data->getHeightPHBL(hphb);
    int64_t wl;
    data->getBorder(border);
    data->getWidthLoc(wl);
    //b=rdtsc();
    //std::cout<<"virtual calls in controller = "<<b-a<<std::endl;
    w = wl + 2*border;
  }

  inline T& operator()(int64_t x, int64_t y){return tab[x + w*y];}
  inline void set(int64_t x,int64_t y,T val){tab[x + w*y]=val;}
  inline int64_t startWidth(){return swphb;}
  inline int64_t startHeight(){return shphb;}
  inline int64_t width(){return wphb;}
  inline int64_t height(){return hphb;}
};

#endif
