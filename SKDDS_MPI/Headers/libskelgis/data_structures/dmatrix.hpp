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
  //!  template Distributed Matrix interface Class with recovery
  /*!
    This template class defines the distributed matrix interface with recovery
    T = type of data in the matrix
    R = recovery
    line = false (blocs domain decomposition)
  */
  //-------------------------------------------------------------------------------
  template<class T,int R,bool line=false> struct DMatrix
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
