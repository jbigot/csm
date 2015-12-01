#ifndef DMATRIX_IMPL_H
#define DMATRIX_IMPL_H

#include "dmatrix.hpp"
#include "../util/utility.hpp"
#include "../util/mpi_.hpp"
#include "../util/communications.hpp"

#include "../iterators/iterator.hpp"
#include "../iterators/iterator_cont.hpp"
#include "../iterators/iterator_rev.hpp"
#include "../iterators/iterator_phb.hpp"
#include "../iterators/iterator_step.hpp"
#include "../iterators/iterator_line.hpp"

#include<math.h>

namespace skelgis{

  //================================================================================
  //!  base template Distributed Matrix Class with recovery
  /*!
    This template class defines the base distributed matrix with recovery
    T = type of data in the matrix
  */
  //-------------------------------------------------------------------------------
  template<class T> struct DMatrix_base
  //-------------------------------------------------------------------------------
  {
  protected:

    //-------------------------------------------------------------------------------
    DMatrix_base(){}
    ~DMatrix_base(){}
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    //! column and row position in the general division
    int col,row;
    //! total number of columns and rows for he current decomposition (MPI or OpenMP)
    int cols,rows;
  public:
    //data
    T * data;

    /*virtual inline T * getData();
    virtual inline unsigned int getWidth();
    virtual inline unsigned int getWidthLoc();
    virtual inline unsigned int getHeightLoc();
    virtual inline int getBorder();
    */
  protected:
    //! Header defining the current global matrix
    HEADER head;
    //! Header defining the current local matrix
    HEADER loc_head;
    //to get the width of (cols-1) first columns (normal_width) and of the last column (last_width)
    unsigned int normal_width,last_width;
    //to get the height of (cols-1) first columns (normal_height) and of the last column (last_height)
    //unsigned int normal_height,last_height;

    int remainderh,remainderw;
    //-------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------
    virtual void create() = 0;
    virtual void initValues(T value) = 0;
    //-------------------------------------------------------------------------------
  };
  //================================================================================
  //================================================================================
  //!  base template Distributed Matrix Class with recovery
  /*!
    This template class defines the base distributed matrix with recovery
    T = type of data in the matrix
  */
  //-------------------------------------------------------------------------------
  template<class T,int R,bool line=true> struct DMatrix_impl : public DMatrix_base<T>
  //-------------------------------------------------------------------------------
  {
  protected:

    int border;
    //! Header defining the current local matrix with border
    HEADER border_head;
    //! ranks of MPI neighbors
    int mpi_ranks[2];
    unsigned int beginPhBLeft,beginPhBRight,beginPhBUp,beginPhBDown;
    unsigned int endPhBLeft,endPhBRight,endPhBUp,endPhBDown;

    //-------------------------------------------------------------------------------
    void create()
    //-------------------------------------------------------------------------------
    {
      //division of the matrix in cols and rows
      this->cols = 1;
      this->rows = Mpi_::mpi_nb;
	
#ifdef DEBUG
      std::stringstream st;
      st<<"rows and cols :"<<this->rows<<","<<this->cols<<"\n";
      Mpi_::print(st.str());
#endif

      this->row=Mpi_::mpi_rank;
      this->col=0;
	
#ifdef DEBUG
      std::stringstream st3;
      st3<<"MPI row and col :"<<this->row<<","<<this->col<<"\n"<<"\n";
      Mpi_::print(st3.str());
#endif	
      
      this->loc_head.width=this->head.width;
      
     this-> remainderh = 0;
      //if the height of the matrix can be divided by the number of rows
      if(this->head.height%this->rows==0)
	this->loc_head.height=floor(this->head.height/this->rows);
      //else we add one to the height of the submatrix and the last row will have less elements
      else
	{
	  this->remainderh = this->head.height - floor(this->head.height/this->rows)*this->rows;

	  if(this->row<this->remainderh)
	    this->loc_head.height=floor(this->head.height/this->rows)+1;
	  else
	    this->loc_head.height=floor(this->head.height/this->rows);
	}

      this->loc_head.x=this->head.x;
      this->loc_head.y=this->head.y-this->row*this->loc_head.height*this->head.spacing;
      this->loc_head.spacing=this->head.spacing;
      this->loc_head.nodata=this->head.nodata;

      this->border_head.x = this->loc_head.x - border*this->loc_head.spacing;
      this->border_head.y = this->loc_head.y - border*this->loc_head.spacing;
      this->border_head.width = this->loc_head.width + border*2;
      this->border_head.height = this->loc_head.height + border*2;
      
#ifdef DEBUG
      std::stringstream st2;
      st2<<"Global width and height :"<<this->head.width<<","<<this->head.height<<"\n";
      Mpi_::print(st2.str());
#endif

#ifdef DEBUG
      std::stringstream st1;
      st1<<"Local width and height :"<<this->loc_head.width<<","<<this->loc_head.height<<"\n";
      Mpi_::print(st1.str());
#endif
      
      this->data = new T[this->border_head.width*this->border_head.height];
    }
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    void initValues(T value)
    //-------------------------------------------------------------------------------
    {
      for(unsigned int i=0;i<this->border_head.width*this->border_head.height;i++)
	{
	  this->data[i]=value;
	}
    }
    //-------------------------------------------------------------------------------
    void phBLimits()
    //-------------------------------------------------------------------------------
    {
      beginPhBLeft = R*this->border_head.width;
      beginPhBRight = (R+1)*this->border_head.width-R;
      endPhBLeft = this->border_head.width*(this->border_head.height-R-1)+R;
      endPhBRight = (R+1+this->loc_head.height-1)*this->border_head.width;

      if(Mpi_::mpi_rank==0)
	{
	  beginPhBUp = R;
	  endPhBUp = R*this->border_head.width-R;
	}
      else
	{
	  beginPhBUp = 0;
	  endPhBUp = 0;
	}
      if(Mpi_::mpi_rank==Mpi_::mpi_nb-1)
	{
	  beginPhBDown = this->border_head.width*(this->loc_head.height+R)+R;
	  endPhBDown = this->border_head.width*this->border_head.height-R;
	}
      else
	{
	  beginPhBDown = 0;
	  endPhBDown = 0;
	}
    }
    //-------------------------------------------------------------------------------
    //! Read the zone file concerned for the current MPI process
    /*!
      read the MPI prcess zone, and spread the data in the subMatrices OpenMP
      only one submatrice if OpenMP is disabled
      \param binFile is binary file to read
    */
    //-------------------------------------------------------------------------------
    void read(const char * binFile)
    //-------------------------------------------------------------------------------
    {
      //read the zone of the file for my own block of data
      std::ifstream f(binFile, std::ios::binary | std::ios::in);

      //move to the begining of the bloc of data concerned
      unsigned int mvline;
      if(Mpi_::mpi_rank<this->remainderh)
	mvline = sizeof(HEADER) + (this->row*this->loc_head.height*this->head.width)*sizeof(T);
      else
	mvline = sizeof(HEADER) + ((this->remainderh*(this->loc_head.height+1)+(this->row-this->remainderh)*this->loc_head.height)*this->head.width)*sizeof(T);

      f.seekg(mvline,std::ios::beg);

      //read by lines
      for(unsigned int j=0;j<this->loc_head.height;j++)
	{
	  for(unsigned int k=0;k<this->loc_head.width;k++)
	    {
	      T val;
	      unsigned int rank = border*this->border_head.width + j*this->border_head.width + k + border;
	      f.read(reinterpret_cast<char*>(&val),sizeof(T));
	      this->data[rank]=val;
	    }
	}
    }
    //-------------------------------------------------------------------------------
    //! get ranks
    /*!
      Get the ranks of the 2 neighbors for MPI exchanges
      \return 
      if the value is -55555 then there is no neighbor in this direction
      if the value is >0 it represent the MPI rank concerned by the exchange
    */
    //-------------------------------------------------------------------------------
    void getNeighborRanks()
    //-------------------------------------------------------------------------------
    {
      for(int i=0;i<2;i++)
	mpi_ranks[i] = -55555;
      //up
      if((this->row-1)>=0)
	mpi_ranks[0] = Mpi_::mpi_rank-1;
      //down
      if((this->row+1)<this->rows)
	mpi_ranks[1] = Mpi_::mpi_rank+1;
    }
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    //! get the up border to send to neighbor processors
    /*!
      \return list of values
    */
    //-------------------------------------------------------------------------------
    T * getUpBorderToSend()
    //-------------------------------------------------------------------------------
    {
      T * result = new T[this->loc_head.width*border];
      for(int i=0;i<border;i++)
	{
	  for(unsigned int j=0;j<this->loc_head.width;j++)
	    {
	      unsigned int r1 = i*this->loc_head.width+j;
	      unsigned int r2 = (border*border_head.width + border) + i*border_head.width+j;
	      result[r1] = this->data[r2];
	    }
	}
      return result;
    }
    //-------------------------------------------------------------------------------
    //! set the up border ghost cells with neighbor values from processors
    /*!
      \param values is the table values to fill ghost cells with
    */
    //-------------------------------------------------------------------------------
    void setUpBorder(T * values)
    //-------------------------------------------------------------------------------
    {
      for(int i=0;i<border;i++)
	{
	  for(unsigned int j=0;j<this->loc_head.width;j++)
	    this->data[(border) + i*border_head.width+j] = values[i*this->loc_head.width+j];
	}
    }
    //-------------------------------------------------------------------------------
    //! get the down border to send to neighbor processors
    /*!
      \return list of values
    */
    //-------------------------------------------------------------------------------
    T * getDownBorderToSend()
    //-------------------------------------------------------------------------------
    {
      T * result = new T[this->loc_head.width*border];
      for(int i=0;i<border;i++)
	{
	  for(unsigned int j=0;j<this->loc_head.width;j++)
	    {
	      unsigned int r1 = i*this->loc_head.width+j;
	      unsigned int r2 = (border_head.height*border_head.width - (border+border)*border_head.width + border) + i*border_head.width + j;
	      result[r1] = this->data[r2];
	    }
	}
      return result;
    }
    //-------------------------------------------------------------------------------
    //! set the down border ghost cells with neighbor values from processors
    /*!
      \param values is the table values to fill ghost cells with
    */
    //-------------------------------------------------------------------------------
    void setDownBorder(T * values)
    //-------------------------------------------------------------------------------
    {
      for(int i=0;i<border;i++)
	{
	  for(unsigned int j=0;j<this->loc_head.width;j++)
	    this->data[(border_head.width*border_head.height - border_head.width*border + border) + i*border_head.width +j] = values[i*this->loc_head.width+j];
	}
    }
    //-------------------------------------------------------------------------------

        //-------------------------------------------------------------------------------
    T * allRight(unsigned int rank)
    //-------------------------------------------------------------------------------
    {
      T * res = new T[R];
      unsigned int r = rank+1;
      for(int i=0;i<R;i++)
	{
	  res[i] = this->data[r];
	  r++;
	}
      return res;
    }
    //-------------------------------------------------------------------------------
    T * allLeft(unsigned int rank)
    //-------------------------------------------------------------------------------
    {
      T * res = new T[R];
      unsigned int r = rank-1;
      for(int i=0;i<R;i++)
	{
	  res[i] = this->data[r];
	  r--;
	}
      return res;
    }
    //-------------------------------------------------------------------------------
    T * allUp(unsigned int rank,unsigned int width)
    //-------------------------------------------------------------------------------
    {
      T * res = new T[R];
      unsigned int r = rank - width;
      for(int i=0;i<R;i++)
	{
	  res[i] = this->data[r];
	  r = r - width;
	}
      return res;
    }
    //-------------------------------------------------------------------------------
    T * allDown(unsigned int rank,unsigned int width)
    //-------------------------------------------------------------------------------
    {
      T * res = new T[R];
      unsigned int r = rank + width;
      for(int i=0;i<R;i++)
	{
	  res[i] = this->data[r];
	  r = r + width;
	}
      return res;
    }
    //-------------------------------------------------------------------------------
    T * allRightDown(unsigned int rank,unsigned int width)
    //-------------------------------------------------------------------------------
    {
      T * res = new T[R];
      unsigned int r = rank + width + 1;
      for(int i=0;i<R;i++)
	{
	  res[i] = this->data[r];
	  r = r + width + 1;
	}
      return res;
    }
    //-------------------------------------------------------------------------------
    T * allLeftDown(unsigned int rank,unsigned int width)
    //-------------------------------------------------------------------------------
    {
      T * res = new T[R];
      unsigned int r = rank + width -1 ;
      for(int i=0;i<R;i++)
	{
	  res[i] = this->data[r];
	  r = r + width -1 ;
	}
      return res;
    }
    //-------------------------------------------------------------------------------
    T * allRightUp(unsigned int rank,unsigned int width)
    //-------------------------------------------------------------------------------
    {
      T * res = new T[R];
      unsigned int r = rank - width + 1;
      for(int i=0;i<R;i++)
	{
	  res[i] = this->data[r];
	  r = r - width + 1;
	}
      return res;
    }
    //-------------------------------------------------------------------------------
    T * allLeftUp(unsigned int rank,unsigned int width)
    //-------------------------------------------------------------------------------
    {
      T * res = new T[R];
      unsigned int r = rank - width - 1;
      for(int i=0;i<R;i++)
	{
	  res[i] = this->data[r];
	  r = r - width - 1;
	}
      return res;
    }
    //-------------------------------------------------------------------------------
    T right(unsigned int rank)
    //-------------------------------------------------------------------------------
    {
      unsigned int r = rank + 1;
      return this->data[r];
    }
    //-------------------------------------------------------------------------------
    T left(unsigned int rank)
    //-------------------------------------------------------------------------------
    {
      unsigned int r = rank - 1;
      return this->data[r];
    }
    //-------------------------------------------------------------------------------
    T up(unsigned int rank,unsigned int width)
    //-------------------------------------------------------------------------------
    {
      unsigned int r = rank - width;
      return this->data[r];
    }
    //-------------------------------------------------------------------------------
    T down(unsigned int rank,unsigned int width)
    //-------------------------------------------------------------------------------
    {
      unsigned int r = rank + width;
      return this->data[r];
    }
    //-------------------------------------------------------------------------------
    T rightDown(unsigned int rank,unsigned int width)
    //-------------------------------------------------------------------------------
    {
      unsigned int r = rank + width +1;
      return this->data[r];
    }
    //-------------------------------------------------------------------------------
    T leftDown(unsigned int rank,unsigned int width)
    //-------------------------------------------------------------------------------
    {
      unsigned int r = rank + width -1;
      return this->data[r];
    }
    //-------------------------------------------------------------------------------
    T rightUp(unsigned int rank,unsigned int width)
    //-------------------------------------------------------------------------------
    {
      unsigned int r = rank - width +1;
      return this->data[r];
    }
    //-------------------------------------------------------------------------------
    T leftUp(unsigned int rank,unsigned int width)
    //-------------------------------------------------------------------------------
    {
      unsigned int r = rank - width -1;
      return this->data[r];
    }
    //-------------------------------------------------------------------------------
    T * allX(unsigned int rank)
    //-------------------------------------------------------------------------------
    {
      T * res = new T[R*2];
      unsigned int b = rank - R;
      for(int i=0;i<R;i++)
	{
	  res[i] = this->data[b];
	  b++;
	}
      b++;
      for(int i=R;i<2*R;i++)
	{
	  res[i] = this->data[b];
	  b++;
	}
      return res;
    }
    //-------------------------------------------------------------------------------
    void allX(unsigned int rank, T * t)
    //-------------------------------------------------------------------------------
    {
      unsigned int b = rank - R;
      for(int i=0;i<R;i++)
        {
          t[i] = this->data[b];
          b++;
        }
      b++;
      for(int i=R;i<2*R;i++)
        {
          t[i] = this->data[b];
          b++;
        }
    }
    //-------------------------------------------------------------------------------
    T * allY(unsigned int rank,unsigned int width)
    //-------------------------------------------------------------------------------
    {
      T * res = new T[R*2];
      unsigned int b = rank - R*width;

      for(int i=0;i<R;i++)
	{
	  res[i] = this->data[b];
	  b = b + width;
	}
      b = b + width;
      for(int i=R;i<2*R;i++)
	{
	  res[i] = this->data[b];
	  b = b + width;
	}
      return res;
    }
    //-------------------------------------------------------------------------------
    void allY(unsigned int rank,unsigned int width, T * t)
    //-------------------------------------------------------------------------------
    {
      unsigned int b = rank - R*width;

      for(int i=0;i<R;i++)
        {
          t[i] = this->data[b];
          b = b + width;
        }
      b = b + width;
      for(int i=R;i<2*R;i++)
        {
          t[i] = this->data[b];
          b = b + width;
        }
    }
    //-------------------------------------------------------------------------------
    T * eight(unsigned int rank,unsigned int width)
    //-------------------------------------------------------------------------------
    {
      T * res = new T[8];
      res[0] = leftUp(rank,width);
      res[1] = up(rank,width);
      res[2] = rightUp(rank,width);
      res[3] = right(rank);
      res[4] = rightDown(rank,width);
      res[5] = down(rank,width);
      res[6] = leftDown(rank,width);
      res[7] = left(rank);
      return res;
    }
    //-------------------------------------------------------------------------------
    T * four(unsigned int rank,unsigned int width)
    //-------------------------------------------------------------------------------
    {
      T * res = new T[8];
      res[0] = up(rank,width);
      res[1] = right(rank);
      res[2] = down(rank,width);
      res[3] = left(rank);
      return res;
    }
    //-------------------------------------------------------------------------------
    T inright(unsigned int rank)
    //-------------------------------------------------------------------------------
    {
      unsigned int re = rank%border_head.width;
      unsigned int r = rank + (R-re);
      return this->data[r];
    }
    //-------------------------------------------------------------------------------
    T inleft(unsigned int rank)
    //-------------------------------------------------------------------------------
    {
      unsigned int re = border_head.width - rank%border_head.width -1 ;
      unsigned int r = rank - (R-re);
      return this->data[r];
    }
    //-------------------------------------------------------------------------------
    T inup(unsigned int rank,unsigned int width)
    //-------------------------------------------------------------------------------
    {
      unsigned int re = border_head.height-floor(rank/border_head.width)-1;
      unsigned int r = rank - (R-re)*width;
      return this->data[r];
    }
    //-------------------------------------------------------------------------------
    T indown(unsigned int rank,unsigned int width)
    //-------------------------------------------------------------------------------
    {
      unsigned int re = floor(rank/border_head.width);
      unsigned int r = rank + (R-re)*width;
      return this->data[r];
    }
    //-------------------------------------------------------------------------------
    void indexesIt(unsigned int rank,int &col,int &li)
    //-------------------------------------------------------------------------------
    {
      unsigned int relative_rank = rank - border*border_head.width - (floor(rank/border_head.width)-border)*border*2 -border;
      li = floor(relative_rank/this->loc_head.width);
      col = relative_rank%this->loc_head.width;
    }
    //-------------------------------------------------------------------------------

  public:
    //-------------------------------------------------------------------------------
    DMatrix_impl() : DMatrix_base<T>(){}
    //-------------------------------------------------------------------------------

    //! constructor of the distributed matrix
    /*!
      This constructor will simply allocate the good space.
      \param h is the header to use to build your matrix
      \param value is the default value to put in the matrix
    */
    //-------------------------------------------------------------------------------
    DMatrix_impl(HEADER h,const T value) : DMatrix_base<T>()
    //-------------------------------------------------------------------------------
    {
      border = R;
      this->head=h;
      create();
      initValues(value);
      getNeighborRanks();
      phBLimits();
      //print();
    }
  
    //! constructor of the distributed matrix
    /*!
      The constructor evaluate how to divide the matrix in submatrice and do the work.
      \param binFile is the path of the binary file you want to work on
    */
    //-------------------------------------------------------------------------------
    DMatrix_impl(const char * binFile) : DMatrix_base<T>()
    //-------------------------------------------------------------------------------
    {
      border = R;
      this->head = Header::read(binFile);
      create();
      initValues(0);
      read(binFile);
      getNeighborRanks();
      phBLimits();
    }
    //-------------------------------------------------------------------------------
  
    //! destructor of the distributed matrix
    /*!  */
    //-------------------------------------------------------------------------------
    ~DMatrix_impl()
    //-------------------------------------------------------------------------------
    {
      if(NULL!=this->data)
	{
	  delete [] this->data;
	  this->data = NULL;
	}
    }
    //-------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------
    inline T getValue(iterator<T,R> it){return this->data[it._rank];}
    inline T getValue(iterator_cont<T,R> it){return this->data[it._rank];}
    inline T getValue(iterator_step<T,R> it){return this->data[it._rank];}
    inline T getValue(iterator_rev<T,R> it){return this->data[it._rank];}
    inline T getValue(iterator_phb_left<T,R> it){return this->data[it._rank];}
    inline T getValue(iterator_phb_right<T,R> it){return this->data[it._rank];}
    inline T getValue(iterator_phb_up<T,R> it){return this->data[it._rank];}
    inline T getValue(iterator_phb_down<T,R> it){return this->data[it._rank];}
    //-------------------------------------------------------------------------------
    inline T& getValueRef(iterator<T,R> it){return this->data[it._rank];}
    inline T& getValueRef(iterator_cont<T,R> it){return this->data[it._rank];}
    inline T& getValueRef(iterator_step<T,R> it){return this->data[it._rank];}
    inline T& getValueRef(iterator_rev<T,R> it){return this->data[it._rank];}
    inline T& getValueRef(iterator_phb_left<T,R> it){return this->data[it._rank];}
    inline T& getValueRef(iterator_phb_right<T,R> it){return this->data[it._rank];}
    inline T& getValueRef(iterator_phb_up<T,R> it){return this->data[it._rank];}
    inline T& getValueRef(iterator_phb_down<T,R> it){return this->data[it._rank];}
    //-------------------------------------------------------------------------------
    inline void setValue(iterator<T,R> it,T value){this->data[it._rank]=value;}
    inline void setValue(iterator_cont<T,R> it,T value){this->data[it._rank]=value;}
    inline void setValue(iterator_step<T,R> it,T value){this->data[it._rank]=value;}
    inline void setValue(iterator_rev<T,R> it,T value){this->data[it._rank]=value;}
    inline void setValue(iterator_phb_left<T,R> it,T value){this->data[it._rank]=value;}
    inline void setValue(iterator_phb_right<T,R> it,T value){this->data[it._rank]=value;}
    inline void setValue(iterator_phb_up<T,R> it,T value){this->data[it._rank]=value;}
    inline void setValue(iterator_phb_down<T,R> it,T value){this->data[it._rank]=value;}
    //-------------------------------------------------------------------------------
    //! Set value in the global middle
    /*!
      \param val is the value to affect
    */
    //-------------------------------------------------------------------------------
    void setGlobalMiddleValue(T val)
    //-------------------------------------------------------------------------------
    {
      if(this->row!=this->rows-1 || this->rows==1)
	{
	  unsigned int middle_h = floor(this->head.height/2)-1;
	  if(this->loc_head.height*this->row<=middle_h && this->loc_head.height*(this->row+1)>middle_h)
	    {
	      unsigned int j_loc = floor(this->head.width/2)-1;
	      unsigned int i_loc = middle_h-this->row*this->loc_head.height;
	      unsigned int rank = R*border_head.width +  i_loc*border_head.width + j_loc + R;
	      this->data[rank] = val;
	    }
	}
    }
    //-------------------------------------------------------------------------------
    //! to set all the values of the physical border of the matrix
    /*!
      \param val is the value to set
    */
    //-------------------------------------------------------------------------------
    inline void setPhysicalBorder(T val)
    {
      iterator_phb_left<T,R> left =begin_phb_left();
      iterator_phb_left<T,R> leftEnd = end_phb_left();
      for(left;left<leftEnd;++left)
	this->data[left._rank] = val;
      iterator_phb_right<T,R> right =begin_phb_right();
      iterator_phb_right<T,R> rightEnd = end_phb_right();
      for(right;right<rightEnd;++right)
	this->data[right._rank] = val;
      iterator_phb_up<T,R> up =begin_phb_up();
      iterator_phb_up<T,R> upEnd = end_phb_up();
      for(up;up<upEnd;++up)
	this->data[up._rank] = val;
      iterator_phb_down<T,R> down =begin_phb_down();
      iterator_phb_down<T,R> downEnd = end_phb_down();
      for(down;down<downEnd;++down)
	this->data[down._rank] = val;
    }
    //-------------------------------------------------------------------------------
    //! to set all the values on the right of physical border of the matrix
    /*!
      \param val is the value to set
    */
    //-------------------------------------------------------------------------------
    inline void setRightPhysicalBorder(T val)
    {
      iterator_phb_right<T,R> it =begin_phb_right();
      iterator_phb_right<T,R> itEnd = end_phb_right();
      for(it;it<itEnd;++it)
	this->data[it._rank] = val;
    }
    //-------------------------------------------------------------------------------
    //! to set all the values on the left of physical border of the matrix
    /*!
      \param val is the value to set
    */
    //-------------------------------------------------------------------------------
    inline void setLeftPhysicalBorder(T val)
    {
      iterator_phb_left<T,R> it =begin_phb_left();
      iterator_phb_left<T,R> itEnd = end_phb_left();
      for(it;it<itEnd;++it)
	this->data[it._rank] = val;
    }
    //-------------------------------------------------------------------------------
    //! to set all the values on the up of physical border of the matrix
    /*!
      \param val is the value to set
    */
    //-------------------------------------------------------------------------------
    inline void setUpPhysicalBorder(T val)
    {
      iterator_phb_up<T,R> it =begin_phb_up();
      iterator_phb_up<T,R> itEnd = end_phb_up();
      for(it;it<itEnd;++it)
	this->data[it._rank] = val;
    }
    //-------------------------------------------------------------------------------
    //! to set all the values on the down of physical border of the matrix
    /*!
      \param val is the value to set
    */
    //-------------------------------------------------------------------------------
    void setDownPhysicalBorder(T val)
    {
      iterator_phb_down<T,R> it =begin_phb_down();
      iterator_phb_down<T,R> itEnd = end_phb_down();
      for(it;it<itEnd;++it)
	this->data[it._rank] = val;
    }
    //-------------------------------------------------------------------------------
    inline iterator<T,R> begin(){return iterator<T,R>(border*this->border_head.width+border,this->border_head.width);}
    inline iterator_cont<T,R> begin_cont(){return iterator_cont<T,R>(border*this->border_head.width+border,this->border_head.width);}
    inline iterator_rev<T,R> begin_rev(){return iterator_rev<T,R>((this->border_head.width*this->border_head.height)-1-border*this->border_head.width-border,this->border_head.width);}
    inline iterator_step<T,R> begin_step(int step,int nb){return iterator_step<T,R>(border*this->border_head.width+border,this->border_head.width,step,nb);}
    inline iterator_phb_left<T,R> begin_phb_left(){return iterator_phb_left<T,R>(beginPhBLeft,this->border_head.width);}
    inline iterator_phb_right<T,R> begin_phb_right(){return iterator_phb_right<T,R>(beginPhBRight,this->border_head.width);}
    inline iterator_phb_up<T,R> begin_phb_up(){return iterator_phb_up<T,R>(beginPhBUp,this->border_head.width);}
    inline iterator_phb_down<T,R> begin_phb_down(){return iterator_phb_down<T,R>(beginPhBDown,this->border_head.width);}
    inline iterator_line<T,R> begin_line(){return iterator_line<T,R>(border*this->border_head.width+border,this->border_head.width);}
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    inline iterator<T,R> end(){return iterator<T,R>((this->border_head.width*this->border_head.height)-1-border*this->border_head.width-border,this->border_head.width);}
    inline iterator_cont<T,R> end_cont(){return iterator_cont<T,R>((this->border_head.width*this->border_head.height)-1-border*this->border_head.width-border,this->border_head.width);}
    inline iterator_rev<T,R> end_rev(){return iterator_rev<T,R>(border*this->border_head.width+border,this->border_head.width);}
    inline iterator_step<T,R> end_step(){return iterator_step<T,R>((this->border_head.width*this->border_head.height)-1-border*this->border_head.width-border,this->border_head.width);}
    inline iterator_phb_left<T,R> end_phb_left(){return iterator_phb_left<T,R>(endPhBLeft,this->border_head.width);}
    inline iterator_phb_right<T,R> end_phb_right(){return iterator_phb_right<T,R>(endPhBRight,this->border_head.width);}
    inline iterator_phb_up<T,R> end_phb_up(){return iterator_phb_up<T,R>(endPhBUp,this->border_head.width);}
    inline iterator_phb_down<T,R> end_phb_down(){return iterator_phb_down<T,R>(endPhBDown,this->border_head.width);}
    inline iterator_line<T,R> end_line(){return iterator_line<T,R>((this->border_head.width*this->border_head.height)-(border+1)*this->border_head.width+border,this->border_head.width);}
    //-------------------------------------------------------------------------------
    //! get the iterator on the matrix at position (x,y)
    /*!
      \param x is row index
      \pram y is column index
      \return returns the iterator at position (x,y)
    */
    iterator<T,R> getIterator(int col,int li)
    //-------------------------------------------------------------------------------
    {
      if(NULL!=this->data)
	{
	  unsigned int rank = border*border_head.width + border + li*border_head.width+col;
	  return iterator<T,R>(rank,this->border_head.width);
	}
    }
    //-------------------------------------------------------------------------------
    //! get the iterator contiguous on the matrix at position (x,y)
    /*!
      \param x is row index
      \pram y is column index
      \return returns the iterator at position (x,y)
    */
    iterator_cont<T,R> getIterator_cont(int col,int li)
    //-------------------------------------------------------------------------------
    {
      if(NULL!=this->data)
	{
	  unsigned int rank = border*border_head.width + border + li*border_head.width+col;
	  return iterator_cont<T,R>(rank,this->border_head.width);
	}
    }
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    //! get the iterator reverse on the matrix at position (x,y)
    /*!
      \param x is row index
      \pram y is column index
      \return returns the iterator at position (x,y)
    */
    iterator_rev<T,R> getIterator_rev(int col,int li)
    //-------------------------------------------------------------------------------
    {
      if(NULL!=this->data)
	{
	  unsigned int rank = border*border_head.width + border + li*border_head.width+col;
	  return iterator_rev<T,R>(rank,this->border_head.width);
	}
    }
    //-------------------------------------------------------------------------------
    //! get the indexes of the iterator on the matrix
    /*!
      \param it is the iterator from which the indexes will be taken
      \param x is the iterator position x returned
      \param y is the iterator position y returned
    */
    void getIndexes(iterator<T,R> it,int &col,int &li){indexesIt(it._rank,col,li);}
    void getIndexes(iterator_cont<T,R> it,int &col,int &li){indexesIt(it._rank,col,li);}
    void getIndexes(iterator_rev<T,R> it,int &col,int &li){indexesIt(it._rank,col,li);}
    //-------------------------------------------------------------------------------
    inline T& operator[](iterator<T,R> it) { return this->data[it._rank]; }
    inline T& operator[](iterator_cont<T,R> it) { return this->data[it._rank]; }
    inline T& operator[](iterator_rev<T,R> it) { return this->data[it._rank]; }
    inline T& operator[](iterator_step<T,R> it) { return this->data[it._rank]; }
    inline T& operator[](iterator_phb_left<T,R> it) { return this->data[it._rank]; }
    inline T& operator[](iterator_phb_right<T,R> it) { return this->data[it._rank]; }
    inline T& operator[](iterator_phb_up<T,R> it) { return this->data[it._rank]; }
    inline T& operator[](iterator_phb_down<T,R> it) { return this->data[it._rank]; }
    //-------------------------------------------------------------------------------

    //! Print the matrix by bloc
    /*!
     */
    //-------------------------------------------------------------------------------
    void printAll()
    //-------------------------------------------------------------------------------
    {
      std::stringstream st;
      for(unsigned int i=0;i<this->border_head.height;i++)
	{
	  for(unsigned int j=0;j<this->border_head.width;j++)
	    {
	      st<<this->data[i*this->border_head.width+j]<<" ";
	    }
	  st<<"\n";
	}
      Mpi_::print(st.str());
    }
    //-------------------------------------------------------------------------------

    //! Print the matrix by bloc
    /*!
     */
    //-------------------------------------------------------------------------------
    void print()
    //-------------------------------------------------------------------------------
    {
      std::stringstream st;
      for(unsigned int i=0;i<this->loc_head.height;i++)
        {
          for(unsigned int j=0;j<this->loc_head.width;j++)
            {
              st<<this->data[border*this->border_head.width + border + i*this->border_head.width+j]<<" ";
            }
          st<<"\n";
        }
      Mpi_::print(st.str());
    }
    //-------------------------------------------------------------------------------


    //! get borders
    /*!
      Get the borders from other processors
      MPI exchanges
    */
    //-------------------------------------------------------------------------------
    void getBorders()
    //-------------------------------------------------------------------------------
    {
#ifdef DEBUG
      std::stringstream st;
      st<<"MPI neighbors :\n";
      for(int i=0;i<2;i++)
	{
	  st<<mpi_ranks[i]<<",";
	}
      st<<"\n"<<"\n";
      Mpi_::print(st.str());
#endif
	  
      if(this->row!=0)
	{
#ifdef DEBUG
	  std::stringstream st;
	  st<<"Send and Rcv up values : MPI-"<<mpi_ranks[0]<<" : \n";
	  Mpi_::print(st.str());
#endif

	  T * toSend = getUpBorderToSend();
	  T * toGet = new T[this->loc_head.width*border];
	  Communications<T>::Exchanges(toSend,toGet,this->loc_head.width*border,mpi_ranks[0]);
	  setUpBorder(toGet);
	  delete [] toSend;
	  delete [] toGet;

	}
      if(this->row!=(this->rows-1))
	{				      
#ifdef DEBUG
	  std::stringstream st;
	  st<<"Send and Rcv down values : MPI-"<<mpi_ranks[1]<<" : \n";
	  Mpi_::print(st.str());
#endif

	  T * toSend = getDownBorderToSend();
	  T * toGet = new T[this->loc_head.width*border];
	  Communications<T>::Exchanges(toSend,toGet,this->loc_head.width*border,mpi_ranks[1]);
	  setDownBorder(toGet);
	  delete [] toSend;
	  delete [] toGet;
	}
    }
    //-------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------
    inline T * getAllRight(iterator<T,R> it){return allRight(it._rank);}
    inline T * getAllRight(iterator_cont<T,R> it){return allRight(it._rank);}
    inline T * getAllRight(iterator_step<T,R> it){return allRight(it._rank);}
    inline T * getAllRight(iterator_rev<T,R> it){return allRight(it._rank);}
    inline T * getAllRight(iterator_phb_left<T,R> it){return allRight(it._rank);}
    //-------------------------------------------------------------------------------
    inline T * getAllLeft(iterator<T,R> it){return allLeft(it._rank);}
    inline T * getAllLeft(iterator_cont<T,R> it){return allLeft(it._rank);}
    inline T * getAllLeft(iterator_step<T,R> it){return allLeft(it._rank);}
    inline T * getAllLeft(iterator_rev<T,R> it){return allLeft(it._rank);}
    inline T * getAllLeft(iterator_phb_right<T,R> it){return allLeft(it._rank);}
    //-------------------------------------------------------------------------------
    inline T * getAllUp(iterator<T,R> it){return allUp(it._rank,it._width);}
    inline T * getAllUp(iterator_cont<T,R> it){return allUp(it._rank,it._width);}
    inline T * getAllUp(iterator_step<T,R> it){return allUp(it._rank,it._width);}
    inline T * getAllUp(iterator_rev<T,R> it){return allUp(it._rank,it._width);}
    inline T * getAllUp(iterator_phb_down<T,R> it){return allUp(it._rank,it._width);}
    //-------------------------------------------------------------------------------
    inline T * getAllDown(iterator<T,R> it){return allDown(it._rank,it._width);}
    inline T * getAllDown(iterator_cont<T,R> it){return allDown(it._rank,it._width);}
    inline T * getAllDown(iterator_step<T,R> it){return allDown(it._rank,it._width);}
    inline T * getAllDown(iterator_rev<T,R> it){return allDown(it._rank,it._width);}
    inline T * getAllDown(iterator_phb_up<T,R> it){return allDown(it._rank,it._width);}
    //-------------------------------------------------------------------------------
    inline T * getAllRightDown(iterator<T,R> it){return allRightDown(it._rank,it._width);}
    inline T * getAllRightDown(iterator_cont<T,R> it){return allRightDown(it._rank,it._width);}
    inline T * getAllRightDown(iterator_step<T,R> it){return allRightDown(it._rank,it._width);}
    inline T * getAllRightDown(iterator_rev<T,R> it){return allRightDown(it._rank,it._width);}
    //-------------------------------------------------------------------------------
    inline T * getAllLeftDown(iterator<T,R> it){return allLeftDown(it._rank,it._width);}
    inline T * getAllLeftDown(iterator_cont<T,R> it){return allLeftDown(it._rank,it._width);}
    inline T * getAllLeftDown(iterator_step<T,R> it){return allLeftDown(it._rank,it._width);}
    inline T * getAllLeftDown(iterator_rev<T,R> it){return allLeftDown(it._rank,it._width);}
    //-------------------------------------------------------------------------------
    inline T * getAllRightUp(iterator<T,R> it){return allRightUp(it._rank,it._width);}
    inline T * getAllRightUp(iterator_cont<T,R> it){return allRightUp(it._rank,it._width);}
    inline T * getAllRightUp(iterator_step<T,R> it){return allRightUp(it._rank,it._width);}
    inline T * getAllRightUp(iterator_rev<T,R> it){return allRightUp(it._rank,it._width);}
    //-------------------------------------------------------------------------------
    inline T * getAllLeftUp(iterator<T,R> it){return allLeftUp(it._rank,it._width);}
    inline T * getAllLeftUp(iterator_cont<T,R> it){return allLeftUp(it._rank,it._width);}
    inline T * getAllLeftUp(iterator_step<T,R> it){return allLeftUp(it._rank,it._width);}
    inline T * getAllLeftUp(iterator_rev<T,R> it){return allLeftUp(it._rank,it._width);}
    //-------------------------------------------------------------------------------
    inline T getRight(iterator<T,R> it){return right(it._rank);}
    inline T getRight(iterator_cont<T,R> it){return right(it._rank);}
    inline T getRight(iterator_step<T,R> it){return right(it._rank);}
    inline T getRight(iterator_rev<T,R> it){return right(it._rank);}
    inline T getRight(iterator_phb_left<T,R> it){return right(it._rank);}
    //-------------------------------------------------------------------------------
    inline T getLeft(iterator<T,R> it){return left(it._rank);}
    inline T getLeft(iterator_cont<T,R> it){return left(it._rank);}
    inline T getLeft(iterator_step<T,R> it){return left(it._rank);}
    inline T getLeft(iterator_rev<T,R> it){return left(it._rank);}
    inline T getLeft(iterator_phb_right<T,R> it){return left(it._rank);}
    //-------------------------------------------------------------------------------
    inline T getUp(iterator<T,R> it){return up(it._rank,it._width);}
    inline T getUp(iterator_cont<T,R> it){return up(it._rank,it._width);}
    inline T getUp(iterator_step<T,R> it){return up(it._rank,it._width);}
    inline T getUp(iterator_rev<T,R> it){return up(it._rank,it._width);}
    inline T getUp(iterator_phb_down<T,R> it){return up(it._rank,it._width);}
    //-------------------------------------------------------------------------------
    inline T getDown(iterator<T,R> it){return down(it._rank,it._width);}
    inline T getDown(iterator_cont<T,R> it){return down(it._rank,it._width);}
    inline T getDown(iterator_step<T,R> it){return down(it._rank,it._width);}
    inline T getDown(iterator_rev<T,R> it){return down(it._rank,it._width);}
    inline T getDown(iterator_phb_up<T,R> it){return down(it._rank,it._width);}
    //-------------------------------------------------------------------------------
    inline T getRightDown(iterator<T,R> it){return rightDown(it._rank,it._width);}
    inline T getRightDown(iterator_cont<T,R> it){return rightDown(it._rank,it._width);}
    inline T getRightDown(iterator_step<T,R> it){return rightDown(it._rank,it._width);}
    inline T getRightDown(iterator_rev<T,R> it){return rightDown(it._rank,it._width);}
    //-------------------------------------------------------------------------------
    inline T getLeftDown(iterator<T,R> it){return leftDown(it._rank,it._width);}
    inline T getLeftDown(iterator_cont<T,R> it){return leftDown(it._rank,it._width);}
    inline T getLeftDown(iterator_step<T,R> it){return leftDown(it._rank,it._width);}
    inline T getLeftDown(iterator_rev<T,R> it){return leftDown(it._rank,it._width);}
    //-------------------------------------------------------------------------------
    inline T getRightUp(iterator<T,R> it){return rightUp(it._rank,it._width);}
    inline T getRightUp(iterator_cont<T,R> it){return rightUp(it._rank,it._width);}
    inline T getRightUp(iterator_step<T,R> it){return rightUp(it._rank,it._width);}
    inline T getRightUp(iterator_rev<T,R> it){return rightUp(it._rank,it._width);}
    //-------------------------------------------------------------------------------
    inline T getLeftUp(iterator<T,R> it){return leftUp(it._rank,it._width);}
    inline T getLeftUp(iterator_cont<T,R> it){return leftUp(it._rank,it._width);}
    inline T getLeftUp(iterator_step<T,R> it){return leftUp(it._rank,it._width);}
    inline T getLeftUp(iterator_rev<T,R> it){return leftUp(it._rank,it._width);}
    //-------------------------------------------------------------------------------
    inline T * getAllX(iterator<T,R> it){return allX(it._rank);}
    inline T * getAllX(iterator_cont<T,R> it){return allX(it._rank);}
    inline T * getAllX(iterator_step<T,R> it){return allX(it._rank);}
    inline T * getAllX(iterator_rev<T,R> it){return allX(it._rank);}
    //-------------------------------------------------------------------------------
    inline void getAllX(iterator<T,R> it, T * t){return allX(it._rank,t);}
    inline void getAllX(iterator_cont<T,R> it, T * t){return allX(it._rank,t);}
    inline void getAllX(iterator_step<T,R> it, T * t){return allX(it._rank,t);}
    inline void getAllX(iterator_rev<T,R> it, T * t){return allX(it._rank,t);}
    //-------------------------------------------------------------------------------
    inline T * getAllY(iterator<T,R> it){return allY(it._rank,it._width);}
    inline T * getAllY(iterator_cont<T,R> it){return allY(it._rank,it._width);}
    inline T * getAllY(iterator_step<T,R> it){return allY(it._rank,it._width);}
    inline T * getAllY(iterator_rev<T,R> it){return allY(it._rank,it._width);}
    //-------------------------------------------------------------------------------
    inline void getAllY(iterator<T,R> it, T * t){return allY(it._rank,it._width,t);}
    inline void getAllY(iterator_cont<T,R> it, T * t){return allY(it._rank,it._width,t);}
    inline void getAllY(iterator_step<T,R> it, T * t){return allY(it._rank,it._width,t);}
    inline void getAllY(iterator_rev<T,R> it, T * t){return allY(it._rank,it._width,t);}
    //-------------------------------------------------------------------------------
    inline T * get8(iterator<T,R> it){return eight(it._rank,it._width);}
    inline T * get8(iterator_cont<T,R> it){return eight(it._rank,it._width);}
    inline T * get8(iterator_step<T,R> it){return eight(it._rank,it._width);}
    inline T * get8(iterator_rev<T,R> it){return eight(it._rank,it._width);}
    //-------------------------------------------------------------------------------
    inline T * get4(iterator<T,R> it){return four(it._rank,it._width);}
    inline T * get4(iterator_cont<T,R> it){return four(it._rank,it._width);}
     inline T * get4(iterator_step<T,R> it){return four(it._rank,it._width);}
    inline T * get4(iterator_rev<T,R> it){return four(it._rank,it._width);}
    //-------------------------------------------------------------------------------
    inline T getInRight(iterator_phb_left<T,R> it){return inright(it._rank);}
    inline T getInLeft(iterator_phb_right<T,R> it){return inleft(it._rank);}
    inline T getInUp(iterator_phb_down<T,R> it){return inup(it._rank,it._width);}
    inline T getInDown(iterator_phb_up<T,R> it){return indown(it._rank,it._width);}
    //-------------------------------------------------------------------------------
    inline HEADER getGlobalHeader(){return this->head;}
    //-------------------------------------------------------------------------------
    inline HEADER getHeader(){return this->loc_head;}
    //-------------------------------------------------------------------------------
    inline HEADER getBorderHeader(){return this->border_head;}
    //-------------------------------------------------------------------------------
    inline int getCols(){return this->cols;}
    //-------------------------------------------------------------------------------
    inline int getRows(){return this->rows;}
    //-------------------------------------------------------------------------------
    inline int getCol(){return this->col;}
    //-------------------------------------------------------------------------------
    inline int getRow(){return this->row;}
    //-------------------------------------------------------------------------------
    inline T * getData(){return this->data;}
    inline unsigned int getWidth(){return this->head.width;}
    inline unsigned int getWidthLoc(){return this->loc_head.width;}
    inline unsigned int getHeightLoc(){return this->loc_head.height;}
    //-------------------------------------------------------------------------------
    inline int getBorder(){return this->border;}
    //-------------------------------------------------------------------------------
  };
  

