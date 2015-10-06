/**********************************************************************************
 * Interface of DMatrix for the user
 * Instantiation of the object DMatrix_impl 
 * 4 templates specializations
 ********************************************************************************/

#ifndef DMATRIX_H
#define DMATRIX_H

#include "dmatrix_impl.hpp"

namespace skelgis{

  //================================================================================
  //!  template Distributed Matrix interface Class with recovery and a line distribution
  /*!
    This template class defines the distributed matrix interface with recovery with a distribution in line
    T = type of data in the matrix
    R = recovery wanted
    line = domain decomposition by line (true, default value) or blocs (false)
  */
  //-------------------------------------------------------------------------------
  template<class T,int R,bool line=true> struct DMatrix
  //-------------------------------------------------------------------------------
  {
  protected:
    //!pointer to DMatrix_impl object
    DMatrix_impl<T,R,line> * m;

  public:
    //! default constructor of the distributed matrix interface
    /*!
    */
    //-------------------------------------------------------------------------------
    DMatrix(){}
    //-------------------------------------------------------------------------------
    //! constructor of the distributed matrix interface
    /*!
      \param _m is a DMatrix_impl object
    */
    //-------------------------------------------------------------------------------
    DMatrix(DMatrix_impl<T,R,line> * _m){m=_m;}
    //-------------------------------------------------------------------------------
    //! constructor of the distributed matrix interface
    /*!
      \param _m is a DMatrix object
    */
    //-------------------------------------------------------------------------------
    DMatrix(DMatrix<T,R,line>& _m){m=_m.getDMatrix();}
    //-------------------------------------------------------------------------------
    //! constructor of the distributed matrix interface
    /*!
      \param h is the header to construct the object (see skelgis/util/utility.hpp)
      \param value is the default value to put in the matrix
    */
    //-------------------------------------------------------------------------------
    DMatrix(HEADER h,const T value){m = new DMatrix_impl<T,R,line>(h,value);}
    //-------------------------------------------------------------------------------
    //! constructor of the distributed matrix interface
    /*!
      \param binFile is a binary input file to construct the DMatrix with
    */
    //-------------------------------------------------------------------------------
    DMatrix(const char * binFile){m = new DMatrix_impl<T,R,line>(binFile);}
    //-------------------------------------------------------------------------------

    //! init of the distributed matrix interface
    /*!
      \param _m is a DMatrix_impl object
    */
    //-------------------------------------------------------------------------------
    void init(DMatrix_impl<T,R,line> * _m){m=_m;}
    //-------------------------------------------------------------------------------
    //! init of the distributed matrix interface
    /*!
      \param _m is a DMatrix object
    */
    //-------------------------------------------------------------------------------
    void init(DMatrix<T,R,line>& _m){m=_m.getDMatrix();}
    //-------------------------------------------------------------------------------
    //! init of the distributed matrix interface
    /*!
      \param h is the header to construct the object (see skelgis/util/utility.hpp)
      \param value is the default value to put in the matrix
    */
    //-------------------------------------------------------------------------------
    void init(HEADER h,const T value){m = new DMatrix_impl<T,R,line>(h,value);}
    //-------------------------------------------------------------------------------
    //! init of the distributed matrix interface
    /*!
      \param binFile is a binary input file to construct the DMatrix with
    */
    //-------------------------------------------------------------------------------
    void init(const char * binFile){m = new DMatrix_impl<T,R,line>(binFile);}
    //-------------------------------------------------------------------------------

    inline void erase(){delete m;}
  
    //! destructor of the distributed matrix
    /*!  */
    //-------------------------------------------------------------------------------
    ~DMatrix(){}
    //-------------------------------------------------------------------------------
    //! to get the value of the matrix at the iterator it (alternative to [it])
    /*!
      \param it is the iterator position
      \return the value at the iterator it
    */
    //-------------------------------------------------------------------------------
    /*inline T getValue(iterator<T,R> it){return m->getValue(it);}
    inline T getValue(iterator_cont<T,R> it){return m->getValue(it);}
    inline T getValue(iterator_step<T,R> it){return m->getValue(it);}
    inline T getValue(iterator_rev<T,R> it){return m->getValue(it);}
    inline T getValue(iterator_phb_left<T,R> it){return m->getValue(it);}
    inline T getValue(iterator_phb_right<T,R> it){return m->getValue(it);}
    inline T getValue(iterator_phb_up<T,R> it){return m->getValue(it);}
    inline T getValue(iterator_phb_down<T,R> it){return m->getValue(it);}*/
    //-------------------------------------------------------------------------------
    inline T getValue(iterator<T,R> it) const {return m->data[it._rank];}
    inline T getValue(iterator_cont<T,R> it) const {return m->data[it._rank];}
    inline T getValue(iterator_step<T,R> it) const {return m->data[it._rank];}
    inline T getValue(iterator_rev<T,R> it) const {return m->data[it._rank];}
    inline T getValue(iterator_phb_left<T,R> it) const {return m->data[it._rank];}
    inline T getValue(iterator_phb_right<T,R> it) const {return m->data[it._rank];}
    inline T getValue(iterator_phb_up<T,R> it) const {return m->data[it._rank];}
    inline T getValue(iterator_phb_down<T,R> it) const {return m->data[it._rank];}
    //-------------------------------------------------------------------------------
    //! to set the value of the matrix at the iterator it (alternative to [it]=)
    /*!
      \param it is the iterator position
      \param value is the value to set
    */
    //-------------------------------------------------------------------------------
    /*inline void setValue(iterator<T,R> it,T value){m->setValue(it,value);}
    inline void setValue(iterator_cont<T,R> it,T value){m->setValue(it,value);}
    inline void setValue(iterator_step<T,R> it,T value){m->setValue(it,value);}
    inline void setValue(iterator_rev<T,R> it,T value){m->setValue(it,value);}
    inline void setValue(iterator_phb_left<T,R> it,T value){m->setValue(it,value);}
    inline void setValue(iterator_phb_right<T,R> it,T value){m->setValue(it,value);}
    inline void setValue(iterator_phb_up<T,R> it,T value){m->setValue(it,value);}
    inline void setValue(iterator_phb_down<T,R> it,T value){m->setValue(it,value);}*/
    inline void setValue(iterator<T,R> it,T value){m->data[it._rank] = value;}
    inline void setValue(iterator_cont<T,R> it,T value){m->data[it._rank] = value;}
    inline void setValue(iterator_step<T,R> it,T value){m->data[it._rank] = value;}
    inline void setValue(iterator_rev<T,R> it,T value){m->data[it._rank] = value;}
    inline void setValue(iterator_phb_left<T,R> it,T value){m->data[it._rank] = value;}
    inline void setValue(iterator_phb_right<T,R> it,T value){m->data[it._rank] = value;}
    inline void setValue(iterator_phb_up<T,R> it,T value){m->data[it._rank] = value;}
    inline void setValue(iterator_phb_down<T,R> it,T value){m->data[it._rank] = value;}
    //-------------------------------------------------------------------------------
    //! Set value in the middle of the global matrix
    /*!
      \param val is the value to affect
    */
    //-------------------------------------------------------------------------------
    void setGlobalMiddleValue(T val){m->setGlobalMiddleValue(val);}
    //-------------------------------------------------------------------------------
    //! to set all the values of the physical border of the matrix
    /*!
      \param val is the value to set
    */
    //-------------------------------------------------------------------------------
    inline void setPhysicalBorder(T val)
    {
      m->setPhysicalBorder(val);
    }
    //-------------------------------------------------------------------------------
    //! to set all the values on the right of physical border of the matrix
    /*!
      \param val is the value to set
    */
    //-------------------------------------------------------------------------------
    inline void setRightPhysicalBorder(T val)
    {
      m->setRightPhysicalBorder(val);
    }
    //-------------------------------------------------------------------------------
    //! to set all the values on the left of physical border of the matrix
    /*!
      \param val is the value to set
    */
    //-------------------------------------------------------------------------------
    inline void setLeftPhysicalBorder(T val)
    {
      m->setLeftPhysicalBorder(val);
    }
    //-------------------------------------------------------------------------------
    //! to set all the values on the up of physical border of the matrix
    /*!
      \param val is the value to set
    */
    //-------------------------------------------------------------------------------
    inline void setUpPhysicalBorder(T val)
    {
      m->setUpPhysicalBorder(val);
    }
    //-------------------------------------------------------------------------------
    //! to set all the values on the down of physical border of the matrix
    /*!
      \param val is the value to set
    */
    //-------------------------------------------------------------------------------
    inline void setDownPhysicalBorder(T val)
    {
      m->setDownPhysicalBorder(val);
    }
    //-------------------------------------------------------------------------------
    //! to get the beginning iterator of the DMatrix (on the first element of the DMatrix)
    /*!
      \return the iterator of the first element
    */
    //-------------------------------------------------------------------------------
    inline iterator<T,R> begin(){return m->begin();}
    inline iterator_cont<T,R> begin_cont(){return m->begin_cont();}
    inline iterator_rev<T,R> begin_rev(){return m->begin_rev();}
    inline iterator_step<T,R> begin_step(int step,int nb){return m->begin_step(step,nb);}
    inline iterator_phb_left<T,R> begin_phb_left(){return m->begin_phb_left();}
    inline iterator_phb_right<T,R> begin_phb_right(){return m->begin_phb_right();}
    inline iterator_phb_up<T,R> begin_phb_up(){return m->begin_phb_up();}
    inline iterator_phb_down<T,R> begin_phb_down(){return m->begin_phb_down();}
    inline iterator_line<T,R> begin_line(){return m->begin_line();}
    //-------------------------------------------------------------------------------
    //! to get the ending iterator of the DMatrix (on the last element of the DMatrix)
    /*!
      \return the iterator of the last element
    */
    //-------------------------------------------------------------------------------
    inline iterator<T,R> end(){return m->end();}
    inline iterator_cont<T,R> end_cont(){return m->end_cont();}
    inline iterator_rev<T,R> end_rev(){return m->end_rev();}
    inline iterator_step<T,R> end_step(){return m->end_step();}
    inline iterator_phb_left<T,R> end_phb_left(){return m->end_phb_left();}
    inline iterator_phb_right<T,R> end_phb_right(){return m->end_phb_right();}
    inline iterator_phb_up<T,R> end_phb_up(){return m->end_phb_up();}
    inline iterator_phb_down<T,R> end_phb_down(){return m->end_phb_down();}
    inline iterator_line<T,R> end_line(){return m->end_line();}
    //-------------------------------------------------------------------------------
    //! operator [] to get and set an element of the matrix at iterator it
    /*!
      \param it is the iterator position
      \return the reference to the value at it
    */
    //-------------------------------------------------------------------------------
/*    inline T& operator[](iterator<T,R> it) { return m->getValueRef(it); }
    inline T& operator[](iterator_cont<T,R> it) { return m->getValueRef(it); }
    inline T& operator[](iterator_rev<T,R> it) { return m->getValueRef(it); }
    inline T& operator[](iterator_step<T,R> it) { return m->getValueRef(it); }
    inline T& operator[](iterator_phb_left<T,R> it) { return m->getValueRef(it); }
    inline T& operator[](iterator_phb_right<T,R> it) { return m->getValueRef(it); }
    inline T& operator[](iterator_phb_up<T,R> it) { return m->getValueRef(it); }
    inline T& operator[](iterator_phb_down<T,R> it) { return m->getValueRef(it); }*/
    //-------------------------------------------------------------------------------
    inline T& operator[](iterator<T,R> it) { return m->data[it._rank]; }
    inline T& operator[](iterator_cont<T,R> it) { return m->data[it._rank]; }
    inline T& operator[](iterator_rev<T,R> it) { return m->data[it._rank]; }
    inline T& operator[](iterator_step<T,R> it) { return m->data[it._rank]; }
    inline T& operator[](iterator_phb_left<T,R> it) { return m->data[it._rank]; }
    inline T& operator[](iterator_phb_right<T,R> it) { return m->data[it._rank]; }
    inline T& operator[](iterator_phb_up<T,R> it) { return m->data[it._rank]; }
    inline T& operator[](iterator_phb_down<T,R> it) { return m->data[it._rank]; }
    //-------------------------------------------------------------------------------
    inline T operator[](iterator<T,R> it) const { return m->data[it._rank]; }
    inline T operator[](iterator_cont<T,R> it) const { return m->data[it._rank]; }
    inline T operator[](iterator_rev<T,R> it) const { return m->data[it._rank]; }
    inline T operator[](iterator_step<T,R> it) const { return m->data[it._rank]; }
    inline T operator[](iterator_phb_left<T,R> it) const { return m->data[it._rank]; }
    inline T operator[](iterator_phb_right<T,R> it) const { return m->data[it._rank]; }
    inline T operator[](iterator_phb_up<T,R> it) const { return m->data[it._rank]; }
    inline T operator[](iterator_phb_down<T,R> it) const { return m->data[it._rank]; }
    //-------------------------------------------------------------------------------
    //! operator = to assign the current DMatrix interface object to another one
    /*!
      \param right is the other DMatrix object
      \return the reference to the new current DMatrix object
    */
    //-------------------------------------------------------------------------------
    inline DMatrix<T,R,line>& operator=(DMatrix<T,R,line>& right){m = right.getDMatrix();}
    //-------------------------------------------------------------------------------
    //! Print the matrix by bloc
    /*!
     */
    //-------------------------------------------------------------------------------
    inline void print(){m->print();}
    inline void printAll(){m->printAll();}
    //-------------------------------------------------------------------------------
    //! get borders
    /*!
      Get the borders from other processors
      MPI exchanges
    */
    //-------------------------------------------------------------------------------
    inline void getBorders(){m->getBorders();}
    //-------------------------------------------------------------------------------
    //! get all neighbor right values for the element at iterator it
    /*!
      \param it is the iterator position
      \return a table of right values
    */
    //-------------------------------------------------------------------------------
    inline T * getAllRight(iterator<T,R> it){return m->getAllRight(it);}
    inline T * getAllRight(iterator_cont<T,R> it){return m->getAllRight(it);}
    inline T * getAllRight(iterator_rev<T,R> it){return m->getAllRight(it);}
    inline T * getAllRight(iterator_step<T,R> it){return m->getAllRight(it);}
    inline T * getAllRight(iterator_phb_left<T,R> it){return m->getAllRight(it);}
    //-------------------------------------------------------------------------------
    //! get all neighbor left values for the element at iterator it
    /*!
      \param it is the iterator position
      \return a table of left values
    */
    //-------------------------------------------------------------------------------
    inline T * getAllLeft(iterator<T,R> it){return m->getAllLeft(it);}
    inline T * getAllLeft(iterator_cont<T,R> it){return m->getAllLeft(it);}
    inline T * getAllLeft(iterator_rev<T,R> it){return m->getAllLeft(it);}
    inline T * getAllLeft(iterator_step<T,R> it){return m->getAllLeft(it);}
    inline T * getAllLeft(iterator_phb_right<T,R> it){return m->getAllLeft(it);}
    //-------------------------------------------------------------------------------
    //! get all neighbor up values for the element at iterator it
    /*!
      \param it is the iterator position
      \return a table of up values
    */
    //-------------------------------------------------------------------------------
    inline T * getAllUp(iterator<T,R> it){return m->getAllUp(it);}
    inline T * getAllUp(iterator_cont<T,R> it){return m->getAllUp(it);}
    inline T * getAllUp(iterator_rev<T,R> it){return m->getAllUp(it);}
    inline T * getAllUp(iterator_step<T,R> it){return m->getAllUp(it);}
    inline T * getAllUp(iterator_phb_down<T,R> it){return m->getAllUp(it);}
    //-------------------------------------------------------------------------------
    //! get all neighbor down values for the element at iterator it
    /*!
      \param it is the iterator position
      \return a table of down values
    */
    //-------------------------------------------------------------------------------
    inline T * getAllDown(iterator<T,R> it){return m->getAllDown(it);}
    inline T * getAllDown(iterator_cont<T,R> it){return m->getAllDown(it);}
    inline T * getAllDown(iterator_rev<T,R> it){return m->getAllDown(it);}
    inline T * getAllDown(iterator_step<T,R> it){return m->getAllDown(it);}
    inline T * getAllDown(iterator_phb_up<T,R> it){return m->getAllDown(it);}
    //-------------------------------------------------------------------------------
    //! get all neighbor right down values for the element at iterator it
    /*!
      \param it is the iterator position
      \return a table of right down values
    */
    //-------------------------------------------------------------------------------
    inline T * getAllRightDown(iterator<T,R> it){return m->getAllRightDown(it);}
    inline T * getAllRightDown(iterator_cont<T,R> it){return m->getAllRightDown(it);}
    inline T * getAllRightDown(iterator_rev<T,R> it){return m->getAllRightDown(it);}
    inline T * getAllRightDown(iterator_step<T,R> it){return m->getAllRightDown(it);}
    //-------------------------------------------------------------------------------
    //! get all neighbor left down values for the element at iterator it
    /*!
      \param it is the iterator position
      \return a table of left down values
    */
    //-------------------------------------------------------------------------------
    inline T * getAllLeftDown(iterator<T,R> it){return m->getAllLeftDown(it);}
    inline T * getAllLeftDown(iterator_cont<T,R> it){return m->getAllLeftDown(it);}
    inline T * getAllLeftDown(iterator_rev<T,R> it){return m->getAllLeftDown(it);}
    inline T * getAllLeftDown(iterator_step<T,R> it){return m->getAllLeftDown(it);}
    //-------------------------------------------------------------------------------
    //! get all neighbor right up values for the element at iterator it
    /*!
      \param it is the iterator position
      \return a table of right up values
    */
    //-------------------------------------------------------------------------------
    inline T * getAllRightUp(iterator<T,R> it){return m->getAllRightUp(it);}
    inline T * getAllRightUp(iterator_cont<T,R> it){return m->getAllRightUp(it);}
    inline T * getAllRightUp(iterator_rev<T,R> it){return m->getAllRightUp(it);}
    inline T * getAllRightUp(iterator_step<T,R> it){return m->getAllRightUp(it);}
    //-------------------------------------------------------------------------------
    //! get all neighbor left up values for the element at iterator it
    /*!
      \param it is the iterator position
      \return a table of left up values
    */
    //-------------------------------------------------------------------------------
    inline T * getAllLeftUp(iterator<T,R> it){return m->getAllLeftUp(it);}
    inline T * getAllLeftUp(iterator_cont<T,R> it){return m->getAllLeftUp(it);}
    inline T * getAllLeftUp(iterator_rev<T,R> it){return m->getAllLeftUp(it);}
    inline T * getAllLeftUp(iterator_step<T,R> it){return m->getAllLeftUp(it);}
    //-------------------------------------------------------------------------------
    //! get the right neighbor value for the element at iterator it
    /*!
      \param it is the iterator position
      \return the right neighbor element value
    */
    //-------------------------------------------------------------------------------
    inline T getRight(iterator<T,R> it){return m->getRight(it);}
    inline T getRight(iterator_cont<T,R> it){return m->getRight(it);}
    inline T getRight(iterator_rev<T,R> it){return m->getRight(it);}
    inline T getRight(iterator_step<T,R> it){return m->getRight(it);}
    inline T getRight(iterator_phb_left<T,R> it){return m->getRight(it);}
    //-------------------------------------------------------------------------------
    //! get the left neighbor value for the element at iterator it
    /*!
      \param it is the iterator position
      \return the left neighbor element value
    */
    //-------------------------------------------------------------------------------
    inline T getLeft(iterator<T,R> it){return m->getLeft(it);}
    inline T getLeft(iterator_cont<T,R> it){return m->getLeft(it);}
    inline T getLeft(iterator_rev<T,R> it){return m->getLeft(it);}
    inline T getLeft(iterator_step<T,R> it){return m->getLeft(it);}
    inline T getLeft(iterator_phb_right<T,R> it){return m->getLeft(it);}
    //-------------------------------------------------------------------------------
    //! get the up neighbor value for the element at iterator it
    /*!
      \param it is the iterator position
      \return the up neighbor element value
    */
    //-------------------------------------------------------------------------------
    inline T getUp(iterator<T,R> it){return m->getUp(it);}
    inline T getUp(iterator_cont<T,R> it){return m->getUp(it);}
    inline T getUp(iterator_rev<T,R> it){return m->getUp(it);}
    inline T getUp(iterator_step<T,R> it){return m->getUp(it);}
    inline T getUp(iterator_phb_down<T,R> it){return m->getUp(it);}
    //-------------------------------------------------------------------------------
    //! get the down neighbor value for the element at iterator it
    /*!
      \param it is the iterator position
      \return the down neighbor element value
    */
    //-------------------------------------------------------------------------------
    inline T getDown(iterator<T,R> it){return m->getDown(it);}
    inline T getDown(iterator_cont<T,R> it){return m->getDown(it);}
    inline T getDown(iterator_rev<T,R> it){return m->getDown(it);}
    inline T getDown(iterator_step<T,R> it){return m->getDown(it);}
    inline T getDown(iterator_phb_up<T,R> it){return m->getDown(it);}
    //-------------------------------------------------------------------------------
    //! get the right down neighbor value for the element at iterator it
    /*!
      \param it is the iterator position
      \return the right down neighbor element value
    */
    //-------------------------------------------------------------------------------
    inline T getRightDown(iterator<T,R> it){return m->getRightDown(it);}
    inline T getRightDown(iterator_cont<T,R> it){return m->getRightDown(it);}
    inline T getRightDown(iterator_rev<T,R> it){return m->getRightDown(it);}
    inline T getRightDown(iterator_step<T,R> it){return m->getRightDown(it);}
    //-------------------------------------------------------------------------------
    //! get the left down neighbor value for the element at iterator it
    /*!
      \param it is the iterator position
      \return the left down neighbor element value
    */
    //-------------------------------------------------------------------------------
    inline T getLeftDown(iterator<T,R> it){return m->getLeftDown(it);}
    inline T getLeftDown(iterator_cont<T,R> it){return m->getLeftDown(it);}
    inline T getLeftDown(iterator_rev<T,R> it){return m->getLeftDown(it);}
    inline T getLeftDown(iterator_step<T,R> it){return m->getLeftDown(it);}
    //-------------------------------------------------------------------------------
    //! get the right up neighbor value for the element at iterator it
    /*!
      \param it is the iterator position
      \return the right up neighbor element value
    */
    //-------------------------------------------------------------------------------
    inline T getRightUp(iterator<T,R> it){return m->getRightUp(it);}
    inline T getRightUp(iterator_cont<T,R> it){return m->getRightUp(it);}
    inline T getRightUp(iterator_rev<T,R> it){return m->getRightUp(it);}
    inline T getRightUp(iterator_step<T,R> it){return m->getRightUp(it);}
    //-------------------------------------------------------------------------------
    //! get the left up neighbor value for the element at iterator it
    /*!
      \param it is the iterator position
      \return the left up neighbor element value
    */
    //-------------------------------------------------------------------------------
    inline T getLeftUp(iterator<T,R> it){return m->getLeftUp(it);}
    inline T getLeftUp(iterator_cont<T,R> it){return m->getLeftUp(it);}
    inline T getLeftUp(iterator_rev<T,R> it){return m->getLeftUp(it);}
    inline T getLeftUp(iterator_step<T,R> it){return m->getLeftUp(it);}
    //-------------------------------------------------------------------------------
    //! get all values along X axe (all left and all right) for the element at iterator it
    /*!
      \param it is the iterator position
      \return a table of all neighbor values along X
    */
    //-------------------------------------------------------------------------------
    inline T * getAllX(iterator<T,R> it){return m->getAllX(it);}
    inline T * getAllX(iterator_cont<T,R> it){return m->getAllX(it);}
    inline T * getAllX(iterator_rev<T,R> it){return m->getAllX(it);}
    inline T * getAllX(iterator_step<T,R> it){return m->getAllX(it);}
    //-------------------------------------------------------------------------------
    inline void getAllX(iterator<T,R> it, T * t){return m->getAllX(it,t);}
    inline void getAllX(iterator_cont<T,R> it, T * t){return m->getAllX(it,t);}
    inline void getAllX(iterator_rev<T,R> it, T * t){return m->getAllX(it,t);}
    inline void getAllX(iterator_step<T,R> it, T * t){return m->getAllX(it,t);}
    //-------------------------------------------------------------------------------

    //! get all values along Y axe (all up and all down) for the element at iterator it
    /*!
      \param it is the iterator position
      \return a table of all neighbor values along Y
    */
    //-------------------------------------------------------------------------------
    inline T * getAllY(iterator<T,R> it){return m->getAllY(it);}
    inline T * getAllY(iterator_cont<T,R> it){return m->getAllY(it);}
    inline T * getAllY(iterator_rev<T,R> it){return m->getAllY(it);}
    inline T * getAllY(iterator_step<T,R> it){return m->getAllY(it);}
    //-------------------------------------------------------------------------------
    inline void getAllY(iterator<T,R> it, T * t){return m->getAllY(it,t);}
    inline void getAllY(iterator_cont<T,R> it, T * t){return m->getAllY(it,t);}
    inline void getAllY(iterator_rev<T,R> it, T * t){return m->getAllY(it,t);}
    inline void getAllY(iterator_step<T,R> it, T * t){return m->getAllY(it,t);}
    //-------------------------------------------------------------------------------
    //! get all 8 directions neighbor values for the element at iterator it
    /*!
      \param it is the iterator position
      \return a table of all 8 directions neighbor values
    */
    //-------------------------------------------------------------------------------
    inline T * get8(iterator<T,R> it){return m->get8(it);}
    inline T * get8(iterator_cont<T,R> it){return m->get8(it);}
    inline T * get8(iterator_rev<T,R> it){return m->get8(it);}
    inline T * get8(iterator_step<T,R> it){return m->get8(it);}
    //-------------------------------------------------------------------------------
    //! get all 4 directions neighbor values for the element at iterator it
    /*!
      \param it is the iterator position
      \return a table of all 4 directions neighbor values
    */
    //-------------------------------------------------------------------------------
    inline T * get4(iterator<T,R> it){return m->get4(it);}
    inline T * get4(iterator_cont<T,R> it){return m->get4(it);}
    inline T * get4(iterator_rev<T,R> it){return m->get4(it);}
    inline T * get4(iterator_step<T,R> it){return m->get4(it);}
    //-------------------------------------------------------------------------------
    //! get the first neighbor value inside the domain for physical border iterators whatever is the recovery size
    /*!
      \param it is the physical border iterator position
      \return the neighbor value
    */
    //-------------------------------------------------------------------------------
    inline T getInRight(iterator_phb_left<T,R> it){return m->getInRight(it);}
    inline T getInLeft(iterator_phb_right<T,R> it){return m->getInLeft(it);}
    inline T getInUp(iterator_phb_down<T,R> it){return m->getInUp(it);}
    inline T getInDown(iterator_phb_up<T,R> it){return m->getInDown(it);}
    //-------------------------------------------------------------------------------
    //! get the iterator on the matrix at position (x,y)
    /*!
      \param x is row index
      \pram y is column index
      \return returns the iterator at position (x,y)
    */
    //-------------------------------------------------------------------------------
    iterator<T,R> getIterator(int x,int y){return m->getIterator(x,y);}
    iterator<T,R> getIterator_cont(int x,int y){return m->getIterator_cont(x,y);}
    iterator<T,R> getIterator_rev(int x,int y){return m->getIterator_rev(x,y);}
    //-------------------------------------------------------------------------------
    //! get the indexes of the iterator on the matrix
    /*!
      \param it is the iterator from which the indexes will be taken
      \param x is the iterator position x returned
      \param y is the iterator position y returned
    */
    //-------------------------------------------------------------------------------
    void getIndexes(iterator<T,R> it,int &col,int &li){m->getIndexes(it,col,li);}
    void getIndexes(iterator_cont<T,R> it,int &col,int &li){m->getIndexes(it,col,li);}
    void getIndexes(iterator_rev<T,R> it,int &col,int &li){m->getIndexes(it,col,li);}
    //-------------------------------------------------------------------------------
    //! get the global header of the DMatrix
    /*!
      \return the global header
    */
    //-------------------------------------------------------------------------------
    inline HEADER getGlobalHeader(){return m->getGlobalHeader();}
    //-------------------------------------------------------------------------------
    //! get the header of the DMatrix
    /*!
      \return the header
    */
    //-------------------------------------------------------------------------------
    inline HEADER getHeader(){return m->getHeader();}
    //-------------------------------------------------------------------------------
    //! get the border header of the DMatrix
    /*!
      \return the border header
    */
    //-------------------------------------------------------------------------------
    inline HEADER getBorderHeader(){return m->getBorderHeader();}
    //-------------------------------------------------------------------------------
    //! get the number of columns in the domain decomposition
    /*!
      \return the number of columns in the domain decomposition
    */
    //-------------------------------------------------------------------------------
    inline int getCols(){return m->getCols();}
    //-------------------------------------------------------------------------------
    //! get the number of rows in the domain decomposition
    /*!
      \return the number of rows in the domain decomposition
    */
    //-------------------------------------------------------------------------------
    inline int getRows(){return m->getRows();}
    //-------------------------------------------------------------------------------
    //! get the column number of the DMatrix
    /*!
      \return the column number of the DMatrix
    */
    //-------------------------------------------------------------------------------
    inline int getCol(){return m->getCol();}
    //-------------------------------------------------------------------------------
    //! get the row number of the DMatrix
    /*!
      \return the row number of the DMatrix
    */
    //-------------------------------------------------------------------------------
    inline int getRow(){return m->getRow();}
    //-------------------------------------------------------------------------------
    //! get the data table of the DMatrix
    /*!
      \return the data table of the DMatrix
    */
    //-------------------------------------------------------------------------------
    inline T * getData(){return m->getData();}
    inline unsigned int getWidth(){return m->getWidth();}
    inline unsigned int getWidthLoc(){return m->getWidthLoc();}
    inline unsigned int getHeightLoc(){return m->getHeightLoc();}
    //-------------------------------------------------------------------------------
    //! get the recovery of the DMatrix (=R)
    /*!
      \return the recovery of the DMatrix
    */
    //-------------------------------------------------------------------------------
    inline int getBorder(){return m->getBorder();}
    //-------------------------------------------------------------------------------
    //! get the DMatrix_impl associated to the DMatrix interface
    /*!
      \return the DMatrix_impl
    */
    //-------------------------------------------------------------------------------
    inline DMatrix_impl<T,R,line> * getDMatrix(){return m;}
    //-------------------------------------------------------------------------------
  };

