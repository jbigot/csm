#ifndef ITERATOR_CONT_H
#define ITERATOR_CONT_H

class Iterator
{
public:
  virtual void update () = 0;
};

//-------------------------------------------------------------------------------
struct iterator
//-------------------------------------------------------------------------------
{
public:
  int64_t _ref;
  int64_t _rank;
  int64_t _width;
  int16_t _bord;
  
  //-------------------------------------------------------------------------------
  iterator():_rank(0),_ref(0),_width(0),_bord(0){}
  //-------------------------------------------------------------------------------
  iterator(const int64_t r,const int64_t w,const int16_t b):_rank(r),_ref(r),_width(w),_bord(b){}
  //-------------------------------------------------------------------------------
  iterator(const iterator& it):_rank(it._rank),_ref(it._rank),_width(it._width),_bord(it._bord){}
  //-------------------------------------------------------------------------------
  ~iterator(){}
  //-------------------------------------------------------------------------------
  
  //-------------------------------------------------------------------------------
  iterator& operator++()
  //-------------------------------------------------------------------------------
  {
    if((_rank+1)%_width<(_width-_bord))
      ++_rank;
    else
      _rank = _rank+2*_bord+1;
    return *this;
  }
  //-------------------------------------------------------------------------------
  //-------------------------------------------------------------------------------
  //  iterator& operator++(int)
  //-------------------------------------------------------------------------------
  /*{
    iterator _tmp = *this;
    if((_rank+1)%_width<(_width-_bord))
      ++_rank;
    else
      _rank = _rank+2*_bord+1;
    return _tmp;
    }*/
  //-------------------------------------------------------------------------------
  //-------------------------------------------------------------------------------
  inline iterator& operator=(iterator right)
  //-------------------------------------------------------------------------------
  {
    _ref = right._ref;
    _rank = right._rank;
    _width = right._width;
    return *this;
  }
  //-------------------------------------------------------------------------------
  //-------------------------------------------------------------------------------
  inline bool operator==(const iterator & toTest) {return _rank==toTest._rank;}
  //-------------------------------------------------------------------------------
  inline bool operator!=(const iterator& toTest) {return _rank!=toTest._rank;}
  //-------------------------------------------------------------------------------
  inline bool operator <=(const iterator& toComp) {return _rank<=toComp._rank;}
  //-------------------------------------------------------------------------------
  inline bool operator <(const iterator & toComp) {return _rank<toComp._rank;}
  //-------------------------------------------------------------------------------
  inline bool operator >=(const iterator& toComp) {return _rank>=toComp._rank;}
  //-------------------------------------------------------------------------------
  inline bool operator >(const iterator & toComp) {return _rank>toComp._rank;}
  //-------------------------------------------------------------------------------

  inline int64_t getRight()
  {
    return _rank+1;
  }
  inline int64_t getLeft()
  {
    return _rank-1;
  }
  inline int64_t getUp()
  {
    return _rank-_width;
  }
  inline int64_t getDown()
  {
    return _rank+_width;
  }
  inline void init()
  {
    _rank=_ref;
  }
};
//-------------------------------------------------------------------------------

#endif
