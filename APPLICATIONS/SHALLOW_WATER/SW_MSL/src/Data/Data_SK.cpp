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

    HEADER head;
    head.width = 0;
    head.height = 0;
    head.x = 0;
    head.y = 0;
    head.spacing = 1;
    head.nodata = -9999;

    dmatrix = new DMatrix_impl<double,2,false>(head,init_val,comm);

    if(str && initializer)
      {
        str->getSizes(width,height); //get local size                                                                                                                                                                                        
        /*if(domain==1)                                                                                                                                                                                                                      
          {                                                                                                                                                                                                                                  
            width = width;//+2*border; //add the border                                                                                                                                                                                      
            height = height;//+2*border; //add the border                                                                                                                                                                                    
            }*/
        if(domain==2)
          {
            width = width+dmatrix->getCols();//2*border+1; //add the border                                                                                                                                                                  
            //height = height;//+2*border; //add the border                                                                                                                                                                                  
          }
        else if(domain==3)
          {
            //width = width;//+2*border; //add the border                                                                                                                                                                                    
            height = height+dmatrix->getRows();//2*border+1; //add the border                                                                                                                                                                
          }
      }

    delete dmatrix;

    head.width = width;
    head.height = height;
    head.x = 0;
    head.y = 0;
    head.spacing = 1;
    head.nodata = -9999;
    dmatrix = new DMatrix_impl<double,2,false>(head,init_val,comm);

  }
  //-------------------------------------------------------------------------------
  //! update interface
  /*! */
  //-------------------------------------------------------------------------------
  inline void update (){dmatrix->getBorders();}
  //-------------------------------------------------------------------------------                                                                                                                      
  //! Interface get the border size
  /*!  */
  //------------------------------------------------------------------------------- 
  inline void getBorder(int16_t& b){b=border;}
  //-------------------------------------------------------------------------------
  //! Set value in the global middle
  /*!  */
  //-------------------------------------------------------------------------------
  inline void setMiddle(double val){dmatrix->setGlobalMiddleValue(val);}
  //-------------------------------------------------------------------------------
  //! Return the pointer on the data
  /*!  */
  //-------------------------------------------------------------------------------
  //inline double * getData(){return dmatrix->getData();}
  inline void getData(double * & data){data = dmatrix->getData();}
  inline void getDMatrix(skelgis::DMatrix_impl<double,2,false> * & dm){dm = dmatrix;}
  //-------------------------------------------------------------------------------
  //! Return width
  /*!  */
  //-------------------------------------------------------------------------------
  inline void getWidth(int64_t& w){w=dmatrix->getWidth();}
  //-------------------------------------------------------------------------------
  //! Return width local
  /*!  */
  //-------------------------------------------------------------------------------
  inline void getWidthLoc(int64_t& wl){wl=dmatrix->getWidthLoc();}
  //-------------------------------------------------------------------------------
  //! Return height local
  /*!  */
  //-------------------------------------------------------------------------------
  inline void getHeightLoc(int64_t& hl){hl=dmatrix->getHeightLoc();}
  //-------------------------------------------------------------------------------

  inline void getStartWPHBL(int64_t& v){v=dmatrix->getStartWPHBL();}
  inline void getWidthPHBL(int64_t& v){v=dmatrix->getWidthPHBL();}
  inline void getStartHPHBL(int64_t& v){v=dmatrix->getStartHPHBL();}
  inline void getHeightPHBL(int64_t& v){v=dmatrix->getHeightPHBL();}

  inline void getStartWPHBR(int64_t& v){v=dmatrix->getStartWPHBR();}
  inline void getWidthPHBR(int64_t& v){v=dmatrix->getWidthPHBR();}
  inline void getStartHPHBR(int64_t& v){v=dmatrix->getStartHPHBR();}
  inline void getHeightPHBR(int64_t& v){v=dmatrix->getHeightPHBR();}

  inline void getStartWPHBU(int64_t& v){v=dmatrix->getStartWPHBU();}
  inline void getWidthPHBU(int64_t& v){v=dmatrix->getWidthPHBU();}
  inline void getStartHPHBU(int64_t& v){v=dmatrix->getStartHPHBU();}
  inline void getHeightPHBU(int64_t& v){v=dmatrix->getHeightPHBU();}

  inline void getStartWPHBD(int64_t& v){v=dmatrix->getStartWPHBD();}
  inline void getWidthPHBD(int64_t& v){v=dmatrix->getWidthPHBD();}
  inline void getStartHPHBD(int64_t& v){v=dmatrix->getStartHPHBD();}
  inline void getHeightPHBD(int64_t& v){v=dmatrix->getHeightPHBD();}

};

#include <Data/Data_SK.l2c>
