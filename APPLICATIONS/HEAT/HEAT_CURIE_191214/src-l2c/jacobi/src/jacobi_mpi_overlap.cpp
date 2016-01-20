#include <algorithm>
#include <iostream>
#include <vector>

#include <mpi.h>
#include <stdint.h>
#include <sys/time.h>
#include <time.h>


typedef struct timeval my_time_t;
#define get_time(a) gettimeofday(&a, NULL)
#define timing_diff_usec(a,b) (((long long)(b).tv_sec - (a).tv_sec)*1000000+((long long)(b).tv_usec - (a).tv_usec))


#define FROM(y,x) A_from[ (x) + w * (y) ]
#define TO(y,x) A_to[ (x) + w * (y) ]


using ::std::cerr;
using ::std::cout;
using ::std::endl;
using ::std::sort;
using ::std::swap;
using ::std::vector;


const double top=10;
const double bottom=0;
const double left=5;
const double right=5;
const double inside=-10;
const uint32_t niter=100;
const uint16_t nxp=10;
const uint8_t debug=0;

uint32_t run_per_size=32;
const uint32_t slow_start=44;
const uint64_t l1_size=1572864;
const uint64_t l2_size=6291456;
const uint64_t l3_size=12582912;
const uint64_t ram_size=8589934592;