  //================================================================================
  //!  template Distributed Matrix interface Class without recovery
  /*!
    This template class defines the distributed matrix interface without recovery
    T = type of data in the matrix
    R = 0
    line = true (line domain decomposition)
  */
  //-------------------------------------------------------------------------------
  template<class T> struct DMatrix<T,0,true>
  //-------------------------------------------------------------------------------
  {
  protected:
    //!pointer to DMatrix_impl object
    DMatrix_impl<T,0,true> * m;

  public:
    //! default constructor of the distributed matrix interface
    /*!
    */
    //-------------------------------------------------------------------------------
    DMatrix(){}
    //-------------------------------------------------------------------------------
    //! constructor of the distributed matrix interface
    /*!
      \param _m is a DMatrix_impl object
    */
    //-------------------------------------------------------------------------------
    DMatrix(DMatrix_impl<T,0,true> * _m){m=_m;}
    //-------------------------------------------------------------------------------
    //! constructor of the distributed matrix interface
    /*!
      \param _m is a DMatrix object
    */
    //-------------------------------------------------------------------------------
    DMatrix(DMatrix<T,0,true>& _m){m=_m.getDMatrix();}
    //-------------------------------------------------------------------------------
    //! constructor of the distributed matrix interface
    /*!
      \param h is the header to construct the object (see skelgis/util/utility.hpp)
      \param value is the default value to put in the matrix
    */
    //-------------------------------------------------------------------------------
    DMatrix(HEADER h,const T value){m = new DMatrix_impl<T,0,true>(h,value);}
    //-------------------------------------------------------------------------------
    //! constructor of the distributed matrix interface
    /*!
      \param binFile is a binary input file to construct the DMatrix with
    */
    //-------------------------------------------------------------------------------
    DMatrix(const char * binFile){m = new DMatrix_impl<T,0,true>(binFile);}
    //-------------------------------------------------------------------------------

    //! init of the distributed matrix interface
    /*!
      \param _m is a DMatrix_impl object
    */
    //-------------------------------------------------------------------------------
    void init(DMatrix_impl<T,0,true> * _m){m=_m;}
    //-------------------------------------------------------------------------------
    //! init of the distributed matrix interface
    /*!
      \param _m is a DMatrix object
    */
    //-------------------------------------------------------------------------------
    void init(DMatrix<T,0,true>& _m){m=_m.getDMatrix();}
    //-------------------------------------------------------------------------------
    //! init of the distributed matrix interface
    /*!
      \param h is the header to construct the object (see skelgis/util/utility.hpp)
      \param value is the default value to put in the matrix
    */
    //-------------------------------------------------------------------------------
    void init(HEADER h,const T value){m = new DMatrix_impl<T,0,true>(h,value);}
    //-------------------------------------------------------------------------------
    //! init of the distributed matrix interface
    /*!
      \param binFile is a binary input file to construct the DMatrix with
    */
    //-------------------------------------------------------------------------------
    void init(const char * binFile){m = new DMatrix_impl<T,0,true>(binFile);}
    //-------------------------------------------------------------------------------

    inline void erase(){delete m;}
  
    //! destructor of the distributed matrix
    /*!  */
    //-------------------------------------------------------------------------------
    ~DMatrix(){}
    //-------------------------------------------------------------------------------

    //! to get the value of the matrix at the iterator it (alternative to [it])
    /*!
      \param it is the iterator position
      \return the value at the iterator it
    */
    //-------------------------------------------------------------------------------
    /*inline T getValue(iterator<T,0> it){return m->getValue(it);}
    inline T getValue(iterator_cont<T,0> it){return m->getValue(it);}
    inline T getValue(iterator_rev<T,0> it){return m->getValue(it);}
    inline T getValue(iterator_step<T,0> it){return m->getValue(it);}*/
    //-------------------------------------------------------------------------------
    inline T getValue(iterator<T,0> it) const {return m->data[it._rank];}
    inline T getValue(iterator_cont<T,0> it) const {return m->data[it._rank];}
    inline T getValue(iterator_rev<T,0> it) const {return m->data[it._rank];}
    inline T getValue(iterator_step<T,0> it) const {return m->data[it._rank];}
    //-------------------------------------------------------------------------------
    //! to set the value of the matrix at the iterator it (alternative to [it]=)
    /*!
      \param it is the iterator position
    */
    //-------------------------------------------------------------------------------
    /*inline T setValue(iterator<T,0> it,T value){m->setValue(it,value);}
    inline T setValue(iterator_cont<T,0> it,T value){m->setValue(it,value);}
    inline T setValue(iterator_rev<T,0> it,T value){m->setValue(it,value);}
    inline T setValue(iterator_step<T,0> it,T value){m->setValue(it,value);}*/
    inline T setValue(iterator<T,0> it,T value){m->data[it._rank] = value;}
    inline T setValue(iterator_cont<T,0> it,T value){m->data[it._rank] = value;}
    inline T setValue(iterator_rev<T,0> it,T value){m->data[it._rank] = value;}
    inline T setValue(iterator_step<T,0> it,T value){m->data[it._rank] = value;}
    //-------------------------------------------------------------------------------
    //! to get the beginning iterator of the DMatrix (on the first element of the DMatrix)
    /*!
      \return the iterator of the first element
    */
    //-------------------------------------------------------------------------------
    inline iterator<T,0> begin(){return m->begin();}
    inline iterator_cont<T,0> begin_cont(){return m->begin_cont();}
    inline iterator_rev<T,0> begin_rev(){return m->begin_rev();}
    inline iterator_step<T,0> begin_step(int step,int nb){return m->begin_step(step,nb);}
    inline iterator_line<T,0> begin_line(){return m->begin_line();}
    //-------------------------------------------------------------------------------
    //! to get the ending iterator of the DMatrix (on the last element of the DMatrix)
    /*!
      \return the iterator of the last element
    */
    //-------------------------------------------------------------------------------
    inline iterator<T,0> end(){return m->end();}
    inline iterator_cont<T,0> end_cont(){return m->end_cont();}
    inline iterator_rev<T,0> end_rev(){return m->end_rev();}
    inline iterator_step<T,0> end_step(){return m->end_step();}
    inline iterator_line<T,0> end_line(){return m->end_line();}
    //-------------------------------------------------------------------------------
    //! operator [] to get and set an element of the matrix at iterator it
    /*!
      \param it is the iterator position
      \return the reference to the value at it
    */
    //-------------------------------------------------------------------------------
    inline T& operator[](iterator<T,0> it) { return m->data[it._rank]; }
    inline T& operator[](iterator_cont<T,0> it) { return m->data[it._rank]; }
    inline T& operator[](iterator_rev<T,0> it) { return m->data[it._rank]; }
    inline T& operator[](iterator_step<T,0> it) { return m->data[it._rank]; }
    //-------------------------------------------------------------------------------
    inline T operator[](iterator<T,0> it) const { return m->data[it._rank]; }
    inline T operator[](iterator_cont<T,0> it) const { return m->data[it._rank]; }
    inline T operator[](iterator_rev<T,0> it) const { return m->data[it._rank]; }
    inline T operator[](iterator_step<T,0> it) const { return m->data[it._rank]; }
    //-------------------------------------------------------------------------------
    //! operator = to assign the current DMatrix interface object to another object
    /*!
      \param right is the other DMatrix object
      \return the reference to the new current DMatrix object
    */
    //-------------------------------------------------------------------------------
    inline DMatrix<T,0,true>& operator=(DMatrix<T,0,true>& right){ m = right.getDMatrix();}
    //-------------------------------------------------------------------------------

    //! Print the matrix by bloc
    /*!
     */
    //-------------------------------------------------------------------------------
    inline void print(){m->print();}
    inline void printAll(){m->printAll();}
    //-------------------------------------------------------------------------------
    //! get the iterator on the matrix at position (x,y)
    /*!
      \param x is row index
      \pram y is column index
      \return returns the iterator at position (x,y)
    */
    //-------------------------------------------------------------------------------
    iterator<T,0> getIterator(int x,int y){return m->getIterator(x,y);}
    iterator<T,0> getIterator_cont(int x,int y){return m->getIterator_cont(x,y);}
    iterator<T,0> getIterator_rev(int x,int y){return m->getIterator_rev(x,y);}
    //-------------------------------------------------------------------------------
    //! get the indexes of the iterator on the matrix
    /*!
      \param it is the iterator from which the indexes will be taken
      \param x is the iterator position x returned
      \param y is the iterator position y returned
    */
    //-------------------------------------------------------------------------------
    void getIndexes(iterator<T,0> it,int &col,int &li){m->getIndexes(it,col,li);}
    void getIndexes(iterator_cont<T,0> it,int &col,int &li){m->getIndexes(it,col,li);}
    void getIndexes(iterator_rev<T,0> it,int &col,int &li){m->getIndexes(it,col,li);}
    //-------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------
    //! get the global header of the DMatrix
    /*!
      \return the global header
    */
    //-------------------------------------------------------------------------------
    inline HEADER getGlobalHeader(){return m->getGlobalHeader();}
    //-------------------------------------------------------------------------------
    //! get the header of the DMatrix
    /*!
      \return the header
    */
    //-------------------------------------------------------------------------------
    inline HEADER getHeader(){return m->getHeader();}
    //-------------------------------------------------------------------------------
    //! get the number of columns in the domain decomposition
    /*!
      \return the number of columns in the domain decomposition
    */
    //-------------------------------------------------------------------------------
    inline int getCols(){return m->getCols();}
    //-------------------------------------------------------------------------------
    //! get the number of rows in the domain decomposition
    /*!
      \return the number of rows in the domain decomposition
    */
    //-------------------------------------------------------------------------------
    inline int getRows(){return m->getRows();}
    //-------------------------------------------------------------------------------
    //! get the column number of the DMatrix
    /*!
      \return the column number of the DMatrix
    */
    //-------------------------------------------------------------------------------
    inline int getCol(){return m->getCol();}
    //-------------------------------------------------------------------------------
    //! get the row number of the DMatrix
    /*!
      \return the row number of the DMatrix
    */
    //-------------------------------------------------------------------------------
    inline int getRow(){return m->getRow();}
    //-------------------------------------------------------------------------------
    //! get the data table of the DMatrix
    /*!
      \return the data table of the DMatrix
    */
    //-------------------------------------------------------------------------------
    inline T * getData(){return m->getData();}
    inline unsigned int getWidth(){return m->getWidth();}
    inline unsigned int getWidthLoc(){return m->getWidthLoc();}
    inline unsigned int getHeightLoc(){return m->getHeightLoc();}
    //-------------------------------------------------------------------------------
    //! get the DMatrix_impl associated to the DMatrix interface
    /*!
      \return the DMatrix_impl
    */
    //-------------------------------------------------------------------------------
    inline DMatrix_impl<T,0,true> * getDMatrix(){return m;}
    //-------------------------------------------------------------------------------
  };