  //================================================================================
  //================================================================================
  //!  specialization of base template Distributed Matrix Class without recovery
  /*!
    This template class defines the distributed matrix without recovery
    T = type of data in the matrix
  */
  //-------------------------------------------------------------------------------
  template<class T> struct DMatrix_impl<T,0,true> : DMatrix_base<T>
  //-------------------------------------------------------------------------------
  {
  protected:

    static const int border = 0;

    //-------------------------------------------------------------------------------
    void create()
    //-------------------------------------------------------------------------------
    {
      //division of the matrix in cols and rows
      this->cols = 1;
      this->rows = Mpi_::mpi_nb;
	
#ifdef DEBUG
      std::stringstream st;
      st<<"rows and cols :"<<this->rows<<","<<this->cols<<"\n";
      Mpi_::print(st.str());
#endif

      this->row=Mpi_::mpi_rank;
      this->col=0;
	
#ifdef DEBUG
      std::stringstream st3;
      st3<<"MPI row and col :"<<this->row<<","<<this->col<<"\n"<<"\n";
      Mpi_::print(st3.str());
#endif	

      this->loc_head.width=this->head.width;

      this-> remainderh = 0;
      //if the height of the matrix can be divided by the number of rows
      if(this->head.height%this->rows==0)
	this->loc_head.height=floor(this->head.height/this->rows);
      //else we add one to the height of the submatrix and the last row will have less elements
      else
	{
	  this->remainderh = this->head.height - floor(this->head.height/this->rows)*this->rows;

	  if(this->row<this->remainderh)
	    this->loc_head.height=floor(this->head.height/this->rows)+1;
	  else
	    this->loc_head.height=floor(this->head.height/this->rows);
	}
      
      //OLD VERSION DOESN4T WORK FOR EACH CASE
      //if the height of the matrix can be divided by the number of rows
      /*if(this->head.height%this->rows==0)
	this->loc_head.height=floor(this->head.height/this->rows);
      //else we add one to the height of the submatrix and the last row will have less elements
      else
	{
	  this->normal_height = floor(this->head.height/this->rows)+1;
	  this->last_height = this->head.height - (this->rows-1)*(floor(this->head.height/this->rows)+1);	
	  if(this->row!=this->rows-1)
	    this->loc_head.height=this->normal_height;
	  else
	    this->loc_head.height=this->last_height;
	    }*/
	
      this->loc_head.x=this->head.x;
      this->loc_head.y=this->head.y-this->row*this->loc_head.height*this->head.spacing;
      this->loc_head.spacing=this->head.spacing;
      this->loc_head.nodata=this->head.nodata;

#ifdef DEBUG
      std::stringstream st2;
      st2<<"Global width and height :"<<this->head.width<<","<<this->head.height<<"\n";
      Mpi_::print(st2.str());
#endif

#ifdef DEBUG
      std::stringstream st1;
      st1<<"Local width and height :"<<this->loc_head.width<<","<<this->loc_head.height<<"\n";
      Mpi_::print(st1.str());
#endif
      
      this->data = new T[this->loc_head.width*this->loc_head.height];
    }
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    void initValues(T value)
    //-------------------------------------------------------------------------------
    {
      for(unsigned int i=0;i<this->loc_head.width*this->loc_head.height;i++)
	{
	  this->data[i]=value;
	}
    }
    //-------------------------------------------------------------------------------
    //! Read the zone file concerned for the current MPI process
    /*!
      read the MPI prcess zone, and spread the data in the subMatrices OpenMP
      only one submatrice if OpenMP is disabled
      \param binFile is binary file to read
    */
    //-------------------------------------------------------------------------------
    void read(const char * binFile)
    //-------------------------------------------------------------------------------
    {
      //read the zone of the file for my own block of data
      std::ifstream f(binFile, std::ios::binary | std::ios::in);

      //move to the begining of the bloc of data concerned
      unsigned int mvline;
      if(Mpi_::mpi_rank<this->remainderh)
	mvline = sizeof(HEADER) + (this->row*this->loc_head.height*this->head.width)*sizeof(T);
      else
	mvline = sizeof(HEADER) + ((this->remainderh*(this->loc_head.height+1)+(this->row-this->remainderh)*this->loc_head.height)*this->head.width)*sizeof(T);

      f.seekg(mvline,std::ios::beg);

      //read by lines
      for(unsigned int j=0;j<this->loc_head.height;j++)
	{
	  /*if(j>0)
	    {
	    //move to the next line of the bloc
	    unsigned int mv = this->head.width - this->loc_head.width;
	    f.seekg(mv*sizeof(T),std::ios::cur);
	    }*/

	  for(unsigned int k=0;k<this->loc_head.width;k++)
	    {
	      T val;
	      unsigned int rank = j*this->loc_head.width + k;
	      f.read(reinterpret_cast<char*>(&val),sizeof(T));
	      this->data[rank]=val;
	    }
	}
    }
    //-------------------------------------------------------------------------------
    void indexesIt(unsigned int rank,int &col,int &li)
    //-------------------------------------------------------------------------------
    {
      li = floor(rank/this->loc_head.width);
      col = rank%this->loc_head.width;
    }
    //-------------------------------------------------------------------------------

  public:
    //! constructor of the distributed matrix
    /*!
      This constructor will simply allocate the good space.
      \param h is the header to use to build your matrix
      \param value is the default value to put in the matrix
    */
    //-------------------------------------------------------------------------------
    DMatrix_impl(HEADER h,const T value) : DMatrix_base<T>()
    //-------------------------------------------------------------------------------
    {
      this->head=h;
      create();
      initValues(value);
      //print();
    }
  
    //! constructor of the distributed matrix
    /*!
      The constructor evaluate how to divide the matrix in submatrice and do the work.
      \param binFile is the path of the binary file you want to work on
    */
    //-------------------------------------------------------------------------------
    DMatrix_impl(const char * binFile) : DMatrix_base<T>()
    //-------------------------------------------------------------------------------
    {
      this->head = Header::read(binFile);
      create();
      initValues(0);
      read(binFile);
      //print();
    }
    //-------------------------------------------------------------------------------
  
    //! destructor of the distributed matrix
    /*!  */
    //-------------------------------------------------------------------------------
    ~DMatrix_impl()
    //-------------------------------------------------------------------------------
    {
      if(NULL!=this->data)
	delete [] this->data;
    }
    //-------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------
    inline T getValue(iterator<T,0> it){return this->data[it._rank];}
    inline T getValue(iterator_cont<T,0> it){return this->data[it._rank];}
    inline T getValue(iterator_step<T,0> it){return this->data[it._rank];}
    inline T getValue(iterator_rev<T,0> it){return this->data[it._rank];}
    //-------------------------------------------------------------------------------
    inline T& getValueRef(iterator<T,0> it){return this->data[it._rank];}
    inline T& getValueRef(iterator_cont<T,0> it){return this->data[it._rank];}
    inline T& getValueRef(iterator_step<T,0> it){return this->data[it._rank];}
    inline T& getValueRef(iterator_rev<T,0> it){return this->data[it._rank];}
    //-------------------------------------------------------------------------------
    inline void setValue(iterator<T,0> it,T value){this->data[it._rank]=value;}
    inline void setValue(iterator_cont<T,0> it,T value){this->data[it._rank]=value;}
    inline void setValue(iterator_step<T,0> it,T value){this->data[it._rank]=value;}
    inline void setValue(iterator_rev<T,0> it,T value){this->data[it._rank]=value;}
    //-------------------------------------------------------------------------------
    inline iterator<T,0> begin(){return iterator<T,0>(0,this->loc_head.width);}
    inline iterator_cont<T,0> begin_cont(){return iterator_cont<T,0>(0,this->loc_head.width);}
    inline iterator_step<T,0> begin_step(int step,int nb){return iterator_step<T,0>(0,step,nb);}
    inline iterator_rev<T,0> begin_rev(){return iterator_rev<T,0>(this->loc_head.width*this->loc_head.height-1,this->loc_head.width);}
    inline iterator_line<T,0> begin_line(){return iterator_line<T,0>(0,this->loc_head.width);}
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    inline iterator<T,0> end(){return iterator<T,0>(this->loc_head.width*this->loc_head.height-1,this->loc_head.width);}
    inline iterator_cont<T,0> end_cont(){return iterator_cont<T,0>(this->loc_head.width*this->loc_head.height-1,this->loc_head.width);}
    inline iterator_step<T,0> end_step(){return iterator_step<T,0>(this->loc_head.width*this->loc_head.height-1,this->loc_head.width);}
    inline iterator_rev<T,0> end_rev(){return iterator_rev<T,0>(0,this->loc_head.width);}
    inline iterator_line<T,0> end_line(){return iterator_line<T,0>(this->loc_head.width*this->loc_head.height-this->loc_head.width,this->loc_head.width);}
    //-------------------------------------------------------------------------------
    //! get the iterator on the matrix at position (x,y)
    /*!
      \param x is row index
      \pram y is column index
      \return returns the iterator at position (x,y)
    */
    iterator<T,0> getIterator(int col,int li)
    //-------------------------------------------------------------------------------
    {
      if(NULL!=this->data)
	{
	  unsigned int rank =li*this->loc_head.width+col;
	  return iterator<T,0>(rank,this->loc_head.width);
	}
    }
    //-------------------------------------------------------------------------------
    //! get the iterator contiguous on the matrix at position (x,y)
    /*!
      \param x is row index
      \pram y is column index
      \return returns the iterator at position (x,y)
    */
    iterator_cont<T,0> getIterator_cont(int col,int li)
    //-------------------------------------------------------------------------------
    {
      if(NULL!=this->data)
	{
	  unsigned int rank = li*this->loc_head.width+col;
	  return iterator_cont<T,0>(rank,this->loc_head.width);
	}
    }
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    //! get the iterator reverse on the matrix at position (x,y)
    /*!
      \param x is row index
      \pram y is column index
      \return returns the iterator at position (x,y)
    */
    iterator_rev<T,0> getIterator_rev(int col,int li)
    //-------------------------------------------------------------------------------
    {
      if(NULL!=this->data)
	{
	  unsigned int rank = li*this->loc_head.width+col;
	  return iterator_rev<T,0>(rank,this->loc_head.width);
	}
    }
    //-------------------------------------------------------------------------------
    //! get the indexes of the iterator on the matrix
    /*!
      \param it is the iterator from which the indexes will be taken
      \param x is the iterator position x returned
      \param y is the iterator position y returned
    */
    void getIndexes(iterator<T,0> it,int &col,int &li){indexesIt(it._rank,col,li);}
    void getIndexes(iterator_cont<T,0> it,int &col,int &li){indexesIt(it._rank,col,li);}
    void getIndexes(iterator_rev<T,0> it,int &col,int &li){indexesIt(it._rank,col,li);}
    //-------------------------------------------------------------------------------
    T& operator[](iterator<T,0> it) { return this->data[it._rank]; }
    T& operator[](iterator_cont<T,0> it) { return this->data[it._rank]; }
    T& operator[](iterator_step<T,0> it) { return this->data[it._rank]; }
    T& operator[](iterator_rev<T,0> it) { return this->data[it._rank]; }
    //-------------------------------------------------------------------------------

    //! Print the matrix by bloc
    /*!
     */
    //-------------------------------------------------------------------------------
    void print()
    //-------------------------------------------------------------------------------
    {
      std::stringstream st;
      for(unsigned int i=0;i<this->loc_head.height;i++)
	{
	  for(unsigned int j=0;j<this->loc_head.width;j++)
	    {
	      st<<this->data[border*this->loc_head.width + border + i*this->loc_head.width+j]<<" ";
	    }
	  st<<"\n";
	}
      Mpi_::print(st.str());
    }

    //-------------------------------------------------------------------------------
    inline HEADER getGlobalHeader(){return this->head;}
    //-------------------------------------------------------------------------------
    inline HEADER getHeader(){return this->loc_head;}
    //-------------------------------------------------------------------------------
    inline int getCols(){return this->cols;}
    //-------------------------------------------------------------------------------
    inline int getRows(){return this->rows;}
    //-------------------------------------------------------------------------------
    inline int getCol(){return this->col;}
    //-------------------------------------------------------------------------------
    inline int getRow(){return this->row;}
    //-------------------------------------------------------------------------------
    inline T * getData(){return this->data;}
    inline unsigned int getWidth(){return this->head.width;}
    inline unsigned int getWidthLoc(){return this->loc_head.width;}
    inline unsigned int getHeightLoc(){return this->loc_head.height;}
    inline int getBorder(){return 0;}
    //-------------------------------------------------------------------------------
  };
  //-------------------------------------------------------------------------------
  //================================================================================
  //================================================================================
  //!  base template Distributed Matrix Class with recovery
  /*!
    This template class defines the base distributed matrix with recovery
    T = type of data in the matrix
  */
  //-------------------------------------------------------------------------------
  template<class T,int R> struct DMatrix_impl<T,R,false> : public DMatrix_base<T>
  //-------------------------------------------------------------------------------
  {
  protected:

    int border;
    //! Header defining the current local matrix with border
    HEADER border_head;
    //! ranks of MPI neighbors
    int mpi_ranks[8];
    unsigned int beginPhBLeft,beginPhBRight,beginPhBUp,beginPhBDown;
    unsigned int endPhBLeft,endPhBRight,endPhBUp,endPhBDown;
    //physical border
    unsigned int swphbL,swphbR,swphbU,swphbD;
    unsigned int wphbL,wphbR,wphbU,wphbD;
    unsigned int shphbL,shphbR,shphbU,shphbD;
    unsigned int hphbL,hphbR,hphbU,hphbD;

    //-------------------------------------------------------------------------------
    void create()
    //-------------------------------------------------------------------------------
    {
      //division of the matrix in cols and rows
      this->cols = floor(sqrt(Mpi_::mpi_nb));
      int nb = 1;
      while(Mpi_::mpi_nb%this->cols!=0)
	{
	  this->cols = floor(sqrt(Mpi_::mpi_nb))-nb;
	  nb++;
	}
      this->rows = Mpi_::mpi_nb/this->cols;
	
#ifdef DEBUG
      std::stringstream st;
      st<<"rows and cols :"<<this->rows<<","<<this->cols<<"\n";
      Mpi_::print(st.str());
#endif

      this->row=floor(Mpi_::mpi_rank/this->cols);
      this->col=Mpi_::mpi_rank%this->cols;
	
#ifdef DEBUG
      std::stringstream st3;
      st3<<"MPI row and col :"<<this->row<<","<<this->col<<"\n"<<"\n";
      Mpi_::print(st3.str());
#endif	

      this-> remainderh = 0;
      //if the height of the matrix can be divided by the number of rows
      if(this->head.height%this->rows==0)
	this->loc_head.height=floor(this->head.height/this->rows);
      //else we add one to the height of the submatrix and the last row will have less elements
      else
	{
	  this->remainderh = this->head.height - floor(this->head.height/this->rows)*this->rows;

	  if(this->row<this->remainderh)
	    this->loc_head.height=floor(this->head.height/this->rows)+1;
	  else
	    this->loc_head.height=floor(this->head.height/this->rows);
	}

      this-> remainderw = 0;
      //if the height of the matrix can be divided by the number of rows
      if(this->head.width%this->cols==0)
	this->loc_head.width=floor(this->head.width/this->cols);
      //else we add one to the height of the submatrix and the last row will have less elements
      else
	{
	  this->remainderw = this->head.width - floor(this->head.width/this->cols)*this->cols;

	  if(this->col<this->remainderw)
	    this->loc_head.width=floor(this->head.width/this->cols)+1;
	  else
	    this->loc_head.width=floor(this->head.width/this->cols);
	}

      //if the width of the matrix can be divided by the number of cols
      /*if(this->head.width%this->cols==0)
	this->loc_head.width=floor(this->head.width/this->cols);
      //else we add one to the width of the submatrix and the last column will have less elements
      else
	{
	  this->normal_width = floor(this->head.width/this->cols)+1;
	  this->last_width = this->head.width - (this->cols-1)*(floor(this->head.width/this->cols)+1);
	  if(this->col!=this->cols-1)
	    this->loc_head.width=this->normal_width;
	  else
	    this->loc_head.width=this->last_width;
	}
      //if the height of the matrix can be divided by the number of rows
      if(this->head.height%this->rows==0)
	this->loc_head.height=floor(this->head.height/this->rows);
      //else we add one to the height of the submatrix and the last row will have less elements
      else
	{
	  this->normal_height = floor(this->head.height/this->rows)+1;
	  this->last_height = this->head.height - (this->rows-1)*(floor(this->head.height/this->rows)+1);	
	  if(this->row!=this->rows-1)
	    this->loc_head.height=this->normal_height;
	  else
	    this->loc_head.height=this->last_height;
	    }*/
	
      this->loc_head.x=this->head.x+this->col*this->loc_head.width*this->head.spacing;
      this->loc_head.y=this->head.y-this->row*this->loc_head.height*this->head.spacing;
      this->loc_head.spacing=this->head.spacing;
      this->loc_head.nodata=this->head.nodata;

      this->border_head.x = this->loc_head.x - border*this->loc_head.spacing;
      this->border_head.y = this->loc_head.y - border*this->loc_head.spacing;
      this->border_head.width = this->loc_head.width + border*2;
      this->border_head.height = this->loc_head.height + border*2;

#ifdef DEBUG
      std::stringstream st2;
      st2<<"Global width and height :"<<this->head.width<<","<<this->head.height<<"\n";
      Mpi_::print(st2.str());
#endif

#ifdef DEBUG
      std::stringstream st1;
      st1<<"Local width and height :"<<this->loc_head.width<<","<<this->loc_head.height<<"\n";
      Mpi_::print(st1.str());
#endif
      this->data = new T[this->border_head.width*this->border_head.height];
    }
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    void initValues(T value)
    //-------------------------------------------------------------------------------
    {
      for(unsigned int i=0;i<this->border_head.width*this->border_head.height;i++)
	{
	  this->data[i]=value;
	}
    }
    //-------------------------------------------------------------------------------
    //void phBLimits()
    //-------------------------------------------------------------------------------
    /*{
      if(this->col==0)
	{
	  beginPhBLeft = R*this->border_head.width;
	  endPhBLeft = this->border_head.width*(this->border_head.height-R-1)+R;
	}
      else
	{
	  beginPhBLeft = 0;
	  endPhBLeft = 0;
	}

      if(this->col==this->cols-1)
	{
	  beginPhBRight = (R+1)*this->border_head.width-R;
	  endPhBRight = (R+1+this->loc_head.height-1)*this->border_head.width;
	}
      else
	{
	  beginPhBRight = 0;
	  endPhBRight = 0;
	}

      if(this->row==0)
	{
	  beginPhBUp = R;
	  endPhBUp = R*this->border_head.width-R;
	}
      else
	{
	  beginPhBUp = 0;
	  endPhBUp = 0;
	}

      if(this->row==this->rows-1)
	{
	  beginPhBDown = this->border_head.width*(this->loc_head.height+R)+R;
	  endPhBDown = this->border_head.width*this->border_head.height-R;
	}
      else
	{
	  beginPhBDown = 0;
	  endPhBDown = 0;
	}
	}*/

    void phBLimits()
    //-------------------------------------------------------------------------------
    {
      if(this->col==0) //left
  {
    swphbL = 0; 
    wphbL = R;
    shphbL = R;
    hphbL = this->border_head.height-2*R;//border+(height-2*border);
  }
      else
  swphbL = wphbL = shphbL = hphbL = 0;

      if(this->col==this->cols-1) //right
  {
    swphbR = this->border_head.width-2*R;//border+(width-2*border); 
    //wphbR = this->border_head.width;//border+(width-2*border)+border;
    wphbR = R;
    shphbR = R;
    hphbR = this->border_head.height-2*R;//border+(height-2*border);
  }
      else
  swphbR = wphbR = shphbR = hphbR = 0;

      if(this->row==0) //up
  {
    swphbU = R; 
    wphbU = this->border_head.width-2*R;//border+(width-2*border);
    shphbU = 0;
    hphbU = R;
  }
      else
  swphbU = wphbU = shphbU = hphbU = 0;

      if(this->row==this->rows-1) //down
  {
    swphbD = R; 
    wphbD = this->border_head.width-2*R;//border+(width-2*border);
    shphbD = this->border_head.height-2*R;//border+(height-2*border);
    hphbD = this->border_head.height-2*R;
  }
      else
  swphbD = wphbD = shphbD = hphbD = 0;
    }
    //-------------------------------------------------------------------------------
    //! Read the zone file concerned for the current MPI process
    /*!
      read the MPI prcess zone, and spread the data in the subMatrices OpenMP
      only one submatrice if OpenMP is disabled
      \param binFile is binary file to read
    */
    //-------------------------------------------------------------------------------
    void read(const char * binFile)
    //-------------------------------------------------------------------------------
    {
      //read the zone of the file for my own block of data
      std::ifstream f(binFile, std::ios::binary | std::ios::in);

      //move to the begining of the bloc of data concerned
      unsigned int mvline;
      if(this->row<this->remainderh && this->col<this->remainderw)
	mvline = sizeof(HEADER) + (this->col*this->loc_head.width
				   +this->row*this->loc_head.height*this->head.width)*sizeof(T);
      else if(this->row<this->remainderh)
	mvline = sizeof(HEADER) + (this->remainderw*(this->loc_head.width+1)+(this->col-this->remainderw)*this->loc_head.width
				   +(this->row*this->loc_head.height)
				   *this->head.width)*sizeof(T);
      else if(this->col<this->remainderw)
	mvline = sizeof(HEADER) + (this->col*this->loc_head.width
				   +(this->remainderh*(this->loc_head.height+1)+(this->row-this->remainderh)*this->loc_head.height)
				   *this->head.width)*sizeof(T);
      else
	mvline = sizeof(HEADER) + (this->remainderw*(this->loc_head.width+1)+(this->col-this->remainderw)*this->loc_head.width
				   +(this->remainderh*(this->loc_head.height+1)+(this->row-this->remainderh)*this->loc_head.height)
				   *this->head.width)*sizeof(T);

      f.seekg(mvline,std::ios::beg);

      //read by lines
      for(unsigned int j=0;j<this->loc_head.height;j++)
	{
	  if(j>0)
	    {
	      //move to the next line of the bloc
	      unsigned int mv = this->head.width - this->loc_head.width;
	      f.seekg(mv*sizeof(T),std::ios::cur);
	    }

	  for(unsigned int k=0;k<this->loc_head.width;k++)
	    {
	      T val;
	      unsigned int rank = border*this->border_head.width + j*this->border_head.width + k + border;
	      f.read(reinterpret_cast<char*>(&val),sizeof(T));
	      this->data[rank]=val;
	    }
	}
    }
    //-------------------------------------------------------------------------------

    //! get rank from a column number and a row number
    /*!
      \param column is the column number
      \param columns is the total number of columns
      \param row is the row number
      \return the rank
    */
    //-------------------------------------------------------------------------------
    unsigned int getRank(int c, int cs, int r)
    //-------------------------------------------------------------------------------
    {
      unsigned int res = r*cs+c;
      return res;
    }
    //-------------------------------------------------------------------------------

    //! get ranks
    /*!
      Get the ranks of the 8 neighbors for MPI exchanges
      \return 
      if the value is -55555 then there is no neighbor in this direction
      if the value is >0 it represent the MPI rank concerned by the exchange
    */
    //-------------------------------------------------------------------------------
    void getNeighborRanks()
    //-------------------------------------------------------------------------------
    {
      for(int i=0;i<8;i++)
	mpi_ranks[i] = -55555;
      //up
      if((this->row-1)>=0)
	mpi_ranks[1] = getRank(this->col,this->cols,(this->row-1));
      //down
      if((this->row+1)<this->rows)
	mpi_ranks[5] = getRank(this->col,this->cols,(this->row+1));
      //right
      if((this->col+1)<this->cols)
	mpi_ranks[3] = getRank((this->col+1),this->cols,this->row);
      //left
      if((this->col-1)>=0)
	mpi_ranks[7] = getRank((this->col-1),this->cols,this->row);
      //up left
      if((this->col-1)>=0 && (this->row-1)>=0)
	mpi_ranks[0] = getRank((this->col-1),this->cols,(this->row-1));
      //up right
      if((this->col+1)<this->cols && (this->row-1)>=0)
	mpi_ranks[2] = getRank((this->col+1),this->cols,(this->row-1));
      //down left
      if((this->col-1)>=0 && (this->row+1)<this->rows)
	mpi_ranks[6] = getRank((this->col-1),this->cols,(this->row+1));
      //down right
      if((this->col+1)<this->cols && (this->row+1)<this->rows)
	mpi_ranks[4] = getRank((this->col+1),this->cols,(this->row+1));
    }
    //-------------------------------------------------------------------------------

    //! get the right border to send to neighbor processors
    /*!
      \return list of values
    */
    //-------------------------------------------------------------------------------
    T * getRightBorderToSend()
    //-------------------------------------------------------------------------------
    {
      T * result = new T[this->loc_head.height*border];
      for(unsigned int i=0;i<this->loc_head.height;i++)
	{
	  for(int j=0;j<border;j++)
	    {
	      unsigned int r1 = i*border+j;
	      unsigned int r2 = ((border+1)*border_head.width - border*2) + i*border_head.width +j;
	      result[r1] = this->data[r2];
	    }
	}
      return result;
    }
    //-------------------------------------------------------------------------------
    //! set the right border ghost cells with neighbor values from processors
    /*!
      \param values is the table values to fill ghost cells with
    */
    //-------------------------------------------------------------------------------
    void setRightBorder(T * values)
    //-------------------------------------------------------------------------------
    {
      for(unsigned int i=0;i<this->loc_head.height;i++)
	{
	  for(int j=0;j<border;j++)
	    this->data[(border_head.width*border + border_head.width - border) + i*border_head.width + j] = values[i*border+j];
	}
    }
    //-------------------------------------------------------------------------------
    //! get the left border to send to neighbor processors
    /*!
      \return list of values
    */
    //-------------------------------------------------------------------------------
    T * getLeftBorderToSend()
    //-------------------------------------------------------------------------------
    {
      T * result = new T[this->loc_head.height*border];
      for(unsigned int i=0;i<this->loc_head.height;i++)
	{
	  for(int j=0;j<border;j++)
	    {
	      unsigned int r1 = i*border+j;
	      unsigned int r2 = (border*border_head.width + border) + i*border_head.width + j;
	      result[r1] = this->data[r2];
	    }
	}
      return result;
    }
    //-------------------------------------------------------------------------------
    //! set the left border ghost cells with neighbor values from processors
    /*!
      \param values is the table values to fill ghost cells with
    */
    //-------------------------------------------------------------------------------
    void setLeftBorder(T * values)
    //-------------------------------------------------------------------------------
    {
      for(unsigned int i=0;i<this->loc_head.height;i++)
	{
	  for(int j=0;j<border;j++)
	    this->data[(border_head.width*border) + i*border_head.width + j] = values[i*border+j];
	}
    }
    //-------------------------------------------------------------------------------
    //! get the up border to send to neighbor processors
    /*!
      \return list of values
    */
    //-------------------------------------------------------------------------------
    T * getUpBorderToSend()
    //-------------------------------------------------------------------------------
    {
      T * result = new T[this->loc_head.width*border];
      for(int i=0;i<border;i++)
	{
	  for(unsigned int j=0;j<this->loc_head.width;j++)
	    {
	      unsigned int r1 = i*this->loc_head.width+j;
	      unsigned int r2 = (border*border_head.width + border) + i*border_head.width+j;
	      result[r1] = this->data[r2];
	    }
	}
      return result;
    }
    //-------------------------------------------------------------------------------
    //! set the up border ghost cells with neighbor values from processors
    /*!
      \param values is the table values to fill ghost cells with
    */
    //-------------------------------------------------------------------------------
    void setUpBorder(T * values)
    //-------------------------------------------------------------------------------
    {
      for(int i=0;i<border;i++)
	{
	  for(unsigned int j=0;j<this->loc_head.width;j++)
	    this->data[(border) + i*border_head.width+j] = values[i*this->loc_head.width+j];
	}
    }
    //-------------------------------------------------------------------------------
    //! get the down border to send to neighbor processors
    /*!
      \return list of values
    */
    //-------------------------------------------------------------------------------
    T * getDownBorderToSend()
    //-------------------------------------------------------------------------------
    {
      T * result = new T[this->loc_head.width*border];
      for(int i=0;i<border;i++)
	{
	  for(unsigned int j=0;j<this->loc_head.width;j++)
	    {
	      unsigned int r1 = i*this->loc_head.width+j;
	      unsigned int r2 = (border_head.height*border_head.width - (border+border)*border_head.width + border) + i*border_head.width + j;
	      result[r1] = this->data[r2];
	    }
	}
      return result;
    }
    //-------------------------------------------------------------------------------
    //! set the down border ghost cells with neighbor values from processors
    /*!
      \param values is the table values to fill ghost cells with
    */
    //-------------------------------------------------------------------------------
    void setDownBorder(T * values)
    //-------------------------------------------------------------------------------
    {
      for(int i=0;i<border;i++)
	{
	  for(unsigned int j=0;j<this->loc_head.width;j++)
	    this->data[(border_head.width*border_head.height - border_head.width*border + border) + i*border_head.width +j] = values[i*this->loc_head.width+j];
	}
    }
    //-------------------------------------------------------------------------------
    //! get the down right border to send to neighbor processors
    /*!
      \return list of values
    */
    //-------------------------------------------------------------------------------
    T * getDownRightBorderToSend()
    //-------------------------------------------------------------------------------
    {
      T * result = new T[border*border];
      for(int i=0;i<border;i++)
	{
	  for(int j=0;j<border;j++)
	    {
	      unsigned int r1 = i*border+j;
	      unsigned int r2 = (border_head.height*border_head.width - (border+border-1)*border_head.width - border*2) + i*border_head.width +j;
	      result[r1] = this->data[r2];
	    }
	}
      return result;
    }
    //-------------------------------------------------------------------------------
    //! set the down right border ghost cells with neighbor values from processors
    /*!
      \param values is the table values to fill ghost cells with
    */
    //-------------------------------------------------------------------------------
    void setDownRightBorder(T * values)
    //-------------------------------------------------------------------------------
    {
      for(int i=0;i<border;i++)
	{
	  for(int j=0;j<border;j++)
	    this->data[(border_head.width*border_head.height - (border-1)*border_head.width - border) + i*border_head.width + j] = values[i*border+j];
	}
    }
    //-------------------------------------------------------------------------------
    //! get the down left border to send to neighbor processors
    /*!
      \return list of values
    */
    //-------------------------------------------------------------------------------
    T * getDownLeftBorderToSend()
    //-------------------------------------------------------------------------------
    {
      T * result = new T[border*border];
      for(int i=0;i<border;i++)
	{
	  for(int j=0;j<border;j++)
	    {
	      unsigned int r1 = i*border+j;
	      unsigned int r2 = (border_head.height*border_head.width - (border+border)*border_head.width + border) + i*border_head.width + j;
	      result[r1] = this->data[r2];
	    }
	}
      return result;
    }
    //-------------------------------------------------------------------------------
    //! set the down left border ghost cells with neighbor values from processors
    /*!
      \param values is the table values to fill ghost cells with
    */
    //-------------------------------------------------------------------------------
    void setDownLeftBorder(T * values)
    //-------------------------------------------------------------------------------
    {
      for(int i=0;i<border;i++)
	{
	  for(int j=0;j<border;j++)
	    this->data[(border_head.width*border_head.height - border_head.width*border) + i*border_head.width +j] = values[i*border+j];
	}
    }
    //-------------------------------------------------------------------------------
    //! get the up left border to send to neighbor processors
    /*!
      \return list of values
    */
    //-------------------------------------------------------------------------------
    T * getUpLeftBorderToSend()
    //-------------------------------------------------------------------------------
    {
      T * result = new T[border*border];
      for(int i=0;i<border;i++)
	{
	  for(int j=0;j<border;j++)
	    {
	      unsigned int r1 = i*border+j;
	      unsigned int r2 = (border_head.width*border + border) + i*border_head.width + j;
	      result[r1] = this->data[r2];
	    }
	}
      return result;
    }
    //-------------------------------------------------------------------------------
    //! set the up left border ghost cells with neighbor values from processors
    /*!
      \param values is the table values to fill ghost cells with
    */
    //-------------------------------------------------------------------------------
    void setUpLeftBorder(T * values)
    //-------------------------------------------------------------------------------
    {
      for(int i=0;i<border;i++)
	{
	  for(int j=0;j<border;j++)
	    this->data[i*border_head.width+j] = values[i*border+j];
	}
    }
    //-------------------------------------------------------------------------------
    //! get the up right border to send to neighbor processors
    /*!
      \return list of values
    */
    //-------------------------------------------------------------------------------
    T * getUpRightBorderToSend()
    //-------------------------------------------------------------------------------
    {
      T * result = new T[border*border];
      for(int i=0;i<border;i++)
	{
	  for(int j=0;j<border;j++)
	    {
	      unsigned int r1 = i*border+j;
	      unsigned int r2 = ((border+1)*border_head.width - border*2) + i*border_head.width +j;
	      result[r1] = this->data[r2];
	    }
	}
      return result;
    }
    //-------------------------------------------------------------------------------
    //! set the up right border ghost cells with neighbor values from processors
    /*!
      \param values is the table values to fill ghost cells with
    */
    //-------------------------------------------------------------------------------
    void setUpRightBorder(T * values)
    //-------------------------------------------------------------------------------
    {
      for(int i=0;i<border;i++)
	{
	  for(int j=0;j<border;j++)
	    this->data[(border_head.width-border) + i*border_head.width+j] = values[i*border+j];
	}
    }
    //-------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------
    T * allRight(unsigned int rank)
    //-------------------------------------------------------------------------------
    {
      T * res = new T[R];
      unsigned int r = rank+1;
      for(int i=0;i<R;i++)
	{
	  res[i] = this->data[r];
	  r++;
	}
      return res;
    }
    //-------------------------------------------------------------------------------
    T * allLeft(unsigned int rank)
    //-------------------------------------------------------------------------------
    {
      T * res = new T[R];
      unsigned int r = rank-1;
      for(int i=0;i<R;i++)
	{
	  res[i] = this->data[r];
	  r--;
	}
      return res;
    }
    //-------------------------------------------------------------------------------
    T * allUp(unsigned int rank,unsigned int width)
    //-------------------------------------------------------------------------------
    {
      T * res = new T[R];
      unsigned int r = rank - width;
      for(int i=0;i<R;i++)
	{
	  res[i] = this->data[r];
	  r = r - width;
	}
      return res;
    }
    //-------------------------------------------------------------------------------
    T * allDown(unsigned int rank,unsigned int width)
    //-------------------------------------------------------------------------------
    {
      T * res = new T[R];
      unsigned int r = rank + width;
      for(int i=0;i<R;i++)
	{
	  res[i] = this->data[r];
	  r = r + width;
	}
      return res;
    }
    //-------------------------------------------------------------------------------
    T * allRightDown(unsigned int rank,unsigned int width)
    //-------------------------------------------------------------------------------
    {
      T * res = new T[R];
      unsigned int r = rank + width + 1;
      for(int i=0;i<R;i++)
	{
	  res[i] = this->data[r];
	  r = r + width + 1;
	}
      return res;
    }
    //-------------------------------------------------------------------------------
    T * allLeftDown(unsigned int rank,unsigned int width)
    //-------------------------------------------------------------------------------
    {
      T * res = new T[R];
      unsigned int r = rank + width -1 ;
      for(int i=0;i<R;i++)
	{
	  res[i] = this->data[r];
	  r = r + width -1 ;
	}
      return res;
    }
    //-------------------------------------------------------------------------------
    T * allRightUp(unsigned int rank,unsigned int width)
    //-------------------------------------------------------------------------------
    {
      T * res = new T[R];
      unsigned int r = rank - width + 1;
      for(int i=0;i<R;i++)
	{
	  res[i] = this->data[r];
	  r = r - width + 1;
	}
      return res;
    }
    //-------------------------------------------------------------------------------
    T * allLeftUp(unsigned int rank,unsigned int width)
    //-------------------------------------------------------------------------------
    {
      T * res = new T[R];
      unsigned int r = rank - width - 1;
      for(int i=0;i<R;i++)
	{
	  res[i] = this->data[r];
	  r = r - width - 1;
	}
      return res;
    }
    //-------------------------------------------------------------------------------
    T right(unsigned int rank)
    //-------------------------------------------------------------------------------
    {
      unsigned int r = rank + 1;
      return this->data[r];
    }
    //-------------------------------------------------------------------------------
    T left(unsigned int rank)
    //-------------------------------------------------------------------------------
    {
      unsigned int r = rank - 1;
      return this->data[r];
    }
    //-------------------------------------------------------------------------------
    T up(unsigned int rank,unsigned int width)
    //-------------------------------------------------------------------------------
    {
      unsigned int r = rank - width;
      return this->data[r];
    }
    //-------------------------------------------------------------------------------
    T down(unsigned int rank,unsigned int width)
    //-------------------------------------------------------------------------------
    {
      unsigned int r = rank + width;
      return this->data[r];
    }
    //-------------------------------------------------------------------------------
    T rightDown(unsigned int rank,unsigned int width)
    //-------------------------------------------------------------------------------
    {
      unsigned int r = rank + width +1;
      return this->data[r];
    }
    //-------------------------------------------------------------------------------
    T leftDown(unsigned int rank,unsigned int width)
    //-------------------------------------------------------------------------------
    {
      unsigned int r = rank + width -1;
      return this->data[r];
    }
    //-------------------------------------------------------------------------------
    T rightUp(unsigned int rank,unsigned int width)
    //-------------------------------------------------------------------------------
    {
      unsigned int r = rank - width +1;
      return this->data[r];
    }
    //-------------------------------------------------------------------------------
    T leftUp(unsigned int rank,unsigned int width)
    //-------------------------------------------------------------------------------
    {
      unsigned int r = rank - width -1;
      return this->data[r];
    }
    //-------------------------------------------------------------------------------
    T * allX(unsigned int rank)
    //-------------------------------------------------------------------------------
    {
      T * res = new T[R*2];
      unsigned int b = rank - R;
      for(int i=0;i<R;i++)
	{
	  res[i] = this->data[b];
	  b++;
	}
      b++;
      for(int i=R;i<2*R;i++)
	{
	  res[i] = this->data[b];
	  b++;
	}
      return res;
    }
    //-------------------------------------------------------------------------------
    void allX(unsigned int rank, T * t)
    //-------------------------------------------------------------------------------
    {
      unsigned int b = rank - R;
      for(int i=0;i<R;i++)
        {
          t[i] = this->data[b];
          b++;
        }
      b++;
      for(int i=R;i<2*R;i++)
        {
          t[i] = this->data[b];
          b++;
        }
    }
    //-------------------------------------------------------------------------------
    T * allY(unsigned int rank,unsigned int width)
    //-------------------------------------------------------------------------------
    {
      T * res = new T[R*2];
      unsigned int b = rank - R*width;

      for(int i=0;i<R;i++)
	{
	  res[i] = this->data[b];
	  b = b + width;
	}
      b = b + width;
      for(int i=R;i<2*R;i++)
	{
	  res[i] = this->data[b];
	  b = b + width;
	}
      return res;
    }
    //-------------------------------------------------------------------------------
    void allY(unsigned int rank,unsigned int width, T * t)
    //-------------------------------------------------------------------------------
    {
      unsigned int b = rank - R*width;

      for(int i=0;i<R;i++)
        {
          t[i] = this->data[b];
          b = b + width;
        }
      b = b + width;
      for(int i=R;i<2*R;i++)
        {
          t[i] = this->data[b];
          b = b + width;
        }
    }
    //-------------------------------------------------------------------------------
    T * eight(unsigned int rank,unsigned int width)
    //-------------------------------------------------------------------------------
    {
      T * res = new T[8];
      res[0] = leftUp(rank,width);
      res[1] = up(rank,width);
      res[2] = rightUp(rank,width);
      res[3] = right(rank);
      res[4] = rightDown(rank,width);
      res[5] = down(rank,width);
      res[6] = leftDown(rank,width);
      res[7] = left(rank);
      return res;
    }
    //-------------------------------------------------------------------------------
    T * four(unsigned int rank,unsigned int width)
    //-------------------------------------------------------------------------------
    {
      T * res = new T[8];
      res[0] = up(rank,width);
      res[1] = right(rank);
      res[2] = down(rank,width);
      res[3] = left(rank);
      return res;
    }
    //-------------------------------------------------------------------------------
    T inright(unsigned int rank)
    //-------------------------------------------------------------------------------
    {
      unsigned int re = rank%border_head.width;
      unsigned int r = rank + (R-re);
      return this->data[r];
    }
    //-------------------------------------------------------------------------------
    T inleft(unsigned int rank)
    //-------------------------------------------------------------------------------
    {
      unsigned int re = border_head.width - rank%border_head.width -1 ;
      unsigned int r = rank - (R-re);
      return this->data[r];
    }
    //-------------------------------------------------------------------------------
    T inup(unsigned int rank,unsigned int width)
    //-------------------------------------------------------------------------------
    {
      unsigned int re = border_head.height-floor(rank/border_head.width)-1;
      unsigned int r = rank - (R-re)*width;
      return this->data[r];
    }
    //-------------------------------------------------------------------------------
    T indown(unsigned int rank,unsigned int width)
    //-------------------------------------------------------------------------------
    {
      unsigned int re = floor(rank/border_head.width);
      unsigned int r = rank + (R-re)*width;
      return this->data[r];
    }
    //-------------------------------------------------------------------------------
    void indexesIt(unsigned int rank,int &col,int &li)
    //-------------------------------------------------------------------------------
    {
      unsigned int relative_rank = rank - border*border_head.width - (floor(rank/border_head.width)-border)*border*2 -border;
      li = floor(relative_rank/this->loc_head.width);
      col = relative_rank%this->loc_head.width;
    }
    //-------------------------------------------------------------------------------

  public:
    //! constructor of the distributed matrix
    /*!
      This constructor will simply allocate the good space.
      \param h is the header to use to build your matrix
      \param value is the default value to put in the matrix
    */
    //-------------------------------------------------------------------------------
    DMatrix_impl(HEADER h,const T value) : DMatrix_base<T>()
    //-------------------------------------------------------------------------------
    {
      border = R;
      this->head=h;
      create();
      initValues(value);
      getNeighborRanks();
      phBLimits();
      //print();
    }
  
    //! constructor of the distributed matrix
    /*!
      The constructor evaluate how to divide the matrix in submatrice and do the work.
      \param binFile is the path of the binary file you want to work on
    */
    //-------------------------------------------------------------------------------
    DMatrix_impl(const char * binFile) : DMatrix_base<T>()
    //-------------------------------------------------------------------------------
    {
      border = R;
      this->head = Header::read(binFile);
      create();
      initValues(0);
      read(binFile);
      getNeighborRanks();
      phBLimits();
      //print();
    }
    //-------------------------------------------------------------------------------
  
    //! destructor of the distributed matrix
    /*!  */
    //-------------------------------------------------------------------------------
    ~DMatrix_impl()
    //-------------------------------------------------------------------------------
    {
      if(NULL!=this->data)
	delete [] this->data;
    }
    //-------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------
    inline T getValue(iterator<T,R> it){return this->data[it._rank];}
    inline T getValue(iterator_cont<T,R> it){return this->data[it._rank];}
    inline T getValue(iterator_step<T,R> it){return this->data[it._rank];}
    inline T getValue(iterator_rev<T,R> it){return this->data[it._rank];}
    inline T getValue(iterator_phb_left<T,R> it){return this->data[it._rank];}
    inline T getValue(iterator_phb_right<T,R> it){return this->data[it._rank];}
    inline T getValue(iterator_phb_up<T,R> it){return this->data[it._rank];}
    inline T getValue(iterator_phb_down<T,R> it){return this->data[it._rank];}
    //-------------------------------------------------------------------------------
    inline T& getValueRef(iterator<T,R> it){return this->data[it._rank];}
    inline T& getValueRef(iterator_cont<T,R> it){return this->data[it._rank];}
    inline T& getValueRef(iterator_step<T,R> it){return this->data[it._rank];}
    inline T& getValueRef(iterator_rev<T,R> it){return this->data[it._rank];}
    inline T& getValueRef(iterator_phb_left<T,R> it){return this->data[it._rank];}
    inline T& getValueRef(iterator_phb_right<T,R> it){return this->data[it._rank];}
    inline T& getValueRef(iterator_phb_up<T,R> it){return this->data[it._rank];}
    inline T& getValueRef(iterator_phb_down<T,R> it){return this->data[it._rank];}
    //-------------------------------------------------------------------------------
    inline void setValue(iterator<T,R> it,T value){this->data[it._rank]=value;}
    inline void setValue(iterator_cont<T,R> it,T value){this->data[it._rank]=value;}
    inline void setValue(iterator_step<T,R> it,T value){this->data[it._rank]=value;}
    inline void setValue(iterator_rev<T,R> it,T value){this->data[it._rank]=value;}
    inline void setValue(iterator_phb_left<T,R> it,T value){this->data[it._rank]=value;}
    inline void setValue(iterator_phb_right<T,R> it,T value){this->data[it._rank]=value;}
    inline void setValue(iterator_phb_up<T,R> it,T value){this->data[it._rank]=value;}
    inline void setValue(iterator_phb_down<T,R> it,T value){this->data[it._rank]=value;}
    //-------------------------------------------------------------------------------
    //! Set value in the global middle
    /*!
      \param val is the value to affect
    */
    //-------------------------------------------------------------------------------
    void setGlobalMiddleValue(T val)
    //-------------------------------------------------------------------------------
    {
      if((this->col!=this->cols-1 && this->row!=this->rows-1) ||
	 (this->cols==1 && this->row!=this->rows-1) ||
	 (this->col!=this->cols-1 && this->rows==1))
	{
	  unsigned int i = floor(this->head.height/2)-1;
	  unsigned int j = floor(this->head.width/2)-1;
	  if(this->col*this->loc_head.width<=j && 
	     (this->col+1)*this->loc_head.width>j && 
	     this->row*this->loc_head.height<=i && 
	     (this->row+1)*this->loc_head.height>i)
	    {
	      unsigned int j_loc = j-this->col*this->loc_head.width;
	      unsigned int i_loc = i-this->row*this->loc_head.height;
	      unsigned int rank = R*border_head.width +  i_loc*border_head.width + j_loc + R;
	      this->data[rank] = val;
	    }
	}
    }
    //-------------------------------------------------------------------------------
    //! to set all the values of the physical border of the matrix
    /*!
      \param val is the value to set
    */
    //-------------------------------------------------------------------------------
    inline void setPhysicalBorder(T val)
    {
      iterator_phb_left<T,R> left =begin_phb_left();
      iterator_phb_left<T,R> leftEnd = end_phb_left();
      for(left;left<leftEnd;++left)
	this->data[left._rank] = val;
      iterator_phb_right<T,R> right =begin_phb_right();
      iterator_phb_right<T,R> rightEnd = end_phb_right();
      for(right;right<rightEnd;++right)
	this->data[right._rank] = val;
      iterator_phb_up<T,R> up =begin_phb_up();
      iterator_phb_up<T,R> upEnd = end_phb_up();
      for(up;up<upEnd;++up)
	this->data[up._rank] = val;
      iterator_phb_down<T,R> down =begin_phb_down();
      iterator_phb_down<T,R> downEnd = end_phb_down();
      for(down;down<downEnd;++down)
	this->data[down._rank] = val;
    }
    //-------------------------------------------------------------------------------
    //! to set all the values on the right of physical border of the matrix
    /*!
      \param val is the value to set
    */
    //-------------------------------------------------------------------------------
    inline void setRightPhysicalBorder(T val)
    {
      iterator_phb_right<T,R> it =begin_phb_right();
      iterator_phb_right<T,R> itEnd = end_phb_right();
      for(it;it<itEnd;++it)
	this->data[it._rank] = val;
    }
    //-------------------------------------------------------------------------------
    //! to set all the values on the left of physical border of the matrix
    /*!
      \param val is the value to set
    */
    //-------------------------------------------------------------------------------
    inline void setLeftPhysicalBorder(T val)
    {
      iterator_phb_left<T,R> it =begin_phb_left();
      iterator_phb_left<T,R> itEnd = end_phb_left();
      for(it;it<itEnd;++it)
	this->data[it._rank] = val;
    }
    //-------------------------------------------------------------------------------
    //! to set all the values on the up of physical border of the matrix
    /*!
      \param val is the value to set
    */
    //-------------------------------------------------------------------------------
    inline void setUpPhysicalBorder(T val)
    {
      iterator_phb_up<T,R> it =begin_phb_up();
      iterator_phb_up<T,R> itEnd = end_phb_up();
      for(it;it<itEnd;++it)
	this->data[it._rank] = val;
    }
    //-------------------------------------------------------------------------------
    //! to set all the values on the down of physical border of the matrix
    /*!
      \param val is the value to set
    */
    //-------------------------------------------------------------------------------
    void setDownPhysicalBorder(T val)
    {
      iterator_phb_down<T,R> it =begin_phb_down();
      iterator_phb_down<T,R> itEnd = end_phb_down();
      for(it;it<itEnd;++it)
	this->data[it._rank] = val;
    }
    //-------------------------------------------------------------------------------
    inline iterator<T,R> begin(){return iterator<T,R>(border*this->border_head.width+border,this->border_head.width);}
    inline iterator_cont<T,R> begin_cont(){return iterator_cont<T,R>(border*this->border_head.width+border,this->border_head.width);}
    inline iterator_rev<T,R> begin_rev(){return iterator_rev<T,R>((this->border_head.width*this->border_head.height)-1-border*this->border_head.width-border,this->border_head.width);}
    inline iterator_step<T,R> begin_step(int step,int nb){return iterator_step<T,R>(border*this->border_head.width+border,this->border_head.width,step,nb);}
    inline iterator_phb_left<T,R> begin_phb_left(){return iterator_phb_left<T,R>(beginPhBLeft,this->border_head.width);}
    inline iterator_phb_right<T,R> begin_phb_right(){return iterator_phb_right<T,R>(beginPhBRight,this->border_head.width);}
    inline iterator_phb_up<T,R> begin_phb_up(){return iterator_phb_up<T,R>(beginPhBUp,this->border_head.width);}
    inline iterator_phb_down<T,R> begin_phb_down(){return iterator_phb_down<T,R>(beginPhBDown,this->border_head.width);}
    inline iterator_line<T,R> begin_line(){return iterator_line<T,R>(border*this->border_head.width+border,this->border_head.width);}
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    inline iterator<T,R> end(){return iterator<T,R>((this->border_head.width*this->border_head.height)-1-border*this->border_head.width-border,this->border_head.width);}
    inline iterator_cont<T,R> end_cont(){return iterator_cont<T,R>((this->border_head.width*this->border_head.height)-1-border*this->border_head.width-border,this->border_head.width);}
    inline iterator_rev<T,R> end_rev(){return iterator_rev<T,R>(border*this->border_head.width+border-1,this->border_head.width);}
    inline iterator_step<T,R> end_step(){return iterator_step<T,R>((this->border_head.width*this->border_head.height)-1-border*this->border_head.width-border,this->border_head.width);}
    inline iterator_phb_left<T,R> end_phb_left(){return iterator_phb_left<T,R>(endPhBLeft,this->border_head.width);}
    inline iterator_phb_right<T,R> end_phb_right(){return iterator_phb_right<T,R>(endPhBRight,this->border_head.width);}
    inline iterator_phb_up<T,R> end_phb_up(){return iterator_phb_up<T,R>(endPhBUp,this->border_head.width);}
    inline iterator_phb_down<T,R> end_phb_down(){return iterator_phb_down<T,R>(endPhBDown,this->border_head.width);}
    inline iterator_line<T,R> end_line(){return iterator_line<T,R>((this->border_head.width*this->border_head.height)-(border+1)*this->border_head.width+border,this->border_head.width);}
    //-------------------------------------------------------------------------------
    //! get the iterator on the matrix at position (x,y)
    /*!
      \param x is row index
      \pram y is column index
      \return returns the iterator at position (x,y)
    */
    iterator<T,R> getIterator(int col,int li)
    //-------------------------------------------------------------------------------
    {
      if(NULL!=this->data)
	{
	  unsigned int rank = border*border_head.width + border + li*border_head.width+col;
	  return iterator<T,R>(rank,this->border_head.width);
	}
    }
    //-------------------------------------------------------------------------------
    //! get the iterator contiguous on the matrix at position (x,y)
    /*!
      \param x is row index
      \pram y is column index
      \return returns the iterator at position (x,y)
    */
    iterator_cont<T,R> getIterator_cont(int col,int li)
    //-------------------------------------------------------------------------------
    {
      if(NULL!=this->data)
	{
	  unsigned int rank = border*border_head.width + border + li*border_head.width+col;
	  return iterator_cont<T,R>(rank,this->border_head.width);
	}
    }
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    //! get the iterator reverse on the matrix at position (x,y)
    /*!
      \param x is row index
      \pram y is column index
      \return returns the iterator at position (x,y)
    */
    iterator_rev<T,R> getIterator_rev(int col,int li)
    //-------------------------------------------------------------------------------
    {
      if(NULL!=this->data)
	{
	  unsigned int rank = border*border_head.width + border + li*border_head.width+col;
	  return iterator_rev<T,R>(rank,this->border_head.width);
	}
    }
    //-------------------------------------------------------------------------------
    //! get the indexes of the iterator on the matrix
    /*!
      \param it is the iterator from which the indexes will be taken
      \param x is the iterator position x returned
      \param y is the iterator position y returned
    */
    void getIndexes(iterator<T,R> it,int &col,int &li){indexesIt(it._rank,col,li);}
    void getIndexes(iterator_cont<T,R> it,int &col,int &li){indexesIt(it._rank,col,li);}
    void getIndexes(iterator_rev<T,R> it,int &col,int &li){indexesIt(it._rank,col,li);}
    //-------------------------------------------------------------------------------
    inline T& operator[](iterator<T,R> it) { return this->data[it._rank]; }
    inline T& operator[](iterator_cont<T,R> it) { return this->data[it._rank]; }
    inline T& operator[](iterator_rev<T,R> it) { return this->data[it._rank]; }
    inline T& operator[](iterator_step<T,R> it) { return this->data[it._rank]; }
    inline T& operator[](iterator_phb_left<T,R> it) { return this->data[it._rank]; }
    inline T& operator[](iterator_phb_right<T,R> it) { return this->data[it._rank]; }
    inline T& operator[](iterator_phb_up<T,R> it) { return this->data[it._rank]; }
    inline T& operator[](iterator_phb_down<T,R> it) { return this->data[it._rank]; }
    //-------------------------------------------------------------------------------

    //! Print the matrix by bloc
    /*!
     */
    //-------------------------------------------------------------------------------
    void print()
    //-------------------------------------------------------------------------------
    {
      std::stringstream st;
      for(unsigned int i=0;i<this->loc_head.height;i++)
	{
	  for(unsigned int j=0;j<this->loc_head.width;j++)
	    {
	      st<<this->data[border*this->border_head.width + border + i*this->border_head.width+j]<<" ";
	    }
	  st<<"\n";
	}
      Mpi_::print(st.str());
    }
    //-------------------------------------------------------------------------------

    //! Print all the matrix by bloc
    /*!
     */
    //-------------------------------------------------------------------------------
    void printAll()
    //-------------------------------------------------------------------------------
    {
      std::stringstream st;
      for(unsigned int i=0;i<this->border_head.height;i++)
        {
          for(unsigned int j=0;j<this->border_head.width;j++)
            {
              st<<this->data[i*this->border_head.width+j]<<" ";
            }
          st<<"\n";
        }
      Mpi_::print(st.str());
    }
    //-------------------------------------------------------------------------------

    //! get borders
    /*!
      Get the borders from other processors
      MPI exchanges
    */
    //-------------------------------------------------------------------------------
    void getBorders()
    //-------------------------------------------------------------------------------
    {
      if(this->row%2==0)
      {
        if(this->row!=0)
      	{
          //recv up

      	  T * toSend = getUpBorderToSend();
      	  T * toGet = new T[this->loc_head.width*border];
      	  Communications<T>::Exchanges(toSend,toGet,this->loc_head.width*border,mpi_ranks[1]);
      	  setUpBorder(toGet);
      	  delete [] toSend;
      	  delete [] toGet;
      	}
        if(this->row!=(this->rows-1))
      	{          
          //rcv down

      	  T * toSend = getDownBorderToSend();
      	  T * toGet = new T[this->loc_head.width*border];
      	  Communications<T>::Exchanges(toSend,toGet,this->loc_head.width*border,mpi_ranks[5]);
      	  setDownBorder(toGet);
      	  delete [] toSend;
      	  delete [] toGet;
      	}
      }
      else if(this->row%2==1)
      {
        if(this->row!=(this->rows-1))
        {          
          //rcv down

          T * toSend = getDownBorderToSend();
          T * toGet = new T[this->loc_head.width*border];
          Communications<T>::Exchanges(toSend,toGet,this->loc_head.width*border,mpi_ranks[5]);
          setDownBorder(toGet);
          delete [] toSend;
          delete [] toGet;
        }
        if(this->row!=0)
        {
          //recv up

          T * toSend = getUpBorderToSend();
          T * toGet = new T[this->loc_head.width*border];
          Communications<T>::Exchanges(toSend,toGet,this->loc_head.width*border,mpi_ranks[1]);
          setUpBorder(toGet);
          delete [] toSend;
          delete [] toGet;
        }
      }
      if(this->col%2==0)
      {
        if(this->col!=(this->cols-1))
      	{
          //rcv right

      	  T * toSend = getRightBorderToSend();
      	  T * toGet = new T[this->loc_head.height*border];
      	  Communications<T>::Exchanges(toSend,toGet,this->loc_head.height*border,mpi_ranks[3]);
      	  setRightBorder(toGet);
      	  delete [] toSend;
      	  delete [] toGet;
      	}
        if(this->col!=0)
      	{
          //rcv left

      	  T * toSend = getLeftBorderToSend();
      	  T * toGet = new T[this->loc_head.height*border];
      	  Communications<T>::Exchanges(toSend,toGet,this->loc_head.height*border,mpi_ranks[7]);
      	  setLeftBorder(toGet);
      	  delete [] toSend;
      	  delete [] toGet;
      	}
      }
      else if(this->col%2==1)
      {
        if(this->col!=0)
        {
          //rcv left

          T * toSend = getLeftBorderToSend();
          T * toGet = new T[this->loc_head.height*border];
          Communications<T>::Exchanges(toSend,toGet,this->loc_head.height*border,mpi_ranks[7]);
          setLeftBorder(toGet);
          delete [] toSend;
          delete [] toGet;
        }
        if(this->col!=(this->cols-1))
        {
          //rcv right

          T * toSend = getRightBorderToSend();
          T * toGet = new T[this->loc_head.height*border];
          Communications<T>::Exchanges(toSend,toGet,this->loc_head.height*border,mpi_ranks[3]);
          setRightBorder(toGet);
          delete [] toSend;
          delete [] toGet;
        }
      }
    }
    //-------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------
    inline T * getAllRight(iterator<T,R> it){return allRight(it._rank);}
    inline T * getAllRight(iterator_cont<T,R> it){return allRight(it._rank);}
    inline T * getAllRight(iterator_step<T,R> it){return allRight(it._rank);}
    inline T * getAllRight(iterator_rev<T,R> it){return allRight(it._rank);}
    inline T * getAllRight(iterator_phb_left<T,R> it){return allRight(it._rank);}
    //-------------------------------------------------------------------------------
    inline T * getAllLeft(iterator<T,R> it){return allLeft(it._rank);}
    inline T * getAllLeft(iterator_cont<T,R> it){return allLeft(it._rank);}
    inline T * getAllLeft(iterator_step<T,R> it){return allLeft(it._rank);}
    inline T * getAllLeft(iterator_rev<T,R> it){return allLeft(it._rank);}
    inline T * getAllLeft(iterator_phb_right<T,R> it){return allLeft(it._rank);}
    //-------------------------------------------------------------------------------
    inline T * getAllUp(iterator<T,R> it){return allUp(it._rank,it._width);}
    inline T * getAllUp(iterator_cont<T,R> it){return allUp(it._rank,it._width);}
    inline T * getAllUp(iterator_step<T,R> it){return allUp(it._rank,it._width);}
    inline T * getAllUp(iterator_rev<T,R> it){return allUp(it._rank,it._width);}
    inline T * getAllUp(iterator_phb_down<T,R> it){return allUp(it._rank,it._width);}
    //-------------------------------------------------------------------------------
    inline T * getAllDown(iterator<T,R> it){return allDown(it._rank,it._width);}
    inline T * getAllDown(iterator_cont<T,R> it){return allDown(it._rank,it._width);}
    inline T * getAllDown(iterator_step<T,R> it){return allDown(it._rank,it._width);}
    inline T * getAllDown(iterator_rev<T,R> it){return allDown(it._rank,it._width);}
    inline T * getAllDown(iterator_phb_up<T,R> it){return allDown(it._rank,it._width);}
    //-------------------------------------------------------------------------------
    inline T * getAllRightDown(iterator<T,R> it){return allRightDown(it._rank,it._width);}
    inline T * getAllRightDown(iterator_cont<T,R> it){return allRightDown(it._rank,it._width);}
    inline T * getAllRightDown(iterator_step<T,R> it){return allRightDown(it._rank,it._width);}
    inline T * getAllRightDown(iterator_rev<T,R> it){return allRightDown(it._rank,it._width);}
    //-------------------------------------------------------------------------------
    inline T * getAllLeftDown(iterator<T,R> it){return allLeftDown(it._rank,it._width);}
    inline T * getAllLeftDown(iterator_cont<T,R> it){return allLeftDown(it._rank,it._width);}
    inline T * getAllLeftDown(iterator_step<T,R> it){return allLeftDown(it._rank,it._width);}
    inline T * getAllLeftDown(iterator_rev<T,R> it){return allLeftDown(it._rank,it._width);}
    //-------------------------------------------------------------------------------
    inline T * getAllRightUp(iterator<T,R> it){return allRightUp(it._rank,it._width);}
    inline T * getAllRightUp(iterator_cont<T,R> it){return allRightUp(it._rank,it._width);}
    inline T * getAllRightUp(iterator_step<T,R> it){return allRightUp(it._rank,it._width);}
    inline T * getAllRightUp(iterator_rev<T,R> it){return allRightUp(it._rank,it._width);}
    //-------------------------------------------------------------------------------
    inline T * getAllLeftUp(iterator<T,R> it){return allLeftUp(it._rank,it._width);}
    inline T * getAllLeftUp(iterator_cont<T,R> it){return allLeftUp(it._rank,it._width);}
    inline T * getAllLeftUp(iterator_step<T,R> it){return allLeftUp(it._rank,it._width);}
    inline T * getAllLeftUp(iterator_rev<T,R> it){return allLeftUp(it._rank,it._width);}
    //-------------------------------------------------------------------------------
    inline T getRight(iterator<T,R> it){return right(it._rank);}
    inline T getRight(iterator_cont<T,R> it){return right(it._rank);}
    inline T getRight(iterator_step<T,R> it){return right(it._rank);}
    inline T getRight(iterator_rev<T,R> it){return right(it._rank);}
    inline T getRight(iterator_phb_left<T,R> it){return right(it._rank);}
    //-------------------------------------------------------------------------------
    inline T getLeft(iterator<T,R> it){return left(it._rank);}
    inline T getLeft(iterator_cont<T,R> it){return left(it._rank);}
    inline T getLeft(iterator_step<T,R> it){return left(it._rank);}
    inline T getLeft(iterator_rev<T,R> it){return left(it._rank);}
    inline T getLeft(iterator_phb_right<T,R> it){return left(it._rank);}
    //-------------------------------------------------------------------------------
    inline T getUp(iterator<T,R> it){return up(it._rank,it._width);}
    inline T getUp(iterator_cont<T,R> it){return up(it._rank,it._width);}
    inline T getUp(iterator_step<T,R> it){return up(it._rank,it._width);}
    inline T getUp(iterator_rev<T,R> it){return up(it._rank,it._width);}
    inline T getUp(iterator_phb_down<T,R> it){return up(it._rank,it._width);}
    //-------------------------------------------------------------------------------
    inline T getDown(iterator<T,R> it){return down(it._rank,it._width);}
    inline T getDown(iterator_cont<T,R> it){return down(it._rank,it._width);}
    inline T getDown(iterator_step<T,R> it){return down(it._rank,it._width);}
    inline T getDown(iterator_rev<T,R> it){return down(it._rank,it._width);}
    inline T getDown(iterator_phb_up<T,R> it){return down(it._rank,it._width);}
    //-------------------------------------------------------------------------------
    inline T getRightDown(iterator<T,R> it){return rightDown(it._rank,it._width);}
    inline T getRightDown(iterator_cont<T,R> it){return rightDown(it._rank,it._width);}
    inline T getRightDown(iterator_step<T,R> it){return rightDown(it._rank,it._width);}
    inline T getRightDown(iterator_rev<T,R> it){return rightDown(it._rank,it._width);}
    //-------------------------------------------------------------------------------
    inline T getLeftDown(iterator<T,R> it){return leftDown(it._rank,it._width);}
    inline T getLeftDown(iterator_cont<T,R> it){return leftDown(it._rank,it._width);}
    inline T getLeftDown(iterator_step<T,R> it){return leftDown(it._rank,it._width);}
    inline T getLeftDown(iterator_rev<T,R> it){return leftDown(it._rank,it._width);}
    //-------------------------------------------------------------------------------
    inline T getRightUp(iterator<T,R> it){return rightUp(it._rank,it._width);}
    inline T getRightUp(iterator_cont<T,R> it){return rightUp(it._rank,it._width);}
    inline T getRightUp(iterator_step<T,R> it){return rightUp(it._rank,it._width);}
    inline T getRightUp(iterator_rev<T,R> it){return rightUp(it._rank,it._width);}
    //-------------------------------------------------------------------------------
    inline T getLeftUp(iterator<T,R> it){return leftUp(it._rank,it._width);}
    inline T getLeftUp(iterator_cont<T,R> it){return leftUp(it._rank,it._width);}
    inline T getLeftUp(iterator_step<T,R> it){return leftUp(it._rank,it._width);}
    inline T getLeftUp(iterator_rev<T,R> it){return leftUp(it._rank,it._width);}
    //-------------------------------------------------------------------------------
    inline T * getAllX(iterator<T,R> it){return allX(it._rank);}
    inline T * getAllX(iterator_cont<T,R> it){return allX(it._rank);}
    inline T * getAllX(iterator_step<T,R> it){return allX(it._rank);}
    inline T * getAllX(iterator_rev<T,R> it){return allX(it._rank);}
    //-------------------------------------------------------------------------------
    inline void getAllX(iterator<T,R> it, T * t){return allX(it._rank,t);}
    inline void getAllX(iterator_cont<T,R> it, T * t){return allX(it._rank,t);}
    inline void getAllX(iterator_step<T,R> it, T * t){return allX(it._rank,t);}
    inline void getAllX(iterator_rev<T,R> it, T * t){return allX(it._rank,t);}
    //-------------------------------------------------------------------------------
    inline void getAllY(iterator<T,R> it, T * t){return allY(it._rank,it._width,t);}
    inline void getAllY(iterator_cont<T,R> it, T * t){return allY(it._rank,it._width,t);}
    inline void getAllY(iterator_step<T,R> it, T * t){return allY(it._rank,it._width,t);}
    inline void getAllY(iterator_rev<T,R> it, T * t){return allY(it._rank,it._width,t);}
    //-------------------------------------------------------------------------------
    inline T * getAllY(iterator<T,R> it){return allY(it._rank,it._width);}
    inline T * getAllY(iterator_cont<T,R> it){return allY(it._rank,it._width);}
    inline T * getAllY(iterator_step<T,R> it){return allY(it._rank,it._width);}
    inline T * getAllY(iterator_rev<T,R> it){return allY(it._rank,it._width);}
    //-------------------------------------------------------------------------------
    inline T * get8(iterator<T,R> it){return eight(it._rank,it._width);}
    inline T * get8(iterator_cont<T,R> it){return eight(it._rank,it._width);}
    inline T * get8(iterator_step<T,R> it){return eight(it._rank,it._width);}
    inline T * get8(iterator_rev<T,R> it){return eight(it._rank,it._width);}
    //-------------------------------------------------------------------------------
    inline T * get4(iterator<T,R> it){return four(it._rank,it._width);}
    inline T * get4(iterator_cont<T,R> it){return four(it._rank,it._width);}
     inline T * get4(iterator_step<T,R> it){return four(it._rank,it._width);}
    inline T * get4(iterator_rev<T,R> it){return four(it._rank,it._width);}
    //-------------------------------------------------------------------------------
    inline T getInRight(iterator_phb_left<T,R> it){return inright(it._rank);}
    inline T getInLeft(iterator_phb_right<T,R> it){return inleft(it._rank);}
    inline T getInUp(iterator_phb_down<T,R> it){return inup(it._rank,it._width);}
    inline T getInDown(iterator_phb_up<T,R> it){return indown(it._rank,it._width);}
    //-------------------------------------------------------------------------------
    inline HEADER getGlobalHeader(){return this->head;}
    //-------------------------------------------------------------------------------
    inline HEADER getHeader(){return this->loc_head;}
    //-------------------------------------------------------------------------------
    inline HEADER getBorderHeader(){return this->border_head;}
    //-------------------------------------------------------------------------------
    inline int getCols(){return this->cols;}
    //-------------------------------------------------------------------------------
    inline int getRows(){return this->rows;}
    //-------------------------------------------------------------------------------
    inline int getCol(){return this->col;}
    //-------------------------------------------------------------------------------
    inline int getRow(){return this->row;}
    //-------------------------------------------------------------------------------
    inline T * getData(){return this->data;}
    inline unsigned int getWidth(){return this->head.width;}
    inline unsigned int getWidthLoc(){return this->loc_head.width;}
    inline unsigned int getHeightLoc(){return this->loc_head.height;}
    //-------------------------------------------------------------------------------
    inline int getBorder(){return this->border;}
    //-------------------------------------------------------------------------------
    inline unsigned int getStartWPHBL(){return swphbL;}
    inline unsigned int getWidthPHBL(){return wphbL;}
    inline unsigned int getStartHPHBL(){return shphbL;}
    inline unsigned int getHeightPHBL(){return hphbL;}

    inline unsigned int getStartWPHBR(){return swphbR;}
    inline unsigned int getWidthPHBR(){return wphbR;}
    inline unsigned int getStartHPHBR(){return shphbR;}
    inline unsigned int getHeightPHBR(){return hphbR;}

    inline unsigned int getStartWPHBU(){return swphbU;}
    inline unsigned int getWidthPHBU(){return wphbU;}
    inline unsigned int getStartHPHBU(){return shphbU;}
    inline unsigned int getHeightPHBU(){return hphbU;}

    inline unsigned int getStartWPHBD(){return swphbD;}
    inline unsigned int getWidthPHBD(){return wphbD;}
    inline unsigned int getStartHPHBD(){return shphbD;}
    inline unsigned int getHeightPHBD(){return hphbD;}

  };
  //-------------------------------------------------------------------------------
  //================================================================================
  //================================================================================
  //!  specialization of base template Distributed Matrix Class without recovery
  /*!
    This template class defines the distributed matrix without recovery
    T = type of data in the matrix
  */
  //-------------------------------------------------------------------------------
  template<class T> struct DMatrix_impl<T,0,false> : DMatrix_base<T>
  //-------------------------------------------------------------------------------
  {
  protected:

    //physical border
    unsigned int swphbL,swphbR,swphbU,swphbD;
    unsigned int wphbL,wphbR,wphbU,wphbD;
    unsigned int shphbL,shphbR,shphbU,shphbD;
    unsigned int hphbL,hphbR,hphbU,hphbD;

    static const int border = 0;

    //-------------------------------------------------------------------------------
    void create()
    //-------------------------------------------------------------------------------
    {
      //division of the matrix in cols and rows
      this->cols = floor(sqrt(Mpi_::mpi_nb));
      int nb = 1;
      while(Mpi_::mpi_nb%this->cols!=0)
	{
	  this->cols = floor(sqrt(Mpi_::mpi_nb))-nb;
	  nb++;
	}
      this->rows = Mpi_::mpi_nb/this->cols;
	
#ifdef DEBUG
      std::stringstream st;
      st<<"rows and cols :"<<this->rows<<","<<this->cols<<"\n";
      Mpi_::print(st.str());
#endif

      this->row=floor(Mpi_::mpi_rank/this->cols);
      this->col=Mpi_::mpi_rank%this->cols;
	
#ifdef DEBUG
      std::stringstream st3;
      st3<<"MPI row and col :"<<this->row<<","<<this->col<<"\n"<<"\n";
      Mpi_::print(st3.str());
#endif	

      this-> remainderh = 0;
      //if the height of the matrix can be divided by the number of rows
      if(this->head.height%this->rows==0)
	this->loc_head.height=floor(this->head.height/this->rows);
      //else we add one to the height of the submatrix and the last row will have less elements
      else
	{
	  this->remainderh = this->head.height - floor(this->head.height/this->rows)*this->rows;

	  if(this->row<this->remainderh)
	    this->loc_head.height=floor(this->head.height/this->rows)+1;
	  else
	    this->loc_head.height=floor(this->head.height/this->rows);
	}

      this-> remainderw = 0;
      //if the height of the matrix can be divided by the number of rows
      if(this->head.width%this->cols==0)
	this->loc_head.width=floor(this->head.width/this->cols);
      //else we add one to the height of the submatrix and the last row will have less elements
      else
	{
	  this->remainderw = this->head.width - floor(this->head.width/this->cols)*this->cols;

	  if(this->col<this->remainderw)
	    this->loc_head.width=floor(this->head.width/this->cols)+1;
	  else
	    this->loc_head.width=floor(this->head.width/this->cols);
	}

      /*//if the width of the matrix can be divided by the number of cols
      if(this->head.width%this->cols==0)
	this->loc_head.width=floor(this->head.width/this->cols);
      //else we add one to the width of the submatrix and the last column will have less elements
      else
	{
	  this->normal_width = floor(this->head.width/this->cols)+1;
	  this->last_width = this->head.width - (this->cols-1)*(floor(this->head.width/this->cols)+1);
	  if(this->col!=this->cols-1)
	    this->loc_head.width=this->normal_width;
	  else
	    this->loc_head.width=this->last_width;
	}
      //if the height of the matrix can be divided by the number of rows
      if(this->head.height%this->rows==0)
	this->loc_head.height=floor(this->head.height/this->rows);
      //else we add one to the height of the submatrix and the last row will have less elements
      else
	{
	  this->normal_height = floor(this->head.height/this->rows)+1;
	  this->last_height = this->head.height - (this->rows-1)*(floor(this->head.height/this->rows)+1);	
	  if(this->row!=this->rows-1)
	    this->loc_head.height=this->normal_height;
	  else
	    this->loc_head.height=this->last_height;
	    }*/
	
      this->loc_head.x=this->head.x+this->col*this->loc_head.width*this->head.spacing;
      this->loc_head.y=this->head.y-this->row*this->loc_head.height*this->head.spacing;
      this->loc_head.spacing=this->head.spacing;
      this->loc_head.nodata=this->head.nodata;

#ifdef DEBUG
      std::stringstream st2;
      st2<<"Global width and height :"<<this->head.width<<","<<this->head.height<<"\n";
      Mpi_::print(st2.str());
#endif

#ifdef DEBUG
      std::stringstream st1;
      st1<<"Local width and height :"<<this->loc_head.width<<","<<this->loc_head.height<<"\n";
      Mpi_::print(st1.str());
#endif
      
      this->data = new T[this->loc_head.width*this->loc_head.height];
    }
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    void initValues(T value)
    //-------------------------------------------------------------------------------
    {
      for(unsigned int i=0;i<this->loc_head.width*this->loc_head.height;i++)
	{
	  this->data[i]=value;
	}
    }

    void phBLimits()
    //-------------------------------------------------------------------------------
    {
      swphbL = wphbL = shphbL = hphbL = 0;
      swphbR = wphbR = shphbR = hphbR = 0;
      swphbU = wphbU = shphbU = hphbU = 0;
      swphbD = wphbD = shphbD = hphbD = 0;
    }

    //-------------------------------------------------------------------------------
    //! Read the zone file concerned for the current MPI process
    /*!
      read the MPI prcess zone, and spread the data in the subMatrices OpenMP
      only one submatrice if OpenMP is disabled
      \param binFile is binary file to read
    */
    //-------------------------------------------------------------------------------
    void read(const char * binFile)
    //-------------------------------------------------------------------------------
    {
      //read the zone of the file for my own block of data
      std::ifstream f(binFile, std::ios::binary | std::ios::in);

      //move to the begining of the bloc of data concerned
      unsigned int mvline;
      if(this->row<this->remainderh && this->col<this->remainderw)
	mvline = sizeof(HEADER) + (this->col*this->loc_head.width
				   +this->row*this->loc_head.height*this->head.width)*sizeof(T);
      else if(this->row<this->remainderh)
	mvline = sizeof(HEADER) + (this->remainderw*(this->loc_head.width+1)+(this->col-this->remainderw)*this->loc_head.width
				   +(this->row*this->loc_head.height)
				   *this->head.width)*sizeof(T);
      else if(this->col<this->remainderw)
	mvline = sizeof(HEADER) + (this->col*this->loc_head.width
				   +(this->remainderh*(this->loc_head.height+1)+(this->row-this->remainderh)*this->loc_head.height)
				   *this->head.width)*sizeof(T);
      else
	mvline = sizeof(HEADER) + (this->remainderw*(this->loc_head.width+1)+(this->col-this->remainderw)*this->loc_head.width
				   +(this->remainderh*(this->loc_head.height+1)+(this->row-this->remainderh)*this->loc_head.height)
				   *this->head.width)*sizeof(T);

      f.seekg(mvline,std::ios::beg);

      //read by lines
      for(unsigned int j=0;j<this->loc_head.height;j++)
	{
	  if(j>0)
	    {
	      //move to the next line of the bloc
	      unsigned int mv = this->head.width - this->loc_head.width;
	      f.seekg(mv*sizeof(T),std::ios::cur);
	    }

	  for(unsigned int k=0;k<this->loc_head.width;k++)
	    {
	      T val;
	      unsigned int rank = j*this->loc_head.width + k;
	      f.read(reinterpret_cast<char*>(&val),sizeof(T));
	      this->data[rank]=val;
	    }
	}
    }
    //-------------------------------------------------------------------------------
    void indexesIt(unsigned int rank,int &col,int &li)
    //-------------------------------------------------------------------------------
    {
      li = floor(rank/this->loc_head.width);
      col = rank%this->loc_head.width;
    }
    //-------------------------------------------------------------------------------

  public:
    //! constructor of the distributed matrix
    /*!
      This constructor will simply allocate the good space.
      \param h is the header to use to build your matrix
      \param value is the default value to put in the matrix
    */
    //-------------------------------------------------------------------------------
    DMatrix_impl(HEADER h,const T value) : DMatrix_base<T>()
    //-------------------------------------------------------------------------------
    {
      this->head=h;
      create();
      initValues(value);
      phBLimits();
      //print();
    }
  
    //! constructor of the distributed matrix
    /*!
      The constructor evaluate how to divide the matrix in submatrice and do the work.
      \param binFile is the path of the binary file you want to work on
    */
    //-------------------------------------------------------------------------------
    DMatrix_impl(const char * binFile) : DMatrix_base<T>()
    //-------------------------------------------------------------------------------
    {
      this->head = Header::read(binFile);
      create();
      initValues(0);
      read(binFile);
      //print();
    }
    //-------------------------------------------------------------------------------
  
    //! destructor of the distributed matrix
    /*!  */
    //-------------------------------------------------------------------------------
    ~DMatrix_impl()
    //-------------------------------------------------------------------------------
    {
      if(NULL!=this->data)
	delete [] this->data;
    }
    //-------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------
    inline T getValue(iterator<T,0> it){return this->data[it._rank];}
    inline T getValue(iterator_cont<T,0> it){return this->data[it._rank];}
    inline T getValue(iterator_step<T,0> it){return this->data[it._rank];}
    inline T getValue(iterator_rev<T,0> it){return this->data[it._rank];}
    //-------------------------------------------------------------------------------
    inline T& getValueRef(iterator<T,0> it){return this->data[it._rank];}
    inline T& getValueRef(iterator_cont<T,0> it){return this->data[it._rank];}
    inline T& getValueRef(iterator_step<T,0> it){return this->data[it._rank];}
    inline T& getValueRef(iterator_rev<T,0> it){return this->data[it._rank];}
    //-------------------------------------------------------------------------------
    inline void setValue(iterator<T,0> it,T value){this->data[it._rank]=value;}
    inline void setValue(iterator_cont<T,0> it,T value){this->data[it._rank]=value;}
    inline void setValue(iterator_step<T,0> it,T value){this->data[it._rank]=value;}
    inline void setValue(iterator_rev<T,0> it,T value){this->data[it._rank]=value;}
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    inline iterator<T,0> begin(){return iterator<T,0>(0,this->loc_head.width);}
    inline iterator_cont<T,0> begin_cont(){return iterator_cont<T,0>(0,this->loc_head.width);}
    inline iterator_step<T,0> begin_step(int step,int nb){return iterator_step<T,0>(0,step,nb);}
    inline iterator_rev<T,0> begin_rev(){return iterator_rev<T,0>(this->loc_head.width*this->loc_head.height-1,this->loc_head.width);}
    inline iterator_line<T,0> begin_line(){return iterator_line<T,0>(0,this->loc_head.width);}
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    inline iterator<T,0> end(){return iterator<T,0>(this->loc_head.width*this->loc_head.height-1,this->loc_head.width);}
    inline iterator_cont<T,0> end_cont(){return iterator_cont<T,0>(this->loc_head.width*this->loc_head.height-1,this->loc_head.width);}
    inline iterator_step<T,0> end_step(){return iterator_step<T,0>(this->loc_head.width*this->loc_head.height-1,this->loc_head.width);}
    inline iterator_rev<T,0> end_rev(){return iterator_rev<T,0>(0,this->loc_head.width);}
    inline iterator_line<T,0> end_line(){return iterator_line<T,0>(this->loc_head.width*this->loc_head.height-this->loc_head.width,this->loc_head.width);}
    //-------------------------------------------------------------------------------
    //! get the iterator on the matrix at position (x,y)
    /*!
      \param x is row index
      \pram y is column index
      \return returns the iterator at position (x,y)
    */
    iterator<T,0> getIterator(int col,int li)
    //-------------------------------------------------------------------------------
    {
      if(NULL!=this->data)
	{
	  unsigned int rank = li*this->loc_head.width+col;
	  return iterator<T,0>(rank,this->loc_head.width);
	}
    }
    //-------------------------------------------------------------------------------
    //! get the iterator contiguous on the matrix at position (x,y)
    /*!
      \param x is row index
      \pram y is column index
      \return returns the iterator at position (x,y)
    */
    iterator_cont<T,0> getIterator_cont(int col,int li)
    //-------------------------------------------------------------------------------
    {
      if(NULL!=this->data)
	{
	  unsigned int rank = li*this->loc_head.width+col;
	  return iterator_cont<T,0>(rank,this->loc_head.width);
	}
    }
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    //! get the iterator reverse on the matrix at position (x,y)
    /*!
      \param x is row index
      \pram y is column index
      \return returns the iterator at position (x,y)
    */
    iterator_rev<T,0> getIterator_rev(int col,int li)
    //-------------------------------------------------------------------------------
    {
      if(NULL!=this->data)
	{
	  unsigned int rank = li*this->loc_head.width+col;
	  return iterator_rev<T,0>(rank,this->loc_head.width);
	}
    }
    //-------------------------------------------------------------------------------
    //! get the indexes of the iterator on the matrix
    /*!
      \param it is the iterator from which the indexes will be taken
      \param x is the iterator position x returned
      \param y is the iterator position y returned
    */
    void getIndexes(iterator<T,0> it,int &col,int &li){indexesIt(it._rank,col,li);}
    void getIndexes(iterator_cont<T,0> it,int &col,int &li){indexesIt(it._rank,col,li);}
    void getIndexes(iterator_rev<T,0> it,int &col,int &li){indexesIt(it._rank,col,li);}
    //-------------------------------------------------------------------------------
    T& operator[](iterator<T,0> it) { return this->data[it._rank]; }
    T& operator[](iterator_cont<T,0> it) { return this->data[it._rank]; }
    T& operator[](iterator_step<T,0> it) { return this->data[it._rank]; }
    T& operator[](iterator_rev<T,0> it) { return this->data[it._rank]; }
    //-------------------------------------------------------------------------------

    //! Print the matrix by bloc
    /*!
     */
    //-------------------------------------------------------------------------------
    void print()
    //-------------------------------------------------------------------------------
    {
      std::stringstream st;
      for(unsigned int i=0;i<this->loc_head.height;i++)
	{
	  for(unsigned int j=0;j<this->loc_head.width;j++)
	    {
	      st<<this->data[border*this->loc_head.width + border + i*this->loc_head.width+j]<<" ";
	    }
	  st<<"\n";
	}
      Mpi_::print(st.str());
    }
    //-------------------------------------------------------------------------------
    inline HEADER getGlobalHeader(){return this->head;}
    //-------------------------------------------------------------------------------
    inline HEADER getHeader(){return this->loc_head;}
    //-------------------------------------------------------------------------------
    inline int getCols(){return this->cols;}
    //-------------------------------------------------------------------------------
    inline int getRows(){return this->rows;}
    //-------------------------------------------------------------------------------
    inline int getCol(){return this->col;}
    //-------------------------------------------------------------------------------
    inline int getRow(){return this->row;}
    //-------------------------------------------------------------------------------
    inline T * getData(){return this->data;}
    inline unsigned int getWidth(){return this->head.width;}
    inline unsigned int getWidthLoc(){return this->loc_head.width;}
    inline unsigned int getHeightLoc(){return this->loc_head.height;}
    inline int getBorder(){return 0;}
    //-------------------------------------------------------------------------------
    inline unsigned int getStartWPHBL(){return swphbL;}
    inline unsigned int getWidthPHBL(){return wphbL;}
    inline unsigned int getStartHPHBL(){return shphbL;}
    inline unsigned int getHeightPHBL(){return hphbL;}

    inline unsigned int getStartWPHBR(){return swphbR;}
    inline unsigned int getWidthPHBR(){return wphbR;}
    inline unsigned int getStartHPHBR(){return shphbR;}
    inline unsigned int getHeightPHBR(){return hphbR;}

    inline unsigned int getStartWPHBU(){return swphbU;}
    inline unsigned int getWidthPHBU(){return wphbU;}
    inline unsigned int getStartHPHBU(){return shphbU;}
    inline unsigned int getHeightPHBU(){return hphbU;}

    inline unsigned int getStartWPHBD(){return swphbD;}
    inline unsigned int getWidthPHBD(){return wphbD;}
    inline unsigned int getStartHPHBD(){return shphbD;}
    inline unsigned int getHeightPHBD(){return hphbD;}
  };
  //-------------------------------------------------------------------------------

}

#endif
