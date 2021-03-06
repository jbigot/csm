#ifndef Data_H
#define Data_H

#include <iostream>
#include <mpi.h>
#include <math.h>
#include <stdint.h>

#include "../util/communications.hpp"
#include <sstream>

using ::std::cout;
using ::std::endl;

namespace skelgis{
class Data
{

public:
  //components variables
   Structure * str;//! structure
  //Inith* initializer; //! initializer
  
  //variables
  int64_t width,height; //! width and height of the data array
  double * data; //! data array
  int16_t border; //! size of the physical border needed (limit conditions), linked to the stencil shape also
  int16_t domain; //! 1=cell 2=edgex 3=edgey
  double init_val;

  //MPI communications                                                                                                                                        
  Communications<double> * exchange;

  //physical border
  int64_t swphbL,swphbR,swphbU,swphbD;
  int64_t wphbL,wphbR,wphbU,wphbD;
  int64_t shphbL,shphbR,shphbU,shphbD;
  int64_t hphbL,hphbR,hphbU,hphbD;

  int32_t* neigh;
  int32_t tile_y;
  int32_t nb_tile_y;
  int32_t tile_x;
  int32_t nb_tile_x;

  //-------------------------------------------------------------------------------                                                                                                                      
  //! default constructor                                                                                                                                            
  /*! */
  //-------------------------------------------------------------------------------
  Data (int16_t b,int16_t d,double init,Structure * s):data(0),width(0),height(0),border(b),domain(d),init_val(init),str(s)//,initializer(0)
  //-------------------------------------------------------------------------------
  {
    exchange = new Communications<double>();
    
    if(str)// && initializer)
      {
        //str->getBorder(border);
        str->getSizes(width,height); //get local size
        if(domain==1)
        {
          width = width+2*border; //add the border
          height = height+2*border; //add the border
        }
        else if(domain==2)
        {
          width = width+2*border+1; //add the border
          height = height+2*border; //add the border
        }
        else if(domain==3)
        {
          width = width+2*border; //add the border
          height = height+2*border+1; //add the border
        }
        data = new double[width*height];
        for(int64_t i=0;i<width*height;i++)
          data[i]=init_val;
      
        neigh=str->getNeighbors();
        tile_y = str->getTileY();
        nb_tile_y = str->getNbTileY();
        tile_x = str->getTileX();
        nb_tile_x = str->getNbTileX();

        phBLimits();

  //cout<<"size of data "<<width*height<<endl;
      }
  }
  //-------------------------------------------------------------------------------                                                                                                                      
  //! default destructor                                                                                                                                                                               
  /*! */
  //-------------------------------------------------------------------------------
  ~Data()
  //-------------------------------------------------------------------------------
  {
    //cout<<"[Data] destruction"<<endl;
    delete [] data;
    delete exchange;
  }
  //-------------------------------------------------------------------------------
  //! update interface
  /*! */
  //-------------------------------------------------------------------------------
  void update ()
  //-------------------------------------------------------------------------------
  {
    //int nbp = 0;
    //MPI_Comm_size(MPI_COMM_WORLD,&nbp);
    //if(nbp>1)
    //{
  	int rank;
  	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  	//if(rank%2==0)
  	//{
	    if(tile_y!=0 && neigh[0]>-1)
	      {
	  double * toSend = getUpBorderToSend();
	  double * toGet = new double[(width-2*border)*border];
	  std::stringstream ss;
	  ss << 0 << rank << neigh[0];
	  int tag;
	  ss >> tag;
	  Communications<double>::Exchanges(toSend,toGet,(width-2*border)*border,neigh[0],MPI_COMM_WORLD,tag);
	  setUpBorder(toGet);
	  delete [] toSend;
	  delete [] toGet;
	      }
	    if(tile_y!=(nb_tile_y-1) && neigh[2]>-1)
	      {          
	  double * toSend = getDownBorderToSend();
	  double * toGet = new double[(width-2*border)*border];
	  std::stringstream ss;
	  ss << 0 << neigh[2] << rank;
	  int tag;
	  ss >> tag;
	  Communications<double>::Exchanges(toSend,toGet,(width-2*border)*border,neigh[2],MPI_COMM_WORLD,tag);
	  setDownBorder(toGet);
	  delete [] toSend;
	  delete [] toGet;
	      }

	    if(tile_x!=(nb_tile_x-1) && neigh[1]>-1)
      {  
  double * toSend = getRightBorderToSend();
  double * toGet = new double[(height-2*border)*border];
  std::stringstream ss;
  ss << 1 << rank << neigh[1];
  int tag;
  ss >> tag;
  Communications<double>::Exchanges(toSend,toGet,(height-2*border)*border,neigh[1],MPI_COMM_WORLD,tag);
  setRightBorder(toGet);
  delete [] toSend;
  delete [] toGet;
      }
    if(tile_x!=0 && neigh[3]>-1)
      {
  double * toSend = getLeftBorderToSend();
  double * toGet = new double[(height-2*border)*border];
  std::stringstream ss;
  ss << 1 << neigh[3] << rank;
  int tag;
  ss >> tag;
  Communications<double>::Exchanges(toSend,toGet,(height-2*border)*border,neigh[3],MPI_COMM_WORLD,tag);
  setLeftBorder(toGet);
  delete [] toSend;
  delete [] toGet;
      }
    //  }
    /*else
	  {
	  	if(tile_y!=(nb_tile_y-1) && neigh[2]>-1)
	      {          
	  double * toSend = getDownBorderToSend();
	  double * toGet = new double[(width-2*border)*border];
	  std::stringstream ss;
	  ss << 0 << neigh[2] << rank;
	  int tag;
	  ss >> tag;
	  Communications<double>::Exchanges(toSend,toGet,(width-2*border)*border,neigh[2],MPI_COMM_WORLD,tag);
	  setDownBorder(toGet);
	  delete [] toSend;
	  delete [] toGet;
	      }
	      if(tile_y!=0 && neigh[0]>-1)
	      {
	  double * toSend = getUpBorderToSend();
	  double * toGet = new double[(width-2*border)*border];
	  std::stringstream ss;
	  ss << 0 << rank << neigh[0];
	  int tag;
	  ss >> tag;
	  Communications<double>::Exchanges(toSend,toGet,(width-2*border)*border,neigh[0],MPI_COMM_WORLD,tag);
	  setUpBorder(toGet);
	  delete [] toSend;
	  delete [] toGet;
	      }
	      if(tile_x!=0 && neigh[3]>-1)
      {
  double * toSend = getLeftBorderToSend();
  double * toGet = new double[(height-2*border)*border];
  std::stringstream ss;
  ss << 1 << neigh[3] << rank;
  int tag;
  ss >> tag;
  Communications<double>::Exchanges(toSend,toGet,(height-2*border)*border,neigh[3],MPI_COMM_WORLD,tag);
  setLeftBorder(toGet);
  delete [] toSend;
  delete [] toGet;
      }
      if(tile_x!=(nb_tile_x-1) && neigh[1]>-1)
      {  
  double * toSend = getRightBorderToSend();
  double * toGet = new double[(height-2*border)*border];
  std::stringstream ss;
  ss << 1 << rank << neigh[1];
  int tag;
  ss >> tag;
  Communications<double>::Exchanges(toSend,toGet,(height-2*border)*border,neigh[1],MPI_COMM_WORLD,tag);
  setRightBorder(toGet);
  delete [] toSend;
  delete [] toGet;
      }
      }*/

	//}
    //MPI_Barrier(comm);
  }
  //-------------------------------------------------------------------------------
  //! start_comm interface
  /*! */
  //-------------------------------------------------------------------------------
  void start_comm ()
  //-------------------------------------------------------------------------------
  {
    if(tile_y!=0)
      {
        double * toSend = getUpBorderToSend();
        exchange->ISend(toSend,(width-2*border)*border,neigh[0],MPI_COMM_WORLD);
        exchange->IRecv((width-2*border)*border,neigh[0],MPI_COMM_WORLD);
        delete [] toSend;
      }
    if(tile_y!=(nb_tile_y-1))
      {          
        double * toSend = getDownBorderToSend();
        exchange->ISend(toSend,(width-2*border)*border,neigh[2],MPI_COMM_WORLD);
        exchange->IRecv((width-2*border)*border,neigh[2],MPI_COMM_WORLD);
        delete [] toSend;
      }
    if(tile_x!=(nb_tile_x-1))
      {  
        double * toSend = getRightBorderToSend();
        exchange->ISend(toSend,(height-2*border)*border,neigh[1],MPI_COMM_WORLD);
        exchange->IRecv((height-2*border)*border,neigh[1],MPI_COMM_WORLD);
        delete [] toSend;
      }
    if(tile_x!=0)
      {
        double * toSend = getLeftBorderToSend();
        exchange->ISend(toSend,(height-2*border)*border,neigh[3],MPI_COMM_WORLD);
        exchange->IRecv((height-2*border)*border,neigh[3],MPI_COMM_WORLD);
        delete [] toSend;
      }
  }
  //-------------------------------------------------------------------------------
  //! end_comm interface
  /*! */
  //-------------------------------------------------------------------------------
  void end_comm ()
  //-------------------------------------------------------------------------------
  {
    if(tile_y!=0)
      {
        double * toGet = new double[(width-2*border)*border];
        exchange->Wait_s(neigh[0]);
        exchange->Wait_r(toGet,neigh[0],(width-2*border)*border);
        setUpBorder(toGet);
        delete [] toGet;
      }
    if(tile_y!=(nb_tile_y-1))
      {          
        double * toGet = new double[(width-2*border)*border];
        exchange->Wait_s(neigh[2]);
        exchange->Wait_r(toGet,neigh[2],(width-2*border)*border);
        setDownBorder(toGet);
        delete [] toGet;
      }
    if(tile_x!=(nb_tile_x-1))
      {  
        double * toGet = new double[(height-2*border)*border];
        exchange->Wait_s(neigh[1]);
        exchange->Wait_r(toGet,neigh[1],(height-2*border)*border);
        setRightBorder(toGet);
        delete [] toGet;
      }
    if(tile_x!=0)
      {
        double * toGet = new double[(height-2*border)*border];
        exchange->Wait_s(neigh[3]);
        exchange->Wait_r(toGet,neigh[3],(height-2*border)*border);
        setLeftBorder(toGet);
        delete [] toGet;
      }
  }
  //-------------------------------------------------------------------------------                                                                                                                      
  //! Interface get the border size
  /*!  */
  //------------------------------------------------------------------------------- 
  inline int16_t getBorder(){return border;}
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
  //-------------------------------------------------------------------------------
  //! Return the pointer on the data
  /*!  */
  //-------------------------------------------------------------------------------
  inline double * getData(){return data;}
  //-------------------------------------------------------------------------------
  //! Return width
  /*!  */
  //-------------------------------------------------------------------------------
  inline int64_t getWidth(){return width;}
  //-------------------------------------------------------------------------------
  //! Return width local
  /*!  */
  //-------------------------------------------------------------------------------
  inline int64_t getWidthLoc(){return width-2*border;}
  //-------------------------------------------------------------------------------
  //! Return height local
  /*!  */
  //-------------------------------------------------------------------------------
  inline int64_t getHeightLoc(){return height-2*border;}
  //-------------------------------------------------------------------------------

