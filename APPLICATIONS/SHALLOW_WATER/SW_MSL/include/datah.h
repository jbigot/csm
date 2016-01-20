#ifndef DATAH_H
#define DATAH_H

#include <iterator.hpp>
#include "libskelgis/skelgis.hpp"

class Datah
{
 public:
  virtual void update () = 0;
  //virtual void start_comm() = 0;
  //virtual void end_comm() = 0;
  // virtual inline int16_t getBorder() = 0;
  // virtual void setMiddle(double val) =0;
  //virtual inline double* getData() = 0;
  // virtual inline int64_t getWidth() = 0;
  // virtual inline int64_t getWidthLoc() = 0;
  // virtual inline int64_t getHeightLoc() = 0;
  // virtual inline int64_t getStartWPHBL() = 0;
  // virtual inline int64_t getWidthPHBL() = 0;
  // virtual inline int64_t getStartHPHBL() = 0;
  // virtual inline int64_t getHeightPHBL() = 0;
  // virtual inline int64_t getStartWPHBR() = 0;
  // virtual inline int64_t getWidthPHBR() = 0;
  // virtual inline int64_t getStartHPHBR() = 0;
  // virtual inline int64_t getHeightPHBR() = 0;
  // virtual inline int64_t getStartWPHBU() = 0;
  // virtual inline int64_t getWidthPHBU() = 0;
  // virtual inline int64_t getStartHPHBU() = 0;
  // virtual inline int64_t getHeightPHBU() = 0;
  // virtual inline int64_t getStartWPHBD() = 0;
  // virtual inline int64_t getWidthPHBD() = 0;
  // virtual inline int64_t getStartHPHBD() = 0;
  // virtual inline int64_t getHeightPHBD() = 0;

  virtual inline void getBorder(int16_t& b) = 0;
  virtual void setMiddle(double val) =0;
  virtual inline void getData(double * & data) = 0;
  virtual void getDMatrix(skelgis::DMatrix_impl<double,2,false> * & dm) = 0;
  virtual inline void getWidth(int64_t& w) = 0;
  virtual inline void getWidthLoc(int64_t& wl) = 0;
  virtual inline void getHeightLoc(int64_t& hl) = 0;
  virtual inline void getStartWPHBL(int64_t& v) = 0;
  virtual inline void getWidthPHBL(int64_t& v) = 0;
  virtual inline void getStartHPHBL(int64_t& v) = 0;
  virtual inline void getHeightPHBL(int64_t& v) = 0;
  virtual inline void getStartWPHBR(int64_t& v) = 0;
  virtual inline void getWidthPHBR(int64_t& v) = 0;
  virtual inline void getStartHPHBR(int64_t& v) = 0;
  virtual inline void getHeightPHBR(int64_t& v) = 0;
  virtual inline void getStartWPHBU(int64_t& v) = 0;
  virtual inline void getWidthPHBU(int64_t& v) = 0;
  virtual inline void getStartHPHBU(int64_t& v) = 0;
  virtual inline void getHeightPHBU(int64_t& v) = 0;
  virtual inline void getStartWPHBD(int64_t& v) = 0;
  virtual inline void getWidthPHBD(int64_t& v) = 0;
  virtual inline void getStartHPHBD(int64_t& v) = 0;
  virtual inline void getHeightPHBD(int64_t& v) = 0;
};

#endif
