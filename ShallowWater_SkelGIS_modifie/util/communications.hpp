#ifndef COMMUNICATIONS_H
#define COMMUNICATIONS_H

#include "string.h"
#include "mpi.h"
#include <stdlib.h>

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
  static void Exchanges(T * toSend, T* toGet,unsigned int size,int rank, MPI_Comm comm, int tag)
  //-------------------------------------------------------------------------------
  {
    MPI_Status status;
    MPI_Request request;

    //work for every basic types not for complex types
    char * get = new char[size*sizeof(T)];
    MPI_Sendrecv(reinterpret_cast<char *>(toSend), size*(sizeof(T)), MPI_BYTE,rank,tag,
                    get,size*(sizeof(T)),MPI_BYTE,rank,tag,comm/*MPI_COMM_WORLD*/,&status);
    for(int i=0;i<size;i++)
      memcpy(&toGet[i], get+i*sizeof(T), sizeof(T));
    delete [] get;
  }
  //-------------------------------------------------------------------------------

  /*!< requests_s is the list of requests for send communications (one for each processor) */
    MPI_Request * requests_s;
    /*!< requests_s is the list of requests for receive communications (one for each processor) */
    MPI_Request * requests_r;
    /*!< requests_s is the list of status for send communications (one for each processor) */
    MPI_Status * status_s;
    /*!< requests_s is the list of status for receive communications (one for each processor) */
    MPI_Status * status_r;
    char ** get;

    //-------------------------------------------------------------------------------
    //! constructor
    /*!
    */
    //-------------------------------------------------------------------------------
    Communications()
    //-------------------------------------------------------------------------------
    {
      int nb_proc = 0;
      MPI_Comm_size(MPI_COMM_WORLD, &nb_proc);
      requests_s = (MPI_Request *)calloc(nb_proc,sizeof(MPI_Request));
      requests_r = (MPI_Request *)calloc(nb_proc,sizeof(MPI_Request));
      status_s = (MPI_Status *)calloc(nb_proc,sizeof(MPI_Status));
      status_r = (MPI_Status *)calloc(nb_proc,sizeof(MPI_Status));
      get = (char **)calloc(nb_proc,sizeof(char*));
    }
    //-------------------------------------------------------------------------------
    //! destructor
    /*!
    */
    //-------------------------------------------------------------------------------
    ~Communications()
    //-------------------------------------------------------------------------------
    {
      free(requests_s);
      free(requests_r);
      free(status_s);
      free(status_r);
      free(get);
    }
    //-------------------------------------------------------------------------------
    //! A non blocking MPI send is initialized
    /*!
      \param buf is the sending buffer
      \param count is the number of elements in the buffer
      \param dest is the destination processor
      \return the return value of the MPI_Isend function
    */
    //-------------------------------------------------------------------------------
    inline int ISend(T *buf, int count, int dest, MPI_Comm comm)
    //-------------------------------------------------------------------------------
    {
      return  MPI_Isend(reinterpret_cast<char *>(buf), count*(sizeof(T)), MPI_BYTE, dest, 0, comm, 
        &(requests_s[dest]));
    }
    //-------------------------------------------------------------------------------
    //! A non blocking MPI receive is initialized
    /*!
      \param count is the number of elements to receive
      \param source is the source processor
      \return the return value of the MPI_Irecv function
    */
    //-------------------------------------------------------------------------------
    int IRecv(int count, int source, MPI_Comm comm)
    //-------------------------------------------------------------------------------
    {
      get[source] = new char[count*sizeof(T)];
      return MPI_Irecv(get[source], count*(sizeof(T)), MPI_BYTE, source, 0, comm, &(requests_r[source]));
    }
    //-------------------------------------------------------------------------------
    //! A MPI wait call to wait for the send exchange with a precise processor
    /*!
      \param dest is the destination processor the send is waiting
      \return the return value of the MPI_Wait function
    */
    //-------------------------------------------------------------------------------
    inline int Wait_s(int dest)
    //-------------------------------------------------------------------------------
    {
      return MPI_Wait(&(requests_s[dest]), &(status_s[dest]));
    }
    //-------------------------------------------------------------------------------
    //! A MPI wait call to wait for the send exchange with a precise processor
    /*!
      \param dest is the destination processor the send is waiting
      \return the return value of the MPI_Wait function
    */
    //-------------------------------------------------------------------------------
    int Wait_r(T * toRecv, int src,int count)
    //-------------------------------------------------------------------------------
    {
      int ret = MPI_Wait(&(requests_r[src]), &(status_r[src]));
      for(int i=0;i<count;i++)
        memcpy(&toRecv[i], get[src]+i*sizeof(T), sizeof(T));

      if(NULL!=get && NULL!= get[src])
        delete [] get[src];

      return ret;
    }
    //-------------------------------------------------------------------------------
};

#endif
