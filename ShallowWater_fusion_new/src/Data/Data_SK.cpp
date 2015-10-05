#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>
#include <llcmcpp/llcmmpi.h>
#include <mpi.h>
#include <math.h>

#include <datah.h>
#include <structh.h>
#include <inith.h>
//#include <iterator.hpp>
//#include <communications.hpp>
#include <sstream>

#include "libskelgis/skelgis.hpp"

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;
using namespace skelgis;

class DataSK:virtual public Go,virtual public Datah
{

public:
  skelgis::DMatrix_impl<double,2,false> * dmatrix;

  //components variables 
  MPI_Comm comm;//! MPI communicator
  Structh* str;//! structure
  Inith* initializer; //! initializer
  //variables
  int64_t width,height; //! width and height of the data array
  double * data; //! data array
  int16_t border; //! size of the physical border needed (limit conditions), linked to the stencil shape also
  int16_t domain; //! 1=cell 2=edgex 3=edgey
  double init_val;

  //-------------------------------------------------------------------------------                                                                                                                      
  //! default constructor                                                                                                                                            
  /*! */
  //-------------------------------------------------------------------------------
  DataSK ()
  //-------------------------------------------------------------------------------
  {
  }
  //-------------------------------------------------------------------------------                                                                                                                      
  //! default destructor                                                                                                                                                                               
  /*! */
  //-------------------------------------------------------------------------------
  ~DataSK()
  //-------------------------------------------------------------------------------
  {
    delete dmatrix;
  }
  //-------------------------------------------------------------------------------                                                                                                                      
  //! go interface
  /*! */
  //-------------------------------------------------------------------------------
  void go ()
  //-------------------------------------------------------------------------------
  {
    if(str && initializer)
      {
        str->getSizes(width,height); //get local size
        if(domain==1)
        {
          width = width;//+2*border; //add the border
          height = height;//+2*border; //add the border
        }
        else if(domain==2)
        {
          width = width+1;//2*border+1; //add the border
          height = height;//+2*border; //add the border
        }
        else if(domain==3)
        {
          width = width;//+2*border; //add the border
          height = height+1;//2*border+1; //add the border
        }
      }

    HEADER head;
    head.width = width;
    head.height = height;
    head.x = 0;
    head.y = 0;
    head.spacing = 1;
    head.nodata = -9999;
    dmatrix = new skelgis::DMatrix_impl<double,2,false>(head,init_val,comm);
  }
  //-------------------------------------------------------------------------------
  //! update interface
  /*! */
  //-------------------------------------------------------------------------------
  inline void update (int tagdata){dmatrix->getBorders();}
  //-------------------------------------------------------------------------------                                                                                                                      
  //! Interface get the border size
  /*!  */
  //------------------------------------------------------------------------------- 
  inline int16_t getBorder(){return border;}
  //-------------------------------------------------------------------------------
  //! Set value in the global middle
  /*!  */
  //-------------------------------------------------------------------------------
  inline void setMiddle(double val){dmatrix->setGlobalMiddleValue(val);}
  //-------------------------------------------------------------------------------
  //! Return the pointer on the data
  /*!  */
  //-------------------------------------------------------------------------------
  inline double * getData(){return dmatrix->getData();}
  //-------------------------------------------------------------------------------
  //! Return width
  /*!  */
  //-------------------------------------------------------------------------------
  inline int64_t getWidth(){return dmatrix->getWidth();}
  //-------------------------------------------------------------------------------
  //! Return width local
  /*!  */
  //-------------------------------------------------------------------------------
  inline int64_t getWidthLoc(){return dmatrix->getWidthLoc();}
  //-------------------------------------------------------------------------------
  //! Return height local
  /*!  */
  //-------------------------------------------------------------------------------
  inline int64_t getHeightLoc(){return dmatrix->getHeightLoc();}
  //-------------------------------------------------------------------------------

  inline int64_t getStartWPHBL(){return dmatrix->getStartWPHBL();}
  inline int64_t getWidthPHBL(){return dmatrix->getWidthPHBL();}
  inline int64_t getStartHPHBL(){return dmatrix->getStartHPHBL();}
  inline int64_t getHeightPHBL(){return dmatrix->getHeightPHBL();}

  inline int64_t getStartWPHBR(){return dmatrix->getStartWPHBR();}
  inline int64_t getWidthPHBR(){return dmatrix->getWidthPHBR();}
  inline int64_t getStartHPHBR(){return dmatrix->getStartHPHBR();}
  inline int64_t getHeightPHBR(){return dmatrix->getHeightPHBR();}

  inline int64_t getStartWPHBU(){return dmatrix->getStartWPHBU();}
  inline int64_t getWidthPHBU(){return dmatrix->getWidthPHBU();}
  inline int64_t getStartHPHBU(){return dmatrix->getStartHPHBU();}
  inline int64_t getHeightPHBU(){return dmatrix->getHeightPHBU();}

  inline int64_t getStartWPHBD(){return dmatrix->getStartWPHBD();}
  inline int64_t getWidthPHBD(){return dmatrix->getWidthPHBD();}
  inline int64_t getStartHPHBD(){return dmatrix->getStartHPHBD();}
  inline int64_t getHeightPHBD(){return dmatrix->getHeightPHBD();}

};

#include <Data/Data_SK.l2c>