  //================================================================================
  //!  template Distributed Matrix interface Class with recovery
  /*!
    This template class defines the distributed matrix interface with recovery
    T = type of data in the matrix
    R = recovery
    line = false (blocs domain decomposition)
  */
  //-------------------------------------------------------------------------------
  template<class T,int R> struct DMatrix<T,R,false>
  //-------------------------------------------------------------------------------
  {
  protected:
    DMatrix_impl<T,R,false> * m;

  public:
    //! default constructor of the distributed matrix interface
    /*!
    */
    //-------------------------------------------------------------------------------
    DMatrix(){}
    //-------------------------------------------------------------------------------
    //! constructor of the distributed matrix interface
    /*!
      \param _m is a DMatrix_impl object
    */
    //-------------------------------------------------------------------------------
    DMatrix(DMatrix_impl<T,R,false> * _m){m=_m;}
    //-------------------------------------------------------------------------------
    //! constructor of the distributed matrix interface
    /*!
      \param _m is a DMatrix object
    */
    //-------------------------------------------------------------------------------
    DMatrix(DMatrix<T,R,false>& _m){m=_m.getDMatrix();}
    //-------------------------------------------------------------------------------
    //! constructor of the distributed matrix interface
    /*!
      \param h is the header to construct the object (see skelgis/util/utility.hpp)
      \param value is the default value to put in the matrix
    */
    //-------------------------------------------------------------------------------
    DMatrix(HEADER h,const T value){ m = new DMatrix_impl<T,R,false>(h,value);}
    //-------------------------------------------------------------------------------
    //! constructor of the distributed matrix interface
    /*!
      \param binFile is a binary input file to construct the DMatrix with
    */
    //-------------------------------------------------------------------------------
    DMatrix(const char * binFile){m = new DMatrix_impl<T,R,false>(binFile);}
    //-------------------------------------------------------------------------------

    //! init of the distributed matrix interface
    /*!
      \param _m is a DMatrix_impl object
    */
    //-------------------------------------------------------------------------------
    void init(DMatrix_impl<T,R,false> * _m){m=_m;}
    //-------------------------------------------------------------------------------
    //! init of the distributed matrix interface
    /*!
      \param _m is a DMatrix object
    */
    //-------------------------------------------------------------------------------
    void init(DMatrix<T,R,false>& _m){m=_m.getDMatrix();}
    //-------------------------------------------------------------------------------
    //! init of the distributed matrix interface
    /*!
      \param h is the header to construct the object (see skelgis/util/utility.hpp)
      \param value is the default value to put in the matrix
    */
    //-------------------------------------------------------------------------------
    void init(HEADER h,const T value){ m = new DMatrix_impl<T,R,false>(h,value);}
    //-------------------------------------------------------------------------------
    //! init of the distributed matrix interface
    /*!
      \param binFile is a binary input file to construct the DMatrix with
    */
    //-------------------------------------------------------------------------------
    void init(const char * binFile){m = new DMatrix_impl<T,R,false>(binFile);}
    //-------------------------------------------------------------------------------

    inline void erase(){delete m;}
  
    //! destructor of the distributed matrix
    /*!  */
    //-------------------------------------------------------------------------------
    ~DMatrix(){}
    //-------------------------------------------------------------------------------
    //! to get the value of the matrix at the iterator it (alternative to [it])
    /*!
      \param it is the iterator position
      \return the value at the iterator it
    */
    //-------------------------------------------------------------------------------
/*    inline T getValue(iterator<T,R> it){return m->getValue(it);}
    inline T getValue(iterator_cont<T,R> it){return m->getValue(it);}
    inline T getValue(iterator_step<T,R> it){return m->getValue(it);}
    inline T getValue(iterator_rev<T,R> it){return m->getValue(it);}
    inline T getValue(iterator_phb_left<T,R> it){return m->getValue(it);}
    inline T getValue(iterator_phb_right<T,R> it){return m->getValue(it);}
    inline T getValue(iterator_phb_up<T,R> it){return m->getValue(it);}
    inline T getValue(iterator_phb_down<T,R> it){return m->getValue(it);}*/
    //-------------------------------------------------------------------------------
    inline T getValue(iterator<T,R> it) const {return m->data[it._rank];}
    inline T getValue(iterator_cont<T,R> it) const {return m->data[it._rank];}
    inline T getValue(iterator_step<T,R> it) const {return m->data[it._rank];}
    inline T getValue(iterator_rev<T,R> it) const {return m->data[it._rank];}
    inline T getValue(iterator_phb_left<T,R> it) const {return m->data[it._rank];}
    inline T getValue(iterator_phb_right<T,R> it) const {return m->data[it._rank];}
    inline T getValue(iterator_phb_up<T,R> it) const {return m->data[it._rank];}
    inline T getValue(iterator_phb_down<T,R> it) const {return m->data[it._rank];}
    //-------------------------------------------------------------------------------
    //! to set the value of the matrix at the iterator it (alternative to [it]=)
    /*!
      \param it is the iterator position
    */
    //-------------------------------------------------------------------------------
    /*inline void setValue(iterator<T,R> it,T value){m->setValue(it,value);}
    inline void setValue(iterator_cont<T,R> it,T value){m->setValue(it,value);}
    inline void setValue(iterator_step<T,R> it,T value){m->setValue(it,value);}
    inline void setValue(iterator_rev<T,R> it,T value){m->setValue(it,value);}
    inline void setValue(iterator_phb_left<T,R> it,T value){m->setValue(it,value);}
    inline void setValue(iterator_phb_right<T,R> it,T value){m->setValue(it,value);}
    inline void setValue(iterator_phb_up<T,R> it,T value){m->setValue(it,value);}
    inline void setValue(iterator_phb_down<T,R> it,T value){m->setValue(it,value);}*/
    inline void setValue(iterator<T,R> it,T value){m->data[it._rank] = value;}
    inline void setValue(iterator_cont<T,R> it,T value){m->data[it._rank] = value;}
    inline void setValue(iterator_step<T,R> it,T value){m->data[it._rank] = value;}
    inline void setValue(iterator_rev<T,R> it,T value){m->data[it._rank] = value;}
    inline void setValue(iterator_phb_left<T,R> it,T value){m->data[it._rank] = value;}
    inline void setValue(iterator_phb_right<T,R> it,T value){m->data[it._rank] = value;}
    inline void setValue(iterator_phb_up<T,R> it,T value){m->data[it._rank] = value;}
    inline void setValue(iterator_phb_down<T,R> it,T value){m->data[it._rank] = value;}
    //-------------------------------------------------------------------------------
    //! Set value in the global middle
    /*!
      \param val is the value to affect
    */
    //-------------------------------------------------------------------------------
    void setGlobalMiddleValue(T val){m->setGlobalMiddleValue(val);}
    //-------------------------------------------------------------------------------
    //! to set all the values of the physical border of the matrix
    /*!
      \param val is the value to set
    */
    //-------------------------------------------------------------------------------
    inline void setPhysicalBorder(T val)
    {
      m->setPhysicalBorder(val);
    }
    //-------------------------------------------------------------------------------
    //! to set all the values on the right of physical border of the matrix
    /*!
      \param val is the value to set
    */
    //-------------------------------------------------------------------------------
    inline void setRightPhysicalBorder(T val)
    {
      m->setRightPhysicalBorder(val);
    }
    //-------------------------------------------------------------------------------
    //! to set all the values on the left of physical border of the matrix
    /*!
      \param val is the value to set
    */
    //-------------------------------------------------------------------------------
    inline void setLeftPhysicalBorder(T val)
    {
      m->setLeftPhysicalBorder(val);
    }
    //-------------------------------------------------------------------------------
    //! to set all the values on the up of physical border of the matrix
    /*!
      \param val is the value to set
    */
    //-------------------------------------------------------------------------------
    inline void setUpPhysicalBorder(T val)
    {
      m->setUpPhysicalBorder(val);
    }
    //-------------------------------------------------------------------------------
    //! to set all the values on the down of physical border of the matrix
    /*!
      \param val is the value to set
    */
    //-------------------------------------------------------------------------------
    inline void setDownPhysicalBorder(T val)
    {
      m->setDownPhysicalBorder(val);
    }
    //-------------------------------------------------------------------------------
    //! to get the beginning iterator of the DMatrix (on the first element of the DMatrix)
    /*!
      \return the iterator of the first element
    */
    //-------------------------------------------------------------------------------
    inline iterator<T,R> begin(){return m->begin();}
    inline iterator_cont<T,R> begin_cont(){return m->begin_cont();}
    inline iterator_rev<T,R> begin_rev(){return m->begin_rev();}
    inline iterator_step<T,R> begin_step(int step,int nb){return m->begin_step(step,nb);}
    inline iterator_phb_left<T,R> begin_phb_left(){return m->begin_phb_left();}
    inline iterator_phb_right<T,R> begin_phb_right(){return m->begin_phb_right();}
    inline iterator_phb_up<T,R> begin_phb_up(){return m->begin_phb_up();}
    inline iterator_phb_down<T,R> begin_phb_down(){return m->begin_phb_down();}
    inline iterator_line<T,R> begin_line(){return m->begin_line();}
    //-------------------------------------------------------------------------------
    //! to get the ending iterator of the DMatrix (on the last element of the DMatrix)
    /*!
      \return the iterator of the last element
    */
    //-------------------------------------------------------------------------------
    inline iterator<T,R> end(){return m->end();}
    inline iterator_cont<T,R> end_cont(){return m->end_cont();}
    inline iterator_rev<T,R> end_rev(){return m->end_rev();}
    inline iterator_step<T,R> end_step(){return m->end_step();}
    inline iterator_phb_left<T,R> end_phb_left(){return m->end_phb_left();}
    inline iterator_phb_right<T,R> end_phb_right(){return m->end_phb_right();}
    inline iterator_phb_up<T,R> end_phb_up(){return m->end_phb_up();}
    inline iterator_phb_down<T,R> end_phb_down(){return m->end_phb_down();}
    inline iterator_line<T,R> end_line(){return m->end_line();}
    //-------------------------------------------------------------------------------
    //! operator [] to get and set an element of the matrix at iterator it
    /*!
      \param it is the iterator position
      \return the reference to the value at it
    */
    //-------------------------------------------------------------------------------
/*    inline T& operator[](iterator<T,R> it) { return m->getValueRef(it); }
    inline T& operator[](iterator_cont<T,R> it) { return m->getValueRef(it); }
    inline T& operator[](iterator_rev<T,R> it) { return m->getValueRef(it); }
    inline T& operator[](iterator_step<T,R> it) { return m->getValueRef(it); }
    inline T& operator[](iterator_phb_left<T,R> it) { return m->getValueRef(it); }
    inline T& operator[](iterator_phb_right<T,R> it) { return m->getValueRef(it); }
    inline T& operator[](iterator_phb_up<T,R> it) { return m->getValueRef(it); }
    inline T& operator[](iterator_phb_down<T,R> it) { return m->getValueRef(it); }*/
    //-------------------------------------------------------------------------------
    inline T& operator[](iterator<T,R> it) { return m->data[it._rank]; }
    inline T& operator[](iterator_cont<T,R> it) { return m->data[it._rank]; }
    inline T& operator[](iterator_rev<T,R> it) { return m->data[it._rank]; }
    inline T& operator[](iterator_step<T,R> it) { return m->data[it._rank]; }
    inline T& operator[](iterator_phb_left<T,R> it) { return m->data[it._rank]; }
    inline T& operator[](iterator_phb_right<T,R> it) { return m->data[it._rank]; }
    inline T& operator[](iterator_phb_up<T,R> it) { return m->data[it._rank]; }
    inline T& operator[](iterator_phb_down<T,R> it) { return m->data[it._rank]; }
    //-------------------------------------------------------------------------------
    inline T operator[](iterator<T,R> it) const { return m->data[it._rank]; }
    inline T operator[](iterator_cont<T,R> it) const { return m->data[it._rank]; }
    inline T operator[](iterator_rev<T,R> it) const { return m->data[it._rank]; }
    inline T operator[](iterator_step<T,R> it) const { return m->data[it._rank]; }
    inline T operator[](iterator_phb_left<T,R> it) const { return m->data[it._rank]; }
    inline T operator[](iterator_phb_right<T,R> it) const { return m->data[it._rank]; }
    inline T operator[](iterator_phb_up<T,R> it) const { return m->data[it._rank]; }
    inline T operator[](iterator_phb_down<T,R> it) const { return m->data[it._rank]; }
    //-------------------------------------------------------------------------------
    //! operator = to assign the current DMatrix interface object to another object
    /*!
      \param right is the other DMatrix object
      \return the reference to the new current DMatrix object
    */
    //-------------------------------------------------------------------------------
    inline DMatrix<T,R,false>& operator=(DMatrix<T,R,false>& right){m = right.getDMatrix();}
    //-------------------------------------------------------------------------------

    //! Print the matrix by bloc
    /*!
     */
    //-------------------------------------------------------------------------------
    inline void print(){m->print();}
    inline void printAll(){m->printAll();}
    //-------------------------------------------------------------------------------

    //! get borders
    /*!
      Get the borders from other processors
      MPI exchanges
    */
    //-------------------------------------------------------------------------------
    inline void getBorders(){m->getBorders();}
    //-------------------------------------------------------------------------------
    //! get all neighbor right values for the element at iterator it
    /*!
      \param it is the iterator position
      \return a table of right values
    */
    //-------------------------------------------------------------------------------
    inline T * getAllRight(iterator<T,R> it){return m->getAllRight(it);}
    inline T * getAllRight(iterator_cont<T,R> it){return m->getAllRight(it);}
    inline T * getAllRight(iterator_rev<T,R> it){return m->getAllRight(it);}
    inline T * getAllRight(iterator_step<T,R> it){return m->getAllRight(it);}
    inline T * getAllRight(iterator_phb_left<T,R> it){return m->getAllRight(it);}
    //-------------------------------------------------------------------------------
    //! get all neighbor left values for the element at iterator it
    /*!
      \param it is the iterator position
      \return a table of left values
    */
    //-------------------------------------------------------------------------------
    inline T * getAllLeft(iterator<T,R> it){return m->getAllLeft(it);}
    inline T * getAllLeft(iterator_cont<T,R> it){return m->getAllLeft(it);}
    inline T * getAllLeft(iterator_rev<T,R> it){return m->getAllLeft(it);}
    inline T * getAllLeft(iterator_step<T,R> it){return m->getAllLeft(it);}
    inline T * getAllLeft(iterator_phb_right<T,R> it){return m->getAllLeft(it);}
    //-------------------------------------------------------------------------------
    //! get all neighbor up values for the element at iterator it
    /*!
      \param it is the iterator position
      \return a table of up values
    */
    //-------------------------------------------------------------------------------
    inline T * getAllUp(iterator<T,R> it){return m->getAllUp(it);}
    inline T * getAllUp(iterator_cont<T,R> it){return m->getAllUp(it);}
    inline T * getAllUp(iterator_rev<T,R> it){return m->getAllUp(it);}
    inline T * getAllUp(iterator_step<T,R> it){return m->getAllUp(it);}
    inline T * getAllUp(iterator_phb_down<T,R> it){return m->getAllUp(it);}
    //-------------------------------------------------------------------------------
    //! get all neighbor down values for the element at iterator it
    /*!
      \param it is the iterator position
      \return a table of down values
    */
    //-------------------------------------------------------------------------------
    inline T * getAllDown(iterator<T,R> it){return m->getAllDown(it);}
    inline T * getAllDown(iterator_cont<T,R> it){return m->getAllDown(it);}
    inline T * getAllDown(iterator_rev<T,R> it){return m->getAllDown(it);}
    inline T * getAllDown(iterator_step<T,R> it){return m->getAllDown(it);}
    inline T * getAllDown(iterator_phb_up<T,R> it){return m->getAllDown(it);}
    //-------------------------------------------------------------------------------
    //! get all neighbor right down values for the element at iterator it
    /*!
      \param it is the iterator position
      \return a table of right down values
    */
    //-------------------------------------------------------------------------------
    inline T * getAllRightDown(iterator<T,R> it){return m->getAllRightDown(it);}
    inline T * getAllRightDown(iterator_cont<T,R> it){return m->getAllRightDown(it);}
    inline T * getAllRightDown(iterator_rev<T,R> it){return m->getAllRightDown(it);}
    inline T * getAllRightDown(iterator_step<T,R> it){return m->getAllRightDown(it);}
    //-------------------------------------------------------------------------------
    //! get all neighbor left down values for the element at iterator it
    /*!
      \param it is the iterator position
      \return a table of left down values
    */
    //-------------------------------------------------------------------------------
    inline T * getAllLeftDown(iterator<T,R> it){return m->getAllLeftDown(it);}
    inline T * getAllLeftDown(iterator_cont<T,R> it){return m->getAllLeftDown(it);}
    inline T * getAllLeftDown(iterator_rev<T,R> it){return m->getAllLeftDown(it);}
    inline T * getAllLeftDown(iterator_step<T,R> it){return m->getAllLeftDown(it);}
    //-------------------------------------------------------------------------------
    //! get all neighbor right up values for the element at iterator it
    /*!
      \param it is the iterator position
      \return a table of right up values
    */
    //-------------------------------------------------------------------------------
    inline T * getAllRightUp(iterator<T,R> it){return m->getAllRightUp(it);}
    inline T * getAllRightUp(iterator_cont<T,R> it){return m->getAllRightUp(it);}
    inline T * getAllRightUp(iterator_rev<T,R> it){return m->getAllRightUp(it);}
    inline T * getAllRightUp(iterator_step<T,R> it){return m->getAllRightUp(it);}
    //-------------------------------------------------------------------------------
    //! get all neighbor left up values for the element at iterator it
    /*!
      \param it is the iterator position
      \return a table of left up values
    */
    //-------------------------------------------------------------------------------
    inline T * getAllLeftUp(iterator<T,R> it){return m->getAllLeftUp(it);}
    inline T * getAllLeftUp(iterator_cont<T,R> it){return m->getAllLeftUp(it);}
    inline T * getAllLeftUp(iterator_rev<T,R> it){return m->getAllLeftUp(it);}
    inline T * getAllLeftUp(iterator_step<T,R> it){return m->getAllLeftUp(it);}
    //-------------------------------------------------------------------------------
    //! get the right neighbor value for the element at iterator it
    /*!
      \param it is the iterator position
      \return the right neighbor element value
    */
    //-------------------------------------------------------------------------------
    inline T getRight(iterator<T,R> it){return m->getRight(it);}
    inline T getRight(iterator_cont<T,R> it){return m->getRight(it);}
    inline T getRight(iterator_rev<T,R> it){return m->getRight(it);}
    inline T getRight(iterator_step<T,R> it){return m->getRight(it);}
    inline T getRight(iterator_phb_left<T,R> it){return m->getRight(it);}
    //-------------------------------------------------------------------------------
    //! get the left neighbor value for the element at iterator it
    /*!
      \param it is the iterator position
      \return the left neighbor element value
    */
    //-------------------------------------------------------------------------------
    inline T getLeft(iterator<T,R> it){return m->getLeft(it);}
    inline T getLeft(iterator_cont<T,R> it){return m->getLeft(it);}
    inline T getLeft(iterator_rev<T,R> it){return m->getLeft(it);}
    inline T getLeft(iterator_step<T,R> it){return m->getLeft(it);}
    inline T getLeft(iterator_phb_right<T,R> it){return m->getLeft(it);}
    //-------------------------------------------------------------------------------
    //! get the up neighbor value for the element at iterator it
    /*!
      \param it is the iterator position
      \return the up neighbor element value
    */
    //-------------------------------------------------------------------------------
    inline T getUp(iterator<T,R> it){return m->getUp(it);}
    inline T getUp(iterator_cont<T,R> it){return m->getUp(it);}
    inline T getUp(iterator_rev<T,R> it){return m->getUp(it);}
    inline T getUp(iterator_step<T,R> it){return m->getUp(it);}
    inline T getUp(iterator_phb_down<T,R> it){return m->getUp(it);}
    //-------------------------------------------------------------------------------
    //! get the down neighbor value for the element at iterator it
    /*!
      \param it is the iterator position
      \return the down neighbor element value
    */
    //-------------------------------------------------------------------------------
    inline T getDown(iterator<T,R> it){return m->getDown(it);}
    inline T getDown(iterator_cont<T,R> it){return m->getDown(it);}
    inline T getDown(iterator_rev<T,R> it){return m->getDown(it);}
    inline T getDown(iterator_step<T,R> it){return m->getDown(it);}
    inline T getDown(iterator_phb_up<T,R> it){return m->getDown(it);}
    //-------------------------------------------------------------------------------
    //! get the right down neighbor value for the element at iterator it
    /*!
      \param it is the iterator position
      \return the right down neighbor element value
    */
    //-------------------------------------------------------------------------------
    inline T getRightDown(iterator<T,R> it){return m->getRightDown(it);}
    inline T getRightDown(iterator_cont<T,R> it){return m->getRightDown(it);}
    inline T getRightDown(iterator_rev<T,R> it){return m->getRightDown(it);}
    inline T getRightDown(iterator_step<T,R> it){return m->getRightDown(it);}
    //-------------------------------------------------------------------------------
    //! get the left down neighbor value for the element at iterator it
    /*!
      \param it is the iterator position
      \return the left down neighbor element value
    */
    //-------------------------------------------------------------------------------
    inline T getLeftDown(iterator<T,R> it){return m->getLeftDown(it);}
    inline T getLeftDown(iterator_cont<T,R> it){return m->getLeftDown(it);}
    inline T getLeftDown(iterator_rev<T,R> it){return m->getLeftDown(it);}
    inline T getLeftDown(iterator_step<T,R> it){return m->getLeftDown(it);}
    //-------------------------------------------------------------------------------
    //! get the right up neighbor value for the element at iterator it
    /*!
      \param it is the iterator position
      \return the right up neighbor element value
    */
    //-------------------------------------------------------------------------------
    inline T getRightUp(iterator<T,R> it){return m->getRightUp(it);}
    inline T getRightUp(iterator_cont<T,R> it){return m->getRightUp(it);}
    inline T getRightUp(iterator_rev<T,R> it){return m->getRightUp(it);}
    inline T getRightUp(iterator_step<T,R> it){return m->getRightUp(it);}
    //-------------------------------------------------------------------------------
    //! get the left up neighbor value for the element at iterator it
    /*!
      \param it is the iterator position
      \return the left up neighbor element value
    */
    //-------------------------------------------------------------------------------
    inline T getLeftUp(iterator<T,R> it){return m->getLeftUp(it);}
    inline T getLeftUp(iterator_cont<T,R> it){return m->getLeftUp(it);}
    inline T getLeftUp(iterator_rev<T,R> it){return m->getLeftUp(it);}
    inline T getLeftUp(iterator_step<T,R> it){return m->getLeftUp(it);}
    //-------------------------------------------------------------------------------
    //! get all values along X axe (all left and all right) for the element at iterator it
    /*!
      \param it is the iterator position
      \return a table of all neighbor values along X
    */
    //-------------------------------------------------------------------------------
    inline T * getAllX(iterator<T,R> it){return m->getAllX(it);}
    inline T * getAllX(iterator_cont<T,R> it){return m->getAllX(it);}
    inline T * getAllX(iterator_rev<T,R> it){return m->getAllX(it);}
    inline T * getAllX(iterator_step<T,R> it){return m->getAllX(it);}
    //-------------------------------------------------------------------------------
    inline void getAllX(iterator<T,R> it, T * t){return m->getAllX(it,t);}
    inline void getAllX(iterator_cont<T,R> it, T * t){return m->getAllX(it,t);}
    inline void getAllX(iterator_rev<T,R> it, T * t){return m->getAllX(it,t);}
    inline void getAllX(iterator_step<T,R> it, T * t){return m->getAllX(it,t);}
    //-------------------------------------------------------------------------------
    //! get all values along Y axe (allup and all down) for the element at iterator it
    /*!
      \param it is the iterator position
      \return a table of all neighbor values along Y
    */
    //-------------------------------------------------------------------------------
    inline T * getAllY(iterator<T,R> it){return m->getAllY(it);}
    inline T * getAllY(iterator_cont<T,R> it){return m->getAllY(it);}
    inline T * getAllY(iterator_rev<T,R> it){return m->getAllY(it);}
    inline T * getAllY(iterator_step<T,R> it){return m->getAllY(it);}
    //-------------------------------------------------------------------------------
    inline void getAllY(iterator<T,R> it, T * t){return m->getAllY(it,t);}
    inline void getAllY(iterator_cont<T,R> it, T * t){return m->getAllY(it,t);}
    inline void getAllY(iterator_rev<T,R> it, T * t){return m->getAllY(it,t);}
    inline void getAllY(iterator_step<T,R> it, T * t){return m->getAllY(it,t);}
    //-------------------------------------------------------------------------------
    //! get all 8 directions neighbor values for the element at iterator it
    /*!
      \param it is the iterator position
      \return a table of all 8 directions neighbor values
    */
    //-------------------------------------------------------------------------------
    inline T * get8(iterator<T,R> it){return m->get8(it);}
    inline T * get8(iterator_cont<T,R> it){return m->get8(it);}
    inline T * get8(iterator_rev<T,R> it){return m->get8(it);}
    inline T * get8(iterator_step<T,R> it){return m->get8(it);}
    //-------------------------------------------------------------------------------
    //! get all 4 directions neighbor values for the element at iterator it
    /*!
      \param it is the iterator position
      \return a table of all 4 directions neighbor values
    */
    //-------------------------------------------------------------------------------
    inline T * get4(iterator<T,R> it){return m->get4(it);};
    inline T * get4(iterator_cont<T,R> it){return m->get4(it);}
    inline T * get4(iterator_rev<T,R> it){return m->get4(it);}
    inline T * get4(iterator_step<T,R> it){return m->get4(it);}
    //-------------------------------------------------------------------------------
    //! get the first neighbor value inside the domain for physical border iterators whatever is the recovery size
    /*!
      \param it is the physical border iterator position
      \return the neighbor value
    */
    //-------------------------------------------------------------------------------
    inline T getInRight(iterator_phb_left<T,R> it){return m->getInRight(it);}
    inline T getInLeft(iterator_phb_right<T,R> it){return m->getInLeft(it);}
    inline T getInUp(iterator_phb_down<T,R> it){return m->getInUp(it);}
    inline T getInDown(iterator_phb_up<T,R> it){return m->getInDown(it);}
    //-------------------------------------------------------------------------------
    //! get the iterator on the matrix at position (x,y)
    /*!
      \param x is row index
      \pram y is column index
      \return returns the iterator at position (x,y)
    */
    //-------------------------------------------------------------------------------
    iterator<T,R> getIterator(int x,int y){return m->getIterator(x,y);}
    iterator<T,R> getIterator_cont(int x,int y){return m->getIterator_cont(x,y);}
    iterator<T,R> getIterator_rev(int x,int y){return m->getIterator_rev(x,y);}
    //-------------------------------------------------------------------------------
    //! get the indexes of the iterator on the matrix
    /*!
      \param it is the iterator from which the indexes will be taken
      \param x is the iterator position x returned
      \param y is the iterator position y returned
    */
    //-------------------------------------------------------------------------------
    void getIndexes(iterator<T,R> it,int &col,int &li){m->getIndexes(it,col,li);}
    void getIndexes(iterator_cont<T,R> it,int &col,int &li){m->getIndexes(it,col,li);}
    void getIndexes(iterator_rev<T,R> it,int &col,int &li){m->getIndexes(it,col,li);}
    //-------------------------------------------------------------------------------
    //! get the global header of the DMatrix
    /*!
      \return the global header
    */
    //-------------------------------------------------------------------------------
    inline HEADER getGlobalHeader(){return m->getGlobalHeader();}
    //-------------------------------------------------------------------------------
    //! get the header of the DMatrix
    /*!
      \return the header
    */
    //-------------------------------------------------------------------------------
    inline HEADER getHeader(){return m->getHeader();}
    //-------------------------------------------------------------------------------
    //! get the border header of the DMatrix
    /*!
      \return the border header
    */
    //-------------------------------------------------------------------------------
    inline HEADER getBorderHeader(){return m->getBorderHeader();}
    //-------------------------------------------------------------------------------
    //! get the number of columns in the domain decomposition
    /*!
      \return the number of columns in the domain decomposition
    */
    //-------------------------------------------------------------------------------
    inline int getCols(){return m->getCols();}
    //-------------------------------------------------------------------------------
    //! get the number of rows in the domain decomposition
    /*!
      \return the number of rows in the domain decomposition
    */
    //-------------------------------------------------------------------------------
    inline int getRows(){return m->getRows();}
    //-------------------------------------------------------------------------------
    //! get the column number of the DMatrix
    /*!
      \return the column number of the DMatrix
    */
    //-------------------------------------------------------------------------------
    inline int getCol(){return m->getCol();}
    //-------------------------------------------------------------------------------
    //! get the row number of the DMatrix
    /*!
      \return the row number of the DMatrix
    */
    //-------------------------------------------------------------------------------
    inline int getRow(){return m->getRow();}
    //-------------------------------------------------------------------------------
    //! get the data table of the DMatrix
    /*!
      \return the data table of the DMatrix
    */
    //-------------------------------------------------------------------------------
    inline T * getData(){return m->getData();}
    inline unsigned int getWidth(){return m->getWidth();}
    inline unsigned int getWidthLoc(){return m->getWidthLoc();}
    inline unsigned int getHeightLoc(){return m->getHeightLoc();}
    //-------------------------------------------------------------------------------
    //! get the recovery of the DMatrix (=R)
    /*!
      \return the recovery of the DMatrix
    */
    //-------------------------------------------------------------------------------
    inline int getBorder(){return m->getBorder();}
    //-------------------------------------------------------------------------------
    //! get the DMatrix_impl associated to the DMatrix interface
    /*!
      \return the DMatrix_impl
    */
    //-------------------------------------------------------------------------------
    inline DMatrix_impl<T,R,false> * getDMatrix(){return m;}
    //-------------------------------------------------------------------------------
  };

