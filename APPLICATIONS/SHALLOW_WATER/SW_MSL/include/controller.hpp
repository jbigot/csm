#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <datah.h>

#include "../src/rdtsc.h"

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
    //unsigned long long a,b;
    //a=rdtsc();
    /*wl = */data->getWidthLoc(wl);
    /*hl = */data->getHeightLoc(hl);
    /*tab = */data->getData(tab);
    /*border = */data->getBorder(border);
    //b=rdtsc();
    //std::cout<<"virtual calls in controller = "<<b-a<<std::endl;
    w = wl + 2*border;
  }
  ~Controller(){}

  void init(Datah * d)
  {
    data = d;
    //unsigned long long a,b;
    //a=rdtsc();
    data->getWidthLoc(wl);
    data->getHeightLoc(hl);
    data->getData(tab);
    data->getBorder(border);
    //b=rdtsc();
    //std::cout<<"virtual calls in controller = "<<b-a<<std::endl;
    w = wl + 2*border;
  }

  inline T& operator()(int64_t x, int64_t y){return tab[x + w*y];}
  inline void set(int64_t x,int64_t y,T val){tab[x + w*y]=val;}
  inline int64_t start(){return border;}
  inline int64_t width(){return border+wl;}
  inline int64_t height(){return border+hl;}
};

#endif
