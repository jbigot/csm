#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>
#include <llcmcpp/llcmmpi.h>
#include <mpi.h>
#include <math.h>

#include <datah.h>
#include <structh.h>
#include <inith.h>
#include <iterator.hpp>
#include <communications.hpp>

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

class Data:virtual public Go,virtual public Datah
{

public:
  //components variables
  MPI_Comm comm;//! MPI communicator
  Structh* str;//! structure
  Inith* initializer; //! initializer
  
  //variables
  int64_t width,height; //! width and height of the data array
  double * data; //! data array
  int16_t border;

  int32_t* neigh;
  int32_t tile_y;
  int32_t nb_tile_y;
  int32_t tile_x;
  int32_t nb_tile_x;

  //-------------------------------------------------------------------------------                                                                                                                      
  //! default constructor                                                                                                                                            
  /*! */
  //-------------------------------------------------------------------------------
  Data ():data(0),width(0),height(0),str(0),initializer(0)
  //-------------------------------------------------------------------------------
  {}
  //-------------------------------------------------------------------------------                                                                                                                      
  //! default destructor                                                                                                                                                                               
  /*! */
  //-------------------------------------------------------------------------------
  ~Data()
  //-------------------------------------------------------------------------------
  {
    //cout<<"[Data] destruction"<<endl;
    delete [] data;
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
	str->getBorder(border);
	str->getSizes(width,height);
	data = new double[width*height];
	initializer->initData(data,width*height);
      
	neigh=str->getNeighbors();
	tile_y = str->getTileY();
	nb_tile_y = str->getNbTileY();
	tile_x = str->getTileX();
	nb_tile_x = str->getNbTileX();

	//cout<<"size of data "<<width*height<<endl;
      }
  }
  //-------------------------------------------------------------------------------
  //! update interface
  /*! */
  //-------------------------------------------------------------------------------
  void update ()
  //-------------------------------------------------------------------------------
  {
    if(tile_y!=0)
      {
	double * toSend = getUpBorderToSend();
	double * toGet = new double[(width-2*border)*border];
	Communications<double>::Exchanges(toSend,toGet,(width-2*border)*border,neigh[0],comm);
	setUpBorder(toGet);
	delete [] toSend;
	delete [] toGet;
      }
    if(tile_y!=(nb_tile_y-1))
      {          
	double * toSend = getDownBorderToSend();
	double * toGet = new double[(width-2*border)*border];
	Communications<double>::Exchanges(toSend,toGet,(width-2*border)*border,neigh[2],comm);
	setDownBorder(toGet);
	delete [] toSend;
	delete [] toGet;
      }
    if(tile_x!=(nb_tile_x-1))
      {  
	double * toSend = getRightBorderToSend();
	double * toGet = new double[(height-2*border)*border];
	Communications<double>::Exchanges(toSend,toGet,(height-2*border)*border,neigh[1],comm);
	setRightBorder(toGet);
	delete [] toSend;
	delete [] toGet;
      }
    if(tile_x!=0)
      {
	double * toSend = getLeftBorderToSend();
	double * toGet = new double[(height-2*border)*border];
	Communications<double>::Exchanges(toSend,toGet,(height-2*border)*border,neigh[3],comm);
	setLeftBorder(toGet);
	delete [] toSend;
	delete [] toGet;
      }
  }
  //-------------------------------------------------------------------------------
  //! begin ietrator
  /*! */
  //-------------------------------------------------------------------------------
  inline iterator begin ()
  //-------------------------------------------------------------------------------
  {
    return iterator(border*width+border,width,border);
  }
  //-------------------------------------------------------------------------------                                                                                                                      
  //! end iterator                                                                                                                                                                  
  /*! */
  //-------------------------------------------------------------------------------
  inline iterator end ()
  //-------------------------------------------------------------------------------
  {
    return iterator(width*height-1-border*width-border,width,border);
  }
  //-------------------------------------------------------------------------------
  //! get data value
  /*! */
  //-------------------------------------------------------------------------------
  inline double get(iterator it)
  //-------------------------------------------------------------------------------
  {
    return data[it._rank];
  }
  //-------------------------------------------------------------------------------
  //! set data value
  /*! */
  //-------------------------------------------------------------------------------
  inline void set(iterator it,double val)
  //-------------------------------------------------------------------------------
  {
    data[it._rank]=val;
  }
  //-------------------------------------------------------------------------------
  //! get the value up
  /*! */
  //-------------------------------------------------------------------------------
  inline double getUp(iterator it)
  //-------------------------------------------------------------------------------
  {
    return data[it._rank-width];
  }
  //-------------------------------------------------------------------------------
  //! get the value right
  /*! */
  //-------------------------------------------------------------------------------
  inline double getRight(iterator it)
  //-------------------------------------------------------------------------------
  {
    return data[it._rank+1];
  }
  //-------------------------------------------------------------------------------
  //! get the value down
  /*! */
  //-------------------------------------------------------------------------------
  inline double getDown(iterator it)
  //-------------------------------------------------------------------------------
  {
    return data[it._rank+width];
  }
  //-------------------------------------------------------------------------------
  //! get the value left
  /*! */
  //-------------------------------------------------------------------------------
  inline double getLeft(iterator it)
  //-------------------------------------------------------------------------------
  {
    return data[it._rank-1];
  }
  //-------------------------------------------------------------------------------
  //! Set value in the global middle
  /*!  */
  //-------------------------------------------------------------------------------
  void setMiddle(double val)
  //-------------------------------------------------------------------------------
  {
    int32_t tile_y = str->getTileY();
    int32_t nb_tile_y = str->getNbTileY();
    int64_t globW = str->getGlobW();
    int64_t globH = str->getGlobH();
    if(tile_y!=nb_tile_y-1 || nb_tile_y==1)
      {
	

	int64_t middle_h = floor(globH/2)-1;
	if((height-2*border)*tile_y<=middle_h && (height-2*border)*(tile_y+1)>middle_h)
	  {
	    int64_t j_loc = floor(globW/2)-1;
	    int64_t i_loc = middle_h-tile_y*(height-2*border);
	    int64_t rank = border*width +  i_loc*width + j_loc + border;
	    data[rank] = val;
	  }
      }
  }
  inline double * getData()
  {
    return data;
  }
  inline int64_t getWidth()
  {
    return width;
  }
  inline int64_t getWidthLoc()
  {
    return width-2;
  }
  inline int64_t getHeightLoc()
  {
    return height-2;
  }