  //================================================================================
  //!  template Distributed Matrix interface Class without recovery
  /*!
    This template class defines the distributed matrix interface without recovery
    T = type of data in the matrix
    R = 0
    line = false (bloc domain decomposition)
  */
  //-------------------------------------------------------------------------------
  template<class T> struct DMatrix<T,0,false>
  //-------------------------------------------------------------------------------
  {
  protected:
    //!pointer to DMatrix_impl object
    DMatrix_impl<T,0,false> * m;

  public:
    //! default constructor of the distributed matrix interface
    /*!
    */
    //-------------------------------------------------------------------------------
    DMatrix(){}
    //-------------------------------------------------------------------------------
    //! constructor of the distributed matrix interface
    /*!
      \param _m is a DMatrix_impl object
    */
    //-------------------------------------------------------------------------------
    DMatrix(DMatrix_impl<T,0,false> * _m){m=_m;}
    //-------------------------------------------------------------------------------
    //! constructor of the distributed matrix interface
    /*!
      \param _m is a DMatrix object
    */
    //-------------------------------------------------------------------------------
    DMatrix(DMatrix<T,0,false>& _m){m=_m.getDMatrix();}
    //-------------------------------------------------------------------------------
    //! constructor of the distributed matrix interface
    /*!
      \param h is the header to construct the object (see skelgis/util/utility.hpp)
      \param value is the default value to put in the matrix
    */
    //-------------------------------------------------------------------------------
    DMatrix(HEADER h,const T value){m = new DMatrix_impl<T,0,false>(h,value);}
    //-------------------------------------------------------------------------------
    //! constructor of the distributed matrix interface
    /*!
      \param binFile is a binary input file to construct the DMatrix with
    */
    //-------------------------------------------------------------------------------
    DMatrix(const char * binFile){m = new DMatrix_impl<T,0,false>(binFile);}
    //-------------------------------------------------------------------------------

    //! init of the distributed matrix interface
    /*!
      \param _m is a DMatrix_impl object
    */
    //-------------------------------------------------------------------------------
    void init(DMatrix_impl<T,0,false> * _m){m=_m;}
    //-------------------------------------------------------------------------------
    //! init of the distributed matrix interface
    /*!
      \param _m is a DMatrix object
    */
    //-------------------------------------------------------------------------------
    void init(DMatrix<T,0,false>& _m){m=_m.getDMatrix();}
    //-------------------------------------------------------------------------------
    //! init of the distributed matrix interface
    /*!
      \param h is the header to construct the object (see skelgis/util/utility.hpp)
      \param value is the default value to put in the matrix
    */
    //-------------------------------------------------------------------------------
    void init(HEADER h,const T value){m = new DMatrix_impl<T,0,false>(h,value);}
    //-------------------------------------------------------------------------------
    //! init of the distributed matrix interface
    /*!
      \param binFile is a binary input file to construct the DMatrix with
    */
    //-------------------------------------------------------------------------------
    void init(const char * binFile){m = new DMatrix_impl<T,0,false>(binFile);}
    //-------------------------------------------------------------------------------

    inline void erase(){delete m;} 
 
    //! destructor of the distributed matrix
    /*!  */
    //-------------------------------------------------------------------------------
    ~DMatrix(){}
    //-------------------------------------------------------------------------------

    //! to get the value of the matrix at the iterator it (alternative to [it])
    /*!
      \param it is the iterator position
      \return the value at the iterator it
    */
    //-------------------------------------------------------------------------------
/*    inline T getValue(iterator<T,0> it){return m->getValue(it);}
    inline T getValue(iterator_cont<T,0> it){return m->getValue(it);}
    inline T getValue(iterator_step<T,0> it){return m->getValue(it);}
    inline T getValue(iterator_rev<T,0> it){return m->getValue(it);}*/
    //-------------------------------------------------------------------------------
    inline T getValue(iterator<T,0> it) const {return m->data[it._rank];}
    inline T getValue(iterator_cont<T,0> it) const {return m->data[it._rank];}
    inline T getValue(iterator_step<T,0> it) const {return m->data[it._rank];}
    inline T getValue(iterator_rev<T,0> it) const {return m->data[it._rank];}
    //-------------------------------------------------------------------------------
    //! to set the value of the matrix at the iterator it (alternative to [it]=)
    /*!
      \param it is the iterator position
    */
    //-------------------------------------------------------------------------------
    /*inline void setValue(iterator<T,0> it,T value){m->setValue(it,value);}
    inline void setValue(iterator_cont<T,0> it,T value){m->setValue(it,value);}
    inline void setValue(iterator_step<T,0> it,T value){m->setValue(it,value);}
    inline void setValue(iterator_rev<T,0> it,T value){m->setValue(it,value);}*/
    inline void setValue(iterator<T,0> it,T value){m->data[it._rank] = value;}
    inline void setValue(iterator_cont<T,0> it,T value){m->data[it._rank] = value;}
    inline void setValue(iterator_step<T,0> it,T value){m->data[it._rank] = value;}
    inline void setValue(iterator_rev<T,0> it,T value){m->data[it._rank] = value;}
    //-------------------------------------------------------------------------------
    //! to get the beginning iterator of the DMatrix (on the first element of the DMatrix)
    /*!
      \return the iterator of the first element
    */
    //-------------------------------------------------------------------------------
    inline iterator<T,0> begin(){return m->begin();}
    inline iterator_cont<T,0> begin_cont(){return m->begin_cont();}
    inline iterator_rev<T,0> begin_rev(){return m->begin_rev();}
    inline iterator_step<T,0> begin_step(int step,int nb){return m->begin_step(step,nb);}
    inline iterator_line<T,0> begin_line(){return m->begin_line();}
    //-------------------------------------------------------------------------------
    //! to get the ending iterator of the DMatrix (on the last element of the DMatrix)
    /*!
      \return the iterator of the last element
    */
    //-------------------------------------------------------------------------------
    inline iterator<T,0> end(){return m->end();}
    inline iterator_cont<T,0> end_cont(){return m->end_cont();}
    inline iterator_rev<T,0> end_rev(){return m->end_rev();}
    inline iterator_step<T,0> end_step(){return m->end_step();}
    inline iterator_line<T,0> end_line(){return m->end_line();}
    //-------------------------------------------------------------------------------
    //! operator [] to get and set an element of the matrix at iterator it
    /*!
      \param it is the iterator position
      \return the reference to the value at it
    */
    //-------------------------------------------------------------------------------
/*    inline T& operator[](iterator<T,0> it) { return m->getValueRef(it); }
    inline T& operator[](iterator_cont<T,0> it) { return m->getValueRef(it); }
    inline T& operator[](iterator_rev<T,0> it) { return m->getValueRef(it); }
    inline T& operator[](iterator_step<T,0> it) { return m->getValueRef(it); }*/
    //-------------------------------------------------------------------------------
    inline T& operator[](iterator<T,0> it) { return m->data[it._rank]; }
    inline T& operator[](iterator_cont<T,0> it) { return m->data[it._rank]; }
    inline T& operator[](iterator_rev<T,0> it) { return m->data[it._rank]; }
    inline T& operator[](iterator_step<T,0> it) { return m->data[it._rank]; }
    //-------------------------------------------------------------------------------
    inline T operator[](iterator<T,0> it) const { return m->data[it._rank]; }
    inline T operator[](iterator_cont<T,0> it) const { return m->data[it._rank]; }
    inline T operator[](iterator_rev<T,0> it) const { return m->data[it._rank]; }
    inline T operator[](iterator_step<T,0> it) const { return m->data[it._rank]; }
    //-------------------------------------------------------------------------------

    //! operator = to assign the current DMatrix interface object to another object
    /*!
      \param right is the other DMatrix object
      \return the reference to the new current DMatrix object
    */
    //-------------------------------------------------------------------------------
    inline DMatrix<T,0,false>& operator=(DMatrix<T,0,false>& right){m = right.getDMatrix();}
    //-------------------------------------------------------------------------------

    //! Print the matrix by bloc
    /*!
     */
    //-------------------------------------------------------------------------------
    inline void print(){m->print();}
    inline void printAll(){m->printAll();}
    //-------------------------------------------------------------------------------
    //! get the iterator on the matrix at position (x,y)
    /*!
      \param x is row index
      \pram y is column index
      \return returns the iterator at position (x,y)
    */
    //-------------------------------------------------------------------------------
    iterator<T,0> getIterator(int x,int y){return m->getIterator(x,y);}
    iterator<T,0> getIterator_cont(int x,int y){return m->getIterator_cont(x,y);}
    iterator<T,0> getIterator_rev(int x,int y){return m->getIterator_rev(x,y);}
    //-------------------------------------------------------------------------------
    //! get the indexes of the iterator on the matrix
    /*!
      \param it is the iterator from which the indexes will be taken
      \param x is the iterator position x returned
      \param y is the iterator position y returned
    */
    //-------------------------------------------------------------------------------
    void getIndexes(iterator<T,0> it,int &col,int &li){m->getIndexes(it,col,li);}
    void getIndexes(iterator_cont<T,0> it,int &col,int &li){m->getIndexes(it,col,li);}
    void getIndexes(iterator_rev<T,0> it,int &col,int &li){m->getIndexes(it,col,li);}
    //-------------------------------------------------------------------------------
    //! get the global header of the DMatrix
    /*!
      \return the global header
    */
    //-------------------------------------------------------------------------------
    inline HEADER getGlobalHeader(){return m->getGlobalHeader();}
    //-------------------------------------------------------------------------------
    //! get the header of the DMatrix
    /*!
      \return the header
    */
    //-------------------------------------------------------------------------------
    inline HEADER getHeader(){return m->getHeader();}
    //-------------------------------------------------------------------------------
    //! get the number of columns in the domain decomposition
    /*!
      \return the number of columns in the domain decomposition
    */
    //-------------------------------------------------------------------------------
    inline int getCols(){return m->getCols();}
    //-------------------------------------------------------------------------------
    //! get the number of rows in the domain decomposition
    /*!
      \return the number of rows in the domain decomposition
    */
    //-------------------------------------------------------------------------------
    inline int getRows(){return m->getRows();}
    //-------------------------------------------------------------------------------
    //! get the column number of the DMatrix
    /*!
      \return the column number of the DMatrix
    */
    //-------------------------------------------------------------------------------
    inline int getCol(){return m->getCol();}
    //-------------------------------------------------------------------------------
    //! get the row number of the DMatrix
    /*!
      \return the row number of the DMatrix
    */
    //-------------------------------------------------------------------------------
    inline int getRow(){return m->getRow();}
    //-------------------------------------------------------------------------------
    //! get the data table of the DMatrix
    /*!
      \return the data table of the DMatrix
    */
    //-------------------------------------------------------------------------------
    inline T * getData(){return m->getData();}
    inline unsigned int getWidth(){return m->getWidth();}
    inline unsigned int getWidthLoc(){return m->getWidthLoc();}
    inline unsigned int getHeightLoc(){return m->getHeightLoc();}
    //-------------------------------------------------------------------------------
    //! get the DMatrix_impl associated to the DMatrix interface
    /*!
      \return the DMatrix_impl
    */
    //-------------------------------------------------------------------------------
    inline DMatrix_impl<T,0,false> * getDMatrix(){return m;}
    //-------------------------------------------------------------------------------
  };
  
}

#endif
