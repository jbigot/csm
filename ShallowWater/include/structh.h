#ifndef STRUCTH_H
#define STRUCTH_H

class Structh
{
 public:
  virtual void init (/*int64_t, int64_t, int32_t*/) = 0;
  virtual void getSizes (int64_t &, int64_t &) = 0;
  //virtual void getBorder (int16_t &) = 0;
  virtual int32_t* getNeighbors () = 0;
  virtual int32_t getTileY() = 0;
  virtual int32_t getNbTileY() = 0;
  virtual int32_t getTileX() = 0;
  virtual int32_t getNbTileX() = 0;
  virtual int64_t getGlobW() = 0;
  virtual int64_t getGlobH() =0;
};

#endif