private:
  //-------------------------------------------------------------------------------
  //! get the right border to send to neighbor processors
  /*!  */
  //-------------------------------------------------------------------------------
  double* getRightBorderToSend()
  //-------------------------------------------------------------------------------
  {
    double* result = new double[(height-2*border)*border];
    for(int64_t i=0;i<(height-2*border);i++)
      {
	for(int16_t j=0;j<border;j++)
	  {
	    int64_t r1 = i*border+j;
	    int64_t r2 = ((border+1)*width - border*2) + i*width +j;
	    result[r1] = data[r2];
	  }
      }	  
    return result;
  }
  //-------------------------------------------------------------------------------
  //! set the right border ghost cells with neighbor values from processors
  /*!  */
  //-------------------------------------------------------------------------------
  void setRightBorder(double * values)
  //-------------------------------------------------------------------------------
  {
    for(int64_t i=0;i<(height-2*border);i++)
      {
	for(int16_t j=0;j<border;j++)
	  data[(width*border + width - border) + i*width + j] = values[i*border+j];
      }
  }
  //-------------------------------------------------------------------------------
  //! get the left border to send to neighbor processors
  /*!  */
  //-------------------------------------------------------------------------------
  double * getLeftBorderToSend()
  //-------------------------------------------------------------------------------
  {
    double * result = new double[(height-2*border)*border];
    for(int64_t i=0;i<(height-2*border);i++)
      {
	for(int16_t j=0;j<border;j++)
	  {
	    int64_t r1 = i*border+j;
	    int64_t r2 = (border*width + border) + i*width + j;
	    result[r1] = data[r2];
	  }
      }
    return result;
  }
  //------------------------------------------------------------------------------- 
  //! set the left border ghost cells with neighbor values from processors
  /*!  */
  //-------------------------------------------------------------------------------
  void setLeftBorder(double * values)
  //-------------------------------------------------------------------------------
  {
    for(int64_t i=0;i<(height-2*border);i++)
      {
	for(int16_t j=0;j<border;j++)
	  data[(width*border) + i*width + j] = values[i*border+j];
      }
  }
  //-------------------------------------------------------------------------------
  //! get the up border to send to neighbor processors
  /*!
      \return list of values
  */
  //-------------------------------------------------------------------------------
  double * getUpBorderToSend()
  //-------------------------------------------------------------------------------
  {
    double * result = new double[(width-2*border)*border];
    for(int16_t i=0;i<border;i++)
      {
	for(int64_t j=0;j<(width-2*border);j++)
	  {
	    int64_t r1 = i*(width-2*border)+j;
	    int64_t r2 = (border*width + border) + i*width+j;
	    result[r1] = data[r2];
	  }
      }
    return result;
  }
  //-------------------------------------------------------------------------------
  //! set the up border ghost cells with neighbor values from processors
  /*!  */
  //-------------------------------------------------------------------------------
  void setUpBorder(double * values)
  //-------------------------------------------------------------------------------
  {
    for(int i=0;i<border;i++)
      {
	for(int64_t j=0;j<(width-2*border);j++)
	      data[border + i*width+j] = values[i*(width-2*border)+j];
      }
  }
  //-------------------------------------------------------------------------------
  //! get the down border to send to neighbor processors
  /*!
      \return list of values
  */
  //-------------------------------------------------------------------------------
  double * getDownBorderToSend()
  //-------------------------------------------------------------------------------
  {
    double* result = new double[(width-2*border)*border];
    for(int16_t i=0;i<border;i++)
      {
	for(int64_t j=0;j<(width-2*border);j++)
	  {
	    int64_t r1 = i*(width-2*border)+j;
	    int64_t r2 = (height*width - (border+border)*width + border) + i*width + j;
	    result[r1] = data[r2];
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
  void setDownBorder(double * values)
  //-------------------------------------------------------------------------------
  {
    for(int16_t i=0;i<border;i++)
      {
	for(int64_t j=0;j<(width-2*border);j++)
	      data[(width*height - width*border + border) + i*width +j] = values[i*(width-2*border)+j];
      }
  }
  
};

#include <Data/Data.l2c>
