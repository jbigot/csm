#ifndef ITERATOR_STEP_H
#define ITERATOR_STEP_H

namespace skelgis{

  //-------------------------------------------------------------------------------
  template<class T,int R> struct iterator_step
  //-------------------------------------------------------------------------------
  {
  public:
    unsigned int _rank;
    unsigned int _width;
    int _step;
    int _nb;
    int _cpt;

    //-------------------------------------------------------------------------------
    iterator_step():_rank(0),_width(0),_cpt(1),_nb(1),_step(1){}
    //-------------------------------------------------------------------------------
    iterator_step(const unsigned int r,const unsigned int w,int s=1,int n=1):_rank(r),_width(w),_cpt(n),_nb(n),_step(s){}
    //-------------------------------------------------------------------------------
    iterator_step(const iterator_step<T,R>& it):_rank(it._rank),_width(it._width),_cpt(it._nb),_nb(it._nb),_step(it._step){}
    //-------------------------------------------------------------------------------
    ~iterator_step(){}
    //-------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------
    iterator_step<T,R>& operator++()
    //-------------------------------------------------------------------------------
    {
      if(_cpt>1)
	{
	  if((_rank+1)%_width<(_width-R))
	    ++_rank;
	  else
	    _rank = _rank+2*R+1;

	  --_cpt;
	}
      else
	{
	  if((_rank+_step)%_width>=(_width-R) || (_rank+_step)%_width<_rank%_width)
	    _rank = _rank+_step+1+2*R+1;
	  else
	    _rank = _rank+_step+1;
	  _cpt = _nb;
	}
      return *this;
    }
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    iterator_step<T,R>& operator++(int)
    //-------------------------------------------------------------------------------
    {
      iterator_step<T,R> _tmp = *this;
      if(_cpt>1)
	{
	  if((_rank+1)%_width<(_width-R))
	    ++_rank;
	  else
	    _rank = _rank+2*R+1;

	  --_cpt;
	}
      else
	{
	  if((_rank+_step)%_width>=(_width-R) || (_rank+_step)%_width<_rank%_width)
	    _rank = _rank+_step+1+2*R+1;
	  else
	    _rank = _rank+_step+1;

	  _cpt = _nb;
	}
      return _tmp;
    }
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    inline iterator_step<T,R>& operator=(iterator_step<T,R> right)
    //-------------------------------------------------------------------------------
    {
      _rank = right._rank;
      _width = right._width;
      _cpt = right._nb;
      _nb = right._nb;
      _step = right._step;
      return *this;
    }
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    inline bool operator==(const iterator_step<T,R> & toTest) {return this->_rank==toTest._rank;}
    //-------------------------------------------------------------------------------
    inline bool operator!=(const iterator_step<T,R>& toTest) {return this->_rank!=toTest._rank;}
    //-------------------------------------------------------------------------------
    inline bool operator <=(const iterator_step<T,R>& toComp) {return this->_rank<=toComp._rank;}
    //-------------------------------------------------------------------------------
    inline bool operator <(const iterator_step<T,R>& toComp) {return this->_rank<toComp._rank;}
    //-------------------------------------------------------------------------------
    inline bool operator >=(const iterator_step<T,R>& toComp) {return this->_rank>=toComp._rank;}
    //-------------------------------------------------------------------------------
    inline bool operator >(const iterator_step<T,R>& toComp) {return this->_rank>toComp._rank;}
    //-------------------------------------------------------------------------------
  };

  //-------------------------------------------------------------------------------
  template<class T> struct iterator_step<T,0>
  //-------------------------------------------------------------------------------
  {
  public:
    unsigned int _rank;
    int _step;
    int _nb;
    int _cpt;

    //-------------------------------------------------------------------------------
    iterator_step():_rank(0),_cpt(1),_nb(1),_step(1){}
    //-------------------------------------------------------------------------------
    iterator_step(const unsigned int r,int s=1,int n=1):_rank(r),_cpt(n),_nb(n),_step(s){}
    //-------------------------------------------------------------------------------
    iterator_step(const iterator_step<T,0>& it):_rank(it._rank),_cpt(it._nb),_nb(it._nb),_step(it._step){}
    //-------------------------------------------------------------------------------
    ~iterator_step(){}
    //-------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------
    iterator_step<T,0>& operator++()
    //-------------------------------------------------------------------------------
    {
      if(_cpt>1)
	{
	  ++_rank;
	  --_cpt;
	}
      else
	{
	  _rank = _rank+_step+1;
	  _cpt = _nb;
	}
      return *this;
    }
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    iterator_step<T,0>& operator++(int)
    //-------------------------------------------------------------------------------
    {
      iterator_step<T,0> _tmp = *this;
      if(_cpt>1)
	{
	  ++_rank;
	  --_cpt;
	}
      else
	{
	  _rank = _rank+_step+1;
	  _cpt = _nb;
	}
      return _tmp;
    }
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    inline iterator_step<T,0>& operator=(iterator_step<T,0> right)
    //-------------------------------------------------------------------------------
    {
      _rank = right._rank;
      _cpt = right._nb;
      _nb = right._nb;
      _step = right._step;
      return *this;
    }
    //-------------------------------------------------------------------------------
    inline bool operator==(const iterator_step<T,0> & toTest) {return this->_rank==toTest._rank;}
    //-------------------------------------------------------------------------------
    inline bool operator!=(const iterator_step<T,0>& toTest) {return this->_rank!=toTest._rank;}
    //-------------------------------------------------------------------------------
    inline bool operator <=(const iterator_step<T,0>& toComp) {return this->_rank<=toComp._rank;}
    //-------------------------------------------------------------------------------
    inline bool operator <(const iterator_step<T,0>& toComp) {return this->_rank<toComp._rank;}
    //-------------------------------------------------------------------------------
    inline bool operator >=(const iterator_step<T,0>& toComp) {return this->_rank>=toComp._rank;}
    //-------------------------------------------------------------------------------
    inline bool operator >(const iterator_step<T,0>& toComp) {return this->_rank>toComp._rank;}
    //-------------------------------------------------------------------------------
  };

}

#endif
