#include <iostream>

#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/llcmmpi.h>
#include <mpi.h>
#include <stdint.h>
#include "exchange.h"

#define TO(y,x) A_to[ (x) + width * (y) ]

using ::std::cerr;
using ::std::cout;
using ::std::endl;


namespace jacobi {

namespace impl {

class MPIConnector :
		virtual public Exchange
{
public:
	MPI_Comm comm;
	uint16_t pos; // UP (1), DOWN (2), RIGTH (3), LEFT (4)

	int16_t rank;
	
	MPIConnector():
			pos(0),
			rank(MPI_PROC_NULL)
	{
	}
	
	virtual void recv_ready(double* A_to, const size_t width, const size_t height, const size_t bloc_x, const size_t bloc_y, const size_t bloc_width, const size_t bloc_height) {
		MPI_Type_vector(bloc_height, bloc_width, width, MPI_DOUBLE, &m_recv_border);

		MPI_Type_commit(&m_recv_border);

		MPI_Irecv(&TO(bloc_y,bloc_x),1,m_recv_border,rank,0,comm,&rq_r);

	};

	virtual void send_ready( double* A_to, const size_t width, const size_t height, const size_t bloc_x, const size_t bloc_y, const size_t bloc_width, const size_t bloc_height) {
		MPI_Type_vector(bloc_height, bloc_width, width, MPI_DOUBLE, &m_send_border);
		MPI_Type_commit(&m_send_border);

		MPI_Isend(&TO(bloc_y,bloc_x),1,m_send_border,rank,0,comm,&rq_s);

	};

	virtual void recv_wait() {
		MPI_Wait(&rq_r, &status_r) ;
	
		MPI_Type_free(&m_recv_border);
	};

	virtual void send_wait() {
		MPI_Wait(&rq_s, &status_s) ;
	
		MPI_Type_free(&m_send_border);

	};
	
  private:
	MPI_Request rq_r;
	MPI_Request rq_s;
	MPI_Datatype m_send_border;
	MPI_Datatype m_recv_border;
	MPI_Status status_r;
	MPI_Status status_s;
};

LCMP(MPIConnector)
	L_CPP_PROVIDE(Exchange, exchange)
	L_MPI_COMM(comm)
	L_PROPERTY(uint16_t, pos)
	L_PROPERTY(int16_t, rank)
LEND

}

}
