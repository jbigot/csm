#ifndef COMMUNICATIONS_H
#define COMMUNICATIONS_H

#include "mpi.h"
#include "chronometer.hpp"
//#include "mpi_.hpp"

//namespace skelgis{

  //!  template communication Class 
  /*!
    This template class defines the MPI initializations, the MPI communications
  */
  //-------------------------------------------------------------------------------
  template<class T> class Communications
  //-------------------------------------------------------------------------------
  {
  public:
    //! used by get borders to make exchanges
    /*!
      \param toGet is the data to get from mpi process and omp thread
      \param toSend is the data to send to mpi process and omp thread
      \param size is the size of data to send and recv
      \param rank is the mpi rank to exchange with
    */
    //-------------------------------------------------------------------------------
    static void Exchanges(T * toSend, T* toGet,char * get,unsigned int size,int rank)//, MPI_Comm comm)
    //-------------------------------------------------------------------------------
    {
      //MPI_Status status;
      //MPI_Request request;
      
// #ifdef DEBUG
//       std::stringstream st;
//       st<<"Values to give to rank "<<rank<<"\n";
//       for(int j=0;j<size;j++)
// 	{
// 	  st<<toSend[j]<<" ";
// 	}
//       st<<"\n";
//       Mpi_::print(st.str());
// #endif

      /*int mpi_rank;
      MPI_Comm_rank(MPI_COMM_WORLD,&(mpi_rank));
      std::cout<<mpi_rank<<" - size to send "<<size<<std::endl;
      

      Chronometer chrono;
      chrono.start();*/

      //work for every basic types not for complex types
      //char * get = new char[size*sizeof(T)];
      //MPI_Sendrecv(reinterpret_cast<char *>(toSend), size*(sizeof(T)), MPI_BYTE,rank,0,get,size*(sizeof(T)),MPI_BYTE,rank,0,MPI_COMM_WORLD/*comm*/,MPI_STATUS_IGNORE);
      MPI_Sendrecv(toSend, size, MPI_DOUBLE,rank,0,toGet,size,MPI_DOUBLE,rank,0,MPI_COMM_WORLD/*comm*/,MPI_STATUS_IGNORE);
      //for(int i=0;i<size;i++)
      //memcpy(&toGet[i], get+i*sizeof(T), sizeof(T));
      //delete [] get;
      
      /*chrono.stop();
	std::cout<<mpi_rank<<" - Time COMM : "<<chrono.dureeCalcule()<<std::endl;*/

// #ifdef DEBUG
//       std::stringstream st2;
//       st2<<"values given by rank "<<rank<<"\n";
//       for(int j=0;j<size;j++)
// 	{
// 	  st2<<toGet[j]<<" ";
// 	}
//       st2<<"\n";
//       Mpi_::print(st2.str());
// #endif
    }
    //-------------------------------------------------------------------------------
  };
  //-------------------------------------------------------------------------------
//}

#endif

