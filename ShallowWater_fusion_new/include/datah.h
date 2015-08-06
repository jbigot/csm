#ifndef DATAH_H
#define DATAH_H

#include <iterator.hpp>

class Datah
{
 public:
  virtual void update (int tagdata) = 0;
  virtual void start_comm() = 0;
  virtual void end_comm() = 0;
  virtual inline int16_t getBorder() = 0;
  virtual void setMiddle(double val) =0;
  virtual inline double* getData() = 0;
  virtual inline int64_t getWidth() = 0;
  virtual inline int64_t getWidthLoc() = 0;
  virtual inline int64_t getHeightLoc() = 0;
  virtual inline int64_t getStartWPHBL() = 0;
  virtual inline int64_t getWidthPHBL() = 0;
  virtual inline int64_t getStartHPHBL() = 0;
  virtual inline int64_t getHeightPHBL() = 0;
  virtual inline int64_t getStartWPHBR() = 0;
  virtual inline int64_t getWidthPHBR() = 0;
  virtual inline int64_t getStartHPHBR() = 0;
  virtual inline int64_t getHeightPHBR() = 0;
  virtual inline int64_t getStartWPHBU() = 0;
  virtual inline int64_t getWidthPHBU() = 0;
  virtual inline int64_t getStartHPHBU() = 0;
  virtual inline int64_t getHeightPHBU() = 0;
  virtual inline int64_t getStartWPHBD() = 0;
  virtual inline int64_t getWidthPHBD() = 0;
  virtual inline int64_t getStartHPHBD() = 0;
  virtual inline int64_t getHeightPHBD() = 0;
};

#endif
