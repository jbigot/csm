#ifndef COMMUNICATIONS_H
#define COMMUNICATIONS_H

#include "mpi.h"
#include "mpi_.hpp"

namespace skelgis{

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
    static void Exchanges(T * toSend, T* toGet,unsigned int size,int rank)
    //-------------------------------------------------------------------------------
    {
      MPI_Status status;
      MPI_Request request;
      
#ifdef DEBUG
      std::stringstream st;
      st<<"Values to give to rank "<<rank<<"\n";
      for(int j=0;j<size;j++)
	{
	  st<<toSend[j]<<" ";
	}
      st<<"\n";
      Mpi_::print(st.str());
#endif

      //work for every basic types not for complex types
      char * get = new char[size*sizeof(T)];
      MPI_Sendrecv(reinterpret_cast<char *>(toSend), size*(sizeof(T)), MPI_BYTE,rank,0,get,size*(sizeof(T)),MPI_BYTE,rank,0,MPI_COMM_WORLD,&status);
      for(int i=0;i<size;i++)
	  memcpy(&toGet[i], get+i*sizeof(T), sizeof(T));
      delete [] get;

#ifdef DEBUG
      std::stringstream st2;
      st2<<"values given by rank "<<rank<<"\n";
      for(int j=0;j<size;j++)
	{
	  st2<<toGet[j]<<" ";
	}
      st2<<"\n";
      Mpi_::print(st2.str());
#endif
    }
    //-------------------------------------------------------------------------------
  };
  //-------------------------------------------------------------------------------
}

#endif

