#include <iostream>
#include <math.h>
#include <mpi.h>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <structh.h>

//#define DEBUG

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

class Structure:virtual public Structh
{
public:
  //Properties from the assembly
  int64_t globW,globH; //! width and height of the global domain
  int32_t nb_proc; //! number of processors
  //this information is on Data component:
  //int16_t border_size; //! size of the physical border needed (limit conditions), linked to the stencil shape also

  //Variables
  int64_t locW,locH; //! width and height of the local domain
  //int64_t bordW,bordH; //! width and height of the local domain with borders
  int32_t tile_x,tile_y; //! local tile position in the global domain
  int32_t nb_tile_x,nb_tile_y; //! total number of tiles in x and y
  int32_t rank;
  int32_t remainderh; //! the remainder of globH-floor(globH/nb_tile_y)*nb_tile_y;
  int32_t remainderw; //! the remainder of globW-floor(globW/nb_tile_x)*nb_tile_x;
  int32_t* ranks; //! array of the MPI ranks of neighborhood processes

  //-------------------------------------------------------------------------------
  //! default constructor of the distributed matrix interface
  /*! */
  //-------------------------------------------------------------------------------
  Structure ():nb_proc(0),globH(0),globW(0),locH(0),locW(0)/*,bordH(0),bordW(0),border_size(0)*/,tile_x(0),tile_y(0),nb_tile_x(0),nb_tile_y(0),rank(0),remainderh(0),remainderw(0),ranks(0)
  {}
  //-------------------------------------------------------------------------------
  //! destructor of the distributed matrix
  /*! */
  //-------------------------------------------------------------------------------
  ~Structure()
  { delete [] ranks;}
  //-------------------------------------------------------------------------------

