#include <algorithm>
#include <iostream>

#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/llcmmpi.h>
#include <mpi.h>
#include <stdint.h>
#include <sys/time.h>
#include <time.h>

#include <checkdata.h>
#include <computer.h>
#include <initializer.h>
#include <XPcomputer.h>
#include <chronometer.hpp>

typedef struct timeval my_time_t;
#define get_time(a) gettimeofday(&a, NULL)
#define timing_diff_usec(a,b) (((long long)(b).tv_sec - (a).tv_sec)*1000000+((long long)(b).tv_usec - (a).tv_usec))


#define FROM(y,x) A_from[ (x) + w * (y) ]
#define TO(y,x) A_to[ (x) + w * (y) ]
#define RES(y,x) res[ (x) + total_width * (y) ]

using ::std::cerr;
using ::std::cout;
using ::std::endl;
using ::std::min;
using ::std::swap;

namespace jacobi {

namespace impl {

class MPIDriver:
	virtual public XPComputer
{
public:
	uint8_t debug;
	
	uint16_t nb_tiles_y, nb_tiles_x;
  uint32_t nbiter;
	
	CheckData* checkdata;
	
	MPI_Comm comm;
	
	Computer* computer;
	
	Initializer* initialization;
	
	MPIDriver ():
			debug(0xFF),
			nb_tiles_y(0),
			nb_tiles_x(0),
			checkdata(0),
			comm(MPI_COMM_NULL),
			computer(0),
			initialization(0)
	{
	}
	
	uint64_t compute ( size_t total_width, size_t total_height, unsigned long niter )
	{
	  //#ifndef NDEBUG
	  cerr << "[MPIDriver] compute(" << total_width << ", " << total_height << ", " << niter << ')' << endl;
	  //#endif //NDEBUG
		
		size_t w = ((total_width-2)/nb_tiles_x)+2;
		size_t h = ((total_height-2)/nb_tiles_y)+2;
		
		MPI_Status stat;
		
		int m_rank;
		MPI_Comm_rank(comm, &m_rank);
		int m_size;
		MPI_Comm_size(comm, &m_size);
		
		if ( m_size != nb_tiles_x*nb_tiles_y ) {
		  //cout << "[MPIDriver] number of processes different than number of domain" << endl;
			MPI_Abort( MPI_COMM_WORLD, 1 );
		}
		
		int up = m_rank+1; 
		int down = m_rank-1; 
		int left = m_rank-nb_tiles_y; 
		int right = m_rank+nb_tiles_y; 
		
		int tile_y = m_rank%nb_tiles_y;
		int tile_x = (m_rank-tile_y)/nb_tiles_y;
		
		if ( tile_y == nb_tiles_y-1 ) h = total_height - tile_y*(h-2);
		if ( tile_x == nb_tiles_x-1 ) w = total_width - tile_x*(w-2);
		
		
		MPI_Datatype column;
		MPI_Type_vector(h, 1, w, MPI_DOUBLE, &column);
		MPI_Type_commit(&column);
		
		if ( tile_y == 0 ) down = MPI_PROC_NULL;
		if ( tile_y == nb_tiles_y-1 ) up = MPI_PROC_NULL;
		if ( tile_x == 0 ) left = MPI_PROC_NULL;
		if ( tile_x == nb_tiles_x-1 ) right = MPI_PROC_NULL;
		
		double* A_from = new double[h*w];
		double* A_to = new double[h*w];
		
		// matrix initialization
		initialization->init(ArrayPart< double >(A_from, w, h, 0, 0, w, h), tile_x==0, tile_x == nb_tiles_x-1, tile_y == 0, tile_y == nb_tiles_y-1 );
		initialization->init(ArrayPart< double >(A_to, w, h, 0, 0, w, h), tile_x==0, tile_x == nb_tiles_x-1, tile_y == 0, tile_y == nb_tiles_y-1);
		
		if ( debug & 1 ) {
			for ( int id=0; id<m_size; ++id) {
				MPI_Barrier(comm);
				if ( id == m_rank ) {
					for ( size_t yy=0; yy<h; ++yy) {
					  cerr << '[' << yy << '(' << tile_x << ',' << tile_y << ")@START]\t";
						for ( size_t xx = 0; xx<w; ++xx ) {
						  cerr << FROM(yy, xx) << '\t';
						}
						cerr << endl;
					}
					cerr << " ================================ " << endl;
				}
			}
		}
		
		//-----Time calculation
		Chronometer chrono;
		chrono.start();
		//-----

		// Heating caches !
		for ( int iter=0; iter<nbiter/*min(10UL, niter)*/; ++iter ) {
			
		       // border update                                                                                                                                                                  
		       MPI_Sendrecv(&TO(h-2, 0), w, MPI_DOUBLE, up, 0, &TO(0, 0), w, MPI_DOUBLE, down, 0, comm, &stat);
		       MPI_Sendrecv(&TO(1, 0), w, MPI_DOUBLE, down, 0, &TO(h-1, 0), w, MPI_DOUBLE, up, 0, comm, &stat);
		       MPI_Sendrecv(&TO(0, 1), 1, column, left, 0, &TO(0, w-1), 1, column, right, 0, comm, &stat);
		       MPI_Sendrecv(&TO(0, w-2), 1, column, right, 0, &TO(0, 0), 1, column, left, 0, comm, &stat);

			computer->one_iter(A_from, A_to, w, h, 1, 1, w-2, h-2);
			
			swap(A_from, A_to);
		}

		//-----Time calculation
		chrono.stop();
		cout<<" Time TOTAL CALCULATION : "<<chrono.dureeCalcule()<<endl;
		//-----
		
		MPI_Barrier(comm);
		my_time_t tick_start;
		get_time ( tick_start );
		
		/*for ( int iter=0; iter<niter; ++iter ) {
			
#ifndef NDEBUG
			if ( debug & 2 ) {
				for ( int id=0; id<m_size; ++id) {
					MPI_Barrier(comm);
					if ( id == m_rank ) {
						for ( size_t yy=0; yy<h; ++yy) {
							cerr << '[' << yy << '(' << tile_x << ',' << tile_y << ")@" << iter << "]\t";
							for ( size_t xx = 0; xx<w; ++xx ) {
								cerr << FROM(yy, xx) << '\t';
							}
							cerr << endl;
						}
						cerr << " ================================ " << endl;
					}
				}
			}
#endif //NDEBUG
			
			computer->one_iter(A_from, A_to, w, h, 1, 1, w-2, h-2);
			
			// border update
			MPI_Sendrecv(&TO(h-2, 0), w, MPI_DOUBLE, up, 0, &TO(0, 0), w, MPI_DOUBLE, down, 0, comm, &stat);
			MPI_Sendrecv(&TO(1, 0), w, MPI_DOUBLE, down, 0, &TO(h-1, 0), w, MPI_DOUBLE, up, 0, comm, &stat);
			MPI_Sendrecv(&TO(0, 1), 1, column, left, 0, &TO(0, w-1), 1, column, right, 0, comm, &stat);
			MPI_Sendrecv(&TO(0, w-2), 1, column, right, 0, &TO(0, 0), 1, column, left, 0, comm, &stat);
			
			swap(A_from, A_to);
		}
		MPI_Barrier(comm);
		*/my_time_t tick_end;
		get_time ( tick_end );
		
		//if (checkdata) checkdata->compare(ArrayPart<double>(A_from, w, h, 1, 1, w-2, h-2), "MPIDriver");
		
		delete[] A_to;
		delete[] A_from;
		
		unsigned long long time_usec = timing_diff_usec(tick_start, tick_end);
		
		unsigned long long max_time;
		MPI_Allreduce(&time_usec, &max_time, 1, MPI_UNSIGNED_LONG_LONG, MPI_MAX, comm);
		return max_time;
	}
	
};

LCMP(MPIDriver)
	L_PROPERTY(uint8_t, debug);
	L_PROPERTY(uint16_t, nb_tiles_y);
	L_PROPERTY(uint16_t, nb_tiles_x);
        L_PROPERTY(uint32_t, nbiter);
        L_CPP_PROVIDE(XPComputer, xpcomputer);
	L_MPI_COMM(comm);
	L_CPP_USE(Computer, computer);
	L_CPP_USE(CheckData, checkdata);
	L_CPP_USE(Initializer, initialization);
LEND

}

}
