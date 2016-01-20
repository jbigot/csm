#ifndef ITERATOR_REV_H
#define ITERATOR_REV_H

namespace skelgis{

  //-------------------------------------------------------------------------------
  template<class T,int R> struct iterator_rev
  //-------------------------------------------------------------------------------
  {
  public:
    unsigned int _rank;
    unsigned int _width;

    //-------------------------------------------------------------------------------
    iterator_rev():_rank(0),_width(0){}
    //-------------------------------------------------------------------------------
    iterator_rev(const unsigned int r,const unsigned int w):_rank(r),_width(w){}
    //-------------------------------------------------------------------------------
    iterator_rev(const iterator_rev& it):_rank(it._rank),_width(it._width){}
    //-------------------------------------------------------------------------------
    ~iterator_rev(){}
    //-------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------
    iterator_rev& operator++()
    //-------------------------------------------------------------------------------
    {
      if((_rank-1)%_width<R)
	_rank = _rank - 2*R -1;
      else
	--_rank;

      return *this;
    }
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    iterator_rev& operator++(int)
    //-------------------------------------------------------------------------------
    {
      iterator_rev<T,R> _tmp = *this;
      if((_rank-1)%_width<R)
	_rank = _rank - 2*R -1;
      else
	--_rank;

      return _tmp;
    }
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    inline iterator_rev& operator=(iterator_rev right)
    //-------------------------------------------------------------------------------
    {
      _rank = right._rank;
      _width = right._width;
      return *this;
    }
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    inline bool operator==(const iterator_rev & toTest) {return _rank==toTest._rank;}
    //-------------------------------------------------------------------------------
    inline bool operator!=(const iterator_rev& toTest) {return _rank!=toTest._rank;}
    //-------------------------------------------------------------------------------
    inline bool operator <=(const iterator_rev& toComp) {return _rank>=toComp._rank;}
    //-------------------------------------------------------------------------------
    inline bool operator <(const iterator_rev & toComp) {return _rank>toComp._rank;}
    //------------------------------------------------------------------------------
    inline bool operator >=(const iterator_rev& toComp) {return _rank<=toComp._rank;}
    //-------------------------------------------------------------------------------
    inline bool operator >(const iterator_rev & toComp) {return _rank<toComp._rank;}
    //-------------------------------------------------------------------------------
  };
  //-------------------------------------------------------------------------------

  //-------------------------------------------------------------------------------
  template<class T> struct iterator_rev<T,0>
  //-------------------------------------------------------------------------------
  {
  public:
    unsigned int _rank;
    unsigned int _width;

    //-------------------------------------------------------------------------------
    iterator_rev():_rank(0),_width(0){}
    //-------------------------------------------------------------------------------
    iterator_rev(const unsigned int r,const unsigned int w):_rank(r),_width(w){}
    //-------------------------------------------------------------------------------
    iterator_rev(const iterator_rev& it):_rank(it._rank),_width(it._width){}
    //------------------------------------------------------------------------------
    iterator_rev& operator++(){--_rank;return *this;}
    //-------------------------------------------------------------------------------
    iterator_rev& operator++(int){iterator_rev<T,0> _tmp = *this;--_rank;return _tmp;}
    //-------------------------------------------------------------------------------
    inline iterator_rev& operator=(iterator_rev right)
    //-------------------------------------------------------------------------------
    {
      _rank = right._rank;
      return *this;
    }
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    inline bool operator==(const iterator_rev & toTest) {return _rank==toTest._rank;}
    //-------------------------------------------------------------------------------
    inline bool operator!=(const iterator_rev& toTest) {return _rank!=toTest._rank;}
    //-------------------------------------------------------------------------------
    inline bool operator <=(const iterator_rev& toComp) {return _rank>=toComp._rank;}
    //-------------------------------------------------------------------------------
    inline bool operator <(const iterator_rev & toComp) {return _rank>toComp._rank;}
    //------------------------------------------------------------------------------
    inline bool operator >=(const iterator_rev& toComp) {return _rank<=toComp._rank;}
    //-------------------------------------------------------------------------------
    inline bool operator >(const iterator_rev & toComp) {return _rank<toComp._rank;}
    //-------------------------------------------------------------------------------
  };
  //-------------------------------------------------------------------------------
}

#endif



