#ifndef DATAH_H
#define DATAH_H

#include <iterator.hpp>

class Datah
{
 public:
  virtual void update () = 0;
  virtual inline iterator begin () = 0;
  virtual inline iterator end () = 0;
  virtual inline double get(iterator) =0;
  virtual inline void set(iterator,double) =0;
  virtual inline double getUp(iterator) =0;
  virtual inline double getRight(iterator) =0;
  virtual inline double getDown(iterator) =0;
  virtual inline double getLeft(iterator) =0;
  virtual void setMiddle(double val) =0;
  virtual inline double* getData() = 0;
  virtual inline int64_t getWidth() = 0;
  virtual inline int64_t getWidthLoc() = 0;
  virtual inline int64_t getHeightLoc() = 0;
};

#endif