uint64_t compute(long total_width, long total_height, long niter, long nb_tiles_x, long nb_tiles_y)
{
	cout << "#[Jacobi_mpi] compute!" << endl;
	size_t w = ((total_width-2)/nb_tiles_x);
	size_t h = ((total_height-2)/nb_tiles_y);
	
	MPI_Status stat;
	
	MPI_Comm comm = MPI_COMM_WORLD;
	
	int m_rank;
	MPI_Comm_rank(comm, &m_rank);
	int m_size;
	MPI_Comm_size(comm, &m_size);
	
	if ( m_size != nb_tiles_x*nb_tiles_y ) {
		cout << "[Jacobi MPI] number of processes different than number of domain" << endl;
		MPI_Abort( MPI_COMM_WORLD, 1 );
	}

	int up = m_rank+1; 
	int down = m_rank-1; 
	int left = m_rank-nb_tiles_y; 
	int right = m_rank+nb_tiles_y; 

	int tile_y = m_rank%nb_tiles_y;
	int tile_x = (m_rank-tile_y)/nb_tiles_y;
	
	if ( tile_y*(h-2)  + h > total_height ) h = total_height - tile_y*(h-2);
	if ( tile_x*(w-2)  + w > total_width ) w = total_width - tile_x*(w-2);
	
	cout << "#[Jacobi_mpi] compute 2!" << endl;
	
	MPI_Datatype column;
	MPI_Type_vector(h, 1, w, MPI_DOUBLE, &column);
	MPI_Type_commit(&column);

	if ( tile_y == 0 ) down = MPI_PROC_NULL;
	if ( tile_y == nb_tiles_y-1 ) up = MPI_PROC_NULL;
	if ( tile_x == 0 ) left = MPI_PROC_NULL;
	if ( tile_x == nb_tiles_x-1 ) right = MPI_PROC_NULL;
	
	double* A_from = new double[h*w];
	double* A_to = new double[h*w];
	
	// Middle Init
	for ( size_t ii=0; ii<h*w; ++ii ) {
		A_from[ii] = inside;
		A_to[ii] = inside;
	}
	
	// Left border
	if ( tile_x == 0 ) {
		for ( size_t yy=0; yy<h; ++yy ) {
			FROM(yy, 0) = left;
			TO(yy, 0) = left;
		}
	}
	
	// Right border
	if ( tile_x == nb_tiles_x-1 ) {
		for ( size_t yy=0; yy<h; ++yy ) {
			FROM(yy, w-1) = right;
			TO(yy, w-1) = right;
		}
	}
	
	// Bottom border
	if ( tile_y == 0 ) {
		for ( size_t xx=0; xx<w; ++xx ) {
			FROM(0, xx) = bottom;
			TO(0, xx) = bottom;
		}
	}
	
	// Top border
	if ( tile_y == nb_tiles_y-1 ) {
		for ( size_t xx=0; xx<w; ++xx ) {
			FROM(h-1, xx) = top;
			TO(h-1, xx) = top;
		}
	}
	
	// Bottom/Left corner
	if ( tile_y == 0 && tile_x == 0 ) {
		FROM(0, 0) = (left+bottom)/2;
		TO(0, 0) = (left+bottom)/2;
	}
	
	// Bottom/Right corner
	if ( tile_y == 0 && tile_x == nb_tiles_x-1 ) {
		FROM(0, w-1) = (right+bottom)/2;
		TO(0, w-1) = (right+bottom)/2;
	}
	
	// Top/Left corners
	if ( tile_y == nb_tiles_y-1 && tile_x == 0 ) {
		FROM(h-1, 0) = (left+top)/2;
		TO(h-1, 0) = (left+top)/2;
	}
	
	// Top/Righ corners
	if ( tile_y == nb_tiles_y-1 && tile_x == nb_tiles_x-1 ) {
		FROM(h-1, w-1) = (right+top)/2;
		TO(h-1, w-1) = (right+top)/2;
	}
	
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
	
	cout << "#[Jacobi_mpi] compute 3!" << endl;
	
	MPI_Barrier(comm);

	my_time_t tick_start;
	get_time ( tick_start );
	
	for ( int iter=0; iter<niter; ++iter ) {

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
	
	cout << "#[Jacobi_mpi] compute 4!" <<iter<< endl;

		MPI_Request rq_r[4];
		MPI_Request rq_s[4];
		MPI_Status statuses_r[4];
		MPI_Status statuses_s[4];

		MPI_Irecv(&TO(h-1,0),w,MPI_DOUBLE,up,0,comm,&rq_r[0]);
		MPI_Irecv(&TO(0,0),w,MPI_DOUBLE,down,0,comm,&rq_r[1]);
		MPI_Irecv(&TO(0,w-1),1,column,left,0,comm,&rq_r[2]);
		MPI_Irecv(&TO(0,0),1,column,right,0,comm,&rq_r[3]);
	
	cout << "#[Jacobi_mpi] compute 5!" <<iter<< endl;
			  
		for ( int yy = 1; yy < (h -1) ; ++yy ) {
			for ( int xx = 1; xx < (w - 1) ; ++xx ) {
				TO(yy, xx) = 0.25*( FROM(yy-1, xx) + FROM(yy+1, xx) + FROM(yy, xx-1) + FROM(yy, xx+1) );
			}
		}
	
	cout << "#[Jacobi_mpi] compute 6!" <<iter<< endl;

		MPI_Isend(&TO(h-2,0),w,MPI_DOUBLE,up,0,comm,&rq_s[0]);
		MPI_Isend(&TO(1,0),w,MPI_DOUBLE,down,0,comm,&rq_s[1]);
		MPI_Isend(&TO(0,1),1,column,right,0,comm,&rq_s[2]);
		MPI_Isend(&TO(0,w-2),1,column,left,0,comm,&rq_s[3]);
	
	cout << "#[Jacobi_mpi] compute 7!" <<iter<< endl;

		MPI_Waitall(4, rq_r, statuses_r) ;
		MPI_Waitall(4, rq_s, statuses_s) ;
	
	cout << "#[Jacobi_mpi] compute 8!" <<iter<< endl;
		
		swap(A_from, A_to);
	}
	
	MPI_Barrier(comm);
	
	my_time_t tick_end;
	get_time ( tick_end );
	
	if ( debug & 4 ) {
		for ( int id=0; id<m_size; ++id) {
			MPI_Barrier(comm);
			if ( id == m_rank ) {
				for ( size_t yy=0; yy<h; ++yy) {
					cerr << '[' << yy << '(' << tile_x << ',' << tile_y << ")@END]\t";
					for ( size_t xx = 0; xx<w; ++xx ) {
						cerr << FROM(yy, xx) << '\t';
					}
					cerr << endl;
				}
				cerr << " ================================ " << endl;
			}
		}
	}
	
	delete[] A_from;
	delete[] A_to;
	
	unsigned long long time_usec = timing_diff_usec(tick_start, tick_end);
	
	unsigned long long max_time;
	MPI_Allreduce(&time_usec, &max_time, 1, MPI_UNSIGNED_LONG_LONG, MPI_MAX, comm);
	
	return max_time;
}

void one_run ( size_t width, size_t height, long nb_tiles_x, long nb_tiles_y )
{
	cout << "#[Jacobi_mpi] one_run!" << endl;
	unsigned long niter = 100000000LL / (100LL*slow_start+width*height);
	if ( niter<2 ) niter = 2;
	
	cout << "# height(nbdouble): " << height << "\t width(nbdouble): " << width << "\t niter: " << niter << endl;
	
	vector< uint64_t > durations;
	durations.reserve(nxp);
	for ( unsigned xpid = 0; xpid < nxp; ++xpid ) {
		durations.push_back(compute(width, height, niter, nb_tiles_x, nb_tiles_y));
	}
	sort(durations.begin(), durations.end());
	
	cout << "size(nbdouble): " << width*height;
	for ( unsigned decile = 1; decile<10; ++decile ) {
		unsigned decimal = (static_cast< uint64_t >(decile)*nxp-5)%10;
		unsigned rank = (static_cast< uint64_t >(decile)*nxp-5)/10;
		uint64_t duration = ((10-decimal)*durations[rank]+decimal*durations[rank+1])/10;
		cout << "\tD" << decile << "(ns): " << (1000*duration)/niter;
	}
	cout << endl;
}