  //-------------------------------------------------------------------------------                                                                                                                      
  //! Interface Init of the component                                                                                                                                                
  /*!                                                                                                                                                                                                    
    \param w is the width of the global cartesian domain                                                                                                                                                 
    \param h is the height of the global cartesian domain                                                                                                                                                
    \param p is the number of processors                                                                                                                                                                
  */
  //-------------------------------------------------------------------------------                                                                                                                      
  void init (/*int64_t w, int64_t h, int32_t p*/)
  //-------------------------------------------------------------------------------                                                                                                                     
  {
    //initialization of variables
    //globW=w;globH=h;nb_proc=p;
    //std::cout<<"number of processors "<<nb_proc<<std::endl;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    //computation of the size of the local table
    computeLocalSizes();
    //computation of the MPI neighborhood ranks
    computeMPIRanks();
  }
  //-------------------------------------------------------------------------------                                                                                                                      
  //! Interface get the border size
  /*!  */
  //------------------------------------------------------------------------------- 
  /*void getBorder(int16_t &border)
  //------------------------------------------------------------------------------- 
  {
    border=border_size;
  }*/
  //------------------------------------------------------------------------------- 
  //! Interface get sizes for data creation                                                                                                                                                              
  /*!  */
  //-------------------------------------------------------------------------------                                                                                                                      
  void getSizes(int64_t &width, int64_t &height)
  //-------------------------------------------------------------------------------                                                                                                                      
  {
    width=locW;//bordW;
    height=locH;//bordH;
  }
  //-------------------------------------------------------------------------------                                                                                                                      
  //! Interface get the array of MPI neighbors
  /*! */
  //-------------------------------------------------------------------------------
  int32_t* getNeighbors()
  //-------------------------------------------------------------------------------
  {
    return ranks;
  }
  //-------------------------------------------------------------------------------
  //! Interface get tile_y
  /*! */
  //-------------------------------------------------------------------------------  
  int32_t getTileY()
  //-------------------------------------------------------------------------------  
  {
    return tile_y;
  }
  //-------------------------------------------------------------------------------                                                                                                                      
  //! Interface get nb_tile_y                                                                                                                                                                           
  /*! */
  //-------------------------------------------------------------------------------                                                                                                                      
  int32_t getNbTileY()
  //-------------------------------------------------------------------------------                                                                                                                      
  {
    return nb_tile_y;
  }
  //-------------------------------------------------------------------------------                                                                                                                      
  //! Interface get tile_x                                                                                                                                                                               
  /*! */
  //-------------------------------------------------------------------------------                                                                                                                      
  int32_t getTileX()
  //-------------------------------------------------------------------------------                                                                                                                      
  {
    return tile_x;
  }
  //-------------------------------------------------------------------------------                                                                                                                      
  //! Interface get nb_tile_x                                                                                                                                                                               
  /*! */
  //-------------------------------------------------------------------------------                                                                                                                      
  int32_t getNbTileX()
  //-------------------------------------------------------------------------------                                                                                                                      
  {
    return nb_tile_x;
  }
  //-------------------------------------------------------------------------------                                                                                                                      
  //! Interface get globW
  /*! */
  //-------------------------------------------------------------------------------                                                                                                                      
  int64_t getGlobW()
  //-------------------------------------------------------------------------------                                                                                                                      
  {
    return globW;
  }
  //-------------------------------------------------------------------------------
  //! Interface get globH
  /*! */
  //-------------------------------------------------------------------------------
  int64_t getGlobH()
  //-------------------------------------------------------------------------------
  {
    return globH;
  }

private:
  //-------------------------------------------------------------------------------
  void computeLocalSizes()
  //-------------------------------------------------------------------------------
  {
    //1- compute nb_tile_x and nb_tile_y                                                                                                                                                                 

    nb_tile_x = floor(sqrt(nb_proc));
    int nb = 1;
    while(nb_proc%nb_tile_x!=0)
      {
        nb_tile_x = floor(sqrt(nb_proc))-nb;
        nb++;
      }
    nb_tile_y = nb_proc/nb_tile_x;

    //nb_tile_x = floor(sqrt(nb_proc))-(nb_proc%int32_t(floor(sqrt(nb_proc))));
    //nb_tile_y = nb_proc/nb_tile_x;

//#ifdef DEBUG
//    cout<<"nb_tile_x and nb_tile_y :"<<nb_tile_x<<","<<nb_tile_y<<endl;
//#endif

    tile_y=floor(rank/nb_tile_x);
    tile_x=rank%nb_tile_x;

//#ifdef DEBUG
//    cout<<"tile_x and tile_y :"<<tile_x<<","<<tile_y<<endl;
//#endif
    
    //2- compute locW and locH                                                                                                                                                                            
    remainderh = 0;
    //Simple case : if the height of the matrix can be divided by the number of rows                                                                                                                      
    if(globH%nb_tile_y==0)
      locH=floor(globH/nb_tile_y);
    //else we add one element to each possible row, and let others with the basic number of elements                                                                                                      
    else
      {
  remainderh = globH - floor(globH/nb_tile_y)*nb_tile_y;
  if(tile_y<remainderh)
          locH=floor(globH/nb_tile_y)+1;
  else
          locH=floor(globH/nb_tile_y);
      }
    remainderw = 0;
    //Simple case : if the width of the matrix can be divided by the number of columns                                                                                                                    
    if(globW%nb_tile_x==0)
      locW=floor(globW/nb_tile_x);
    //else we add one element to each possible column, and let the others with the basic number of elements                                                                                               
    else
      {
  remainderw = globW - floor(globW/nb_tile_x)*nb_tile_x;
  if(tile_x<remainderw)
          locW=floor(globW/nb_tile_x)+1;
        else
          locW=floor(globW/nb_tile_x);
      }

    //std::cout<<"Proc "<<rank<<" - nb_tile_x,nb_tile_y = "<<nb_tile_x<<","<<nb_tile_y<<std::endl;
    //std::cout<<"Proc "<<rank<<" - tile_x,tile_y = "<<tile_x<<","<<tile_y<<std::endl;

    //3- compute bordW and bordH                                                                                                                                                                          
    //bordH = locH + border_size*2;
    //bordW = locW + border_size*2;
  }
  //-------------------------------------------------------------------------------
  int32_t getRank(int32_t row,int32_t col)
  //-------------------------------------------------------------------------------
  {
    return row*nb_tile_x+col;
  }
  //-------------------------------------------------------------------------------
  void computeMPIRanks()
  //-------------------------------------------------------------------------------
  {
    ranks = new int32_t[4];
    for(int i=0;i<4;i++)
      ranks[i] = -55555;
    //up
    if((tile_y-1)>=0)
      ranks[0] = getRank(tile_y-1,tile_x);
    //down
    if((tile_y+1)<nb_tile_y)
      ranks[2] = getRank(tile_y+1,tile_x);
    //right
    if((tile_x+1)<nb_tile_x)
      ranks[1] = getRank(tile_y,tile_x+1);
    //left
    if((tile_x-1)>=0)
      ranks[3] = getRank(tile_y,tile_x-1);
  }
  //-------------------------------------------------------------------------------
  
};

#include <Structure/Struct.l2c>