  inline int64_t getStartWPHBL(){return swphbL;}
  inline int64_t getWidthPHBL(){return wphbL;}
  inline int64_t getStartHPHBL(){return shphbL;}
  inline int64_t getHeightPHBL(){return hphbL;}

  inline int64_t getStartWPHBR(){return swphbR;}
  inline int64_t getWidthPHBR(){return wphbR;}
  inline int64_t getStartHPHBR(){return shphbR;}
  inline int64_t getHeightPHBR(){return hphbR;}

  inline int64_t getStartWPHBU(){return swphbU;}
  inline int64_t getWidthPHBU(){return wphbU;}
  inline int64_t getStartHPHBU(){return shphbU;}
  inline int64_t getHeightPHBU(){return hphbU;}

  inline int64_t getStartWPHBD(){return swphbD;}
  inline int64_t getWidthPHBD(){return wphbD;}
  inline int64_t getStartHPHBD(){return shphbD;}
  inline int64_t getHeightPHBD(){return hphbD;}

private:
  //-------------------------------------------------------------------------------
  //! compute start/end of physical border
  /*!  */
  //-------------------------------------------------------------------------------
  void phBLimits()
  //-------------------------------------------------------------------------------
  {
    if(tile_x==0) //left
    {
      swphbL = 0; 
      wphbL = border;
      shphbL = border;
      hphbL = height-border;//border+(height-2*border);
    }
    else
      swphbL = wphbL = shphbL = hphbL = 0;

    if(tile_x==nb_tile_x-1) //right
    {
      swphbR = width-border;//border+(width-2*border); 
      wphbR = width;//border+(width-2*border)+border;
      shphbR = border;
      hphbR = height-border;//border+(height-2*border);
    }
    else
      swphbR = wphbR = shphbR = hphbR = 0;

    if(tile_y==0) //up
    {
      swphbU = border; 
      wphbU = width-border;//border+(width-2*border);
      shphbU = 0;
      hphbU = border;
    }
    else
      swphbU = wphbU = shphbU = hphbU = 0;

    if(tile_y==nb_tile_y-1) //down
    {
      swphbD = border; 
      wphbD = width-border;//border+(width-2*border);
      shphbD = height-border;//border+(height-2*border);
      hphbD = height;
    }
    else
      swphbD = wphbD = shphbD = hphbD = 0;
  }
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

}

#endif