size_t one_size ( size_t above, size_t below, long nb_tiles_x, long nb_tiles_y )
{
	cout << "#[Jacobi_mpi] one_size!" << endl;
	size_t step = 1;
	while ( step*step<=below ) step*=2; // lowest side size that doesn't fit in below (o)
	step /= 2; // highest side size that fit in below (o)
	step /= 4; // (4x4 == 16 == sizeof(double)x2) highest side size for 2 matrices to fit in below (nbdbl)
	step /= run_per_size; // get at least run_per_size run
	if ( step < 1 ) step = 1;
	
	size_t size = step;
	while ( size <= above ) size += step; // multiple of step above size
	
	cout << "# for " << size*size*sizeof(double)*2 << "o to " << below << "o step " << step*step*sizeof(double)*2 << "o" << endl;
	for ( ; size*size*sizeof(double)*2 <= below; size += step ) one_run(size, size, nb_tiles_x, nb_tiles_y); // in below
	size -= step;
	return size;
}

int main (int argc, char* argv[])
{
	cout << "#[Jacobi_mpi] go!" << endl;
	
	MPI_Init (&argc, &argv);	/* starts MPI */
	
	if ( argc != 3 ) {
		cout << "[Jacobi MPI] need 2 arguments: xtiles and ytiles (size of decomposition domain)" << endl;
		MPI_Abort( MPI_COMM_WORLD, 1 );
	}
	
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	unsigned nb_tiles_x = atoi(argv[1]);
	unsigned nb_tiles_y = atoi(argv[2]);
	
	uint32_t rps = run_per_size;
	run_per_size = 1;
	while ( run_per_size <= rps ) run_per_size *= 2;
	run_per_size /= 2;
	
	cout << "# L1 " << l1_size << "o == " << (l1_size/2/sizeof(double)) << " dbl/mat" << endl;
	size_t reached = one_size(nb_tiles_x*nb_tiles_y*2*sizeof(double)*4, l1_size, nb_tiles_x, nb_tiles_y);
	cout << "# L2 (" << l2_size << "o == " << (l2_size/2/sizeof(double)) << " dbl/mat" << ")" << endl;
	reached = one_size(reached, l2_size, nb_tiles_x, nb_tiles_y);
	cout << "# L3 (" << l3_size << "o == " << (l3_size/2/sizeof(double)) << " dbl/mat" << ")" << endl;
	reached = one_size(reached, l3_size, nb_tiles_x, nb_tiles_y);
	cout << "# RAM (" << ram_size << "o == " << (ram_size/2/sizeof(double)) << " dbl/mat" << ")" << endl;
	reached = one_size(reached, ram_size, nb_tiles_x, nb_tiles_y);
	cout << "# DONE (kthxbye)" << endl;

	MPI_Finalize();

}

// int main (int argc, char* argv[])
// {
// 	MPI_Init (&argc, &argv);	/* starts MPI */
// 	
// 	if ( argc != 3 ) {
// 		cout << "[Jacobi MPI] need 2 arguments: xtiles and ytiles (size of decomposition domain)" << endl;
// 		MPI_Abort( MPI_COMM_WORLD, 1 );
// 	}
// 	
// 	int rank;
// 	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
// 	
// 	unsigned xtiles = atoi(argv[1]);
// 	unsigned ytiles = atoi(argv[2]);
// 	
// 	for ( long ii = 4 ; ii < 15000 ; ) {
// 		if ( ii < 2048 ) {
// 			ii = ii*2;
// 		}
// 		else {
// 			ii = ii + 1000;
// 		}
// 		vector< uint64_t > durations;
// 		durations.reserve(nxp);
// 		for ( unsigned xpid = 0; xpid < nxp; ++xpid ) {
// 			durations.push_back(compute(ii, ii, niter, xtiles, ytiles));
// 		}
// 		sort(durations.begin(), durations.end());
// 		
// 		if ( rank == 0 ) {
// 			cout << "height(nbdouble): " << ii << "\t width(nbdouble): " << ii << "\t totsize(nbdouble): " << ii*ii;
// 			for ( unsigned decile = 1; decile<10; ++decile ) {
// 				unsigned decimal = (static_cast< uint64_t >(decile)*nxp-5)%10;
// 				unsigned rank = (static_cast< uint64_t >(decile)*nxp-5)/10;
// 				uint64_t duration = ((10-decimal)*durations[rank]+decimal*durations[rank+1])/10;
// 				cout << "\tD" << decile << "(us): " << duration;
// 			}
// 			cout << endl;
// 		}
// 	}
// 	MPI_Finalize();
// }
