#ifndef COMMUNICATIONS_H
#define COMMUNICATIONS_H

#include "mpi.h"
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
    static void Exchanges(T * toSend, T* toGet,char * get,unsigned int size,int rank, MPI_Comm comm)
    //-------------------------------------------------------------------------------
    {
      MPI_Status status;
      MPI_Request request;

      //work for every basic types not for complex types
      MPI_Sendrecv(reinterpret_cast<char *>(toSend), size*(sizeof(T)), MPI_BYTE,rank,0,get,size*(sizeof(T)),MPI_BYTE,rank,0,/*MPI_COMM_WORLD*/comm,&status);
      for(int i=0;i<size;i++)
	       memcpy(&toGet[i], get+i*sizeof(T), sizeof(T));
    }
    //-------------------------------------------------------------------------------
  };
  //-------------------------------------------------------------------------------
//}

#endif

