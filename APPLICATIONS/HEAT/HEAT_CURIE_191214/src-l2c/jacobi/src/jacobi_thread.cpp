#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>

#include <boost/thread/barrier.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <omp.h>
#include <stdint.h>
#include <sys/time.h>
#include <time.h>

#include <arraypart.h>


typedef struct timeval my_time_t;
#define get_time(a) gettimeofday(&a, NULL)
#define timing_diff_usec(a,b) (((long long)(b).tv_sec - (a).tv_sec)*1000000+((long long)(b).tv_usec - (a).tv_usec))


#define FROM(y,x) A_from[ (x) + w * (y) ]
#define TO(y,x) A_to[ (x) + w * (y) ]
#define RESULT(y,x) A_result[ (x) + w * (y) ]


using ::boost::barrier;
using ::boost::condition_variable;
using ::boost::lock_guard;
using ::boost::mutex;
using ::boost::thread_group;
using ::boost::unique_lock;
using ::jacobi::ArrayPart;
using ::std::cerr;
using ::std::cout;
using ::std::endl;
using ::std::sort;
using ::std::strtol;
using ::std::swap;
using ::std::vector;


const uint16_t debug=0;

const uint32_t slow_start=44;
const uint64_t l1_size=1572864;
const uint64_t l2_size=6291456;
const uint64_t l3_size=12582912;
const uint64_t ram_size=8589934592;

uint32_t run_per_size=32;

const uint32_t niter=100;
const uint16_t nxp=10;

const double top=10;
const double bottom=0;
const double left=5;
const double right=5;
const double inside=-10;

size_t nb_tiles_x = 1;
size_t nb_tiles_y = 1;


void one_iter (  double* A_from, double* A_to, size_t w, size_t h, size_t tile_xx, size_t tile_yy, size_t tile_width, size_t tile_height )
{
	for ( int yy = tile_yy; yy < (tile_height + tile_yy) ; ++yy ) {
		for ( int xx = tile_xx; xx < (tile_width + tile_xx) ; ++xx ) {
			TO(yy, xx) = 0.25*( FROM(yy-1, xx) + FROM(yy+1, xx) + FROM(yy, xx-1) + FROM(yy, xx+1) );
		}
	}
}

void init ( ArrayPart< double > data, bool on_left, bool on_right, bool on_bottom, bool on_top )
{
	// Middle Init
	for ( size_t yy=0; yy<data.height; ++yy ) {
		for ( size_t xx=0; xx<data.width; ++xx ) {
			data[yy][xx] = inside;
		}
	}
	
	// Left border
	if ( on_left ) {
		for ( size_t yy=0; yy<data.height; ++yy ) {
			data[yy][0] = left;
		}
	}
	
	// Right border
	if ( on_right ) {
		for ( size_t yy=0; yy<data.height; ++yy ) {
			data[yy][data.width-1] = right;
		}
	}
	
	// Bottom border
	if ( on_bottom ) {
		for ( size_t xx=0; xx<data.width; ++xx ) {
			data[0][xx] = bottom;
		}
	}
	
	// Top border
	if ( on_top ) {
		for ( size_t xx=0; xx<data.width; ++xx ) {
			data[data.height-1][xx] = top;
		}
	}
	
	// Bottom/Left corner
	if ( on_bottom && on_left ) {
		data[0][0] = (left+bottom)/2;
	}
	
	// Bottom/Right corner
	if ( on_bottom && on_right ) {
		data[0][data.width-1] = (right+bottom)/2;
	}
	
	// Top/Left corners
	if ( on_top && on_left ) {
		data[data.height-1][0] = (left+top)/2;
	}
	
	// Top/Righ corners
	if ( on_top && on_right ) {
		data[data.height-1][data.width-1] = (right+top)/2;
	}
}

class Worker
{
public:
	barrier& iter_barrier;
	int id;
	
	mutex m_mutex;
	condition_variable m_cond;
	enum { STOPPED, INIT, INITIALIZED, RUN } m_status;
	
	double* m_A_from;
	double* m_A_to;
	size_t m_width;
	size_t m_height;
	size_t m_tile_x;
	size_t m_tile_y;
	size_t m_tile_width;
	size_t m_tile_height;
	long m_niter;

	//my_time_t tick_start, tick_end;

	Worker ( barrier& iter_barrier, int id ):
			iter_barrier(iter_barrier),
			m_status(STOPPED),
			id(id)
	{}
	
	void init ( double* A_from, double* A_to, const size_t width, const size_t height, const size_t tile_x, const size_t tile_y, const size_t tile_width, const size_t tile_height, long niter )
	{
		unique_lock<mutex> l(m_mutex);
		while ( m_status != STOPPED ) m_cond.wait(l);
		m_A_from = A_from;
		m_A_to = A_to;
		m_width = width;
		m_height = height;
		m_tile_x = tile_x;
		m_tile_y = tile_y;
		m_tile_width = tile_width;
		m_tile_height = tile_height;
		m_niter = niter;
		m_status = INIT;
		m_cond.notify_all();
	}
	
	void prepare_to_run ()
	{
		unique_lock<mutex> l(m_mutex);
		while ( m_status != INITIALIZED ) m_cond.wait(l);
	}
	
	void run ()
	{
		unique_lock<mutex> l(m_mutex);
		m_status = RUN;
		m_cond.notify_all();
	}
	
	void operator() ()
	{
		unique_lock<mutex> l(m_mutex);
		for (;;) {
			while ( m_status != INIT ) m_cond.wait(l);
#ifndef NDEBUG
		cerr << "Thread["<<m_tile_x<<", "<<m_tile_y<<"] init("<<m_A_from<<", "<<m_width<<", "<<m_height<<", "<<m_tile_x<<", "<<m_tile_y<<", "<<m_tile_width<<", "<<m_tile_height<<")"<<endl;
#endif // NDEBUG
			
			::init(
					ArrayPart< double >(m_A_from, m_width, m_height, m_tile_x, m_tile_y, m_tile_width, m_tile_height),
					m_tile_x==0,
					m_tile_x+m_tile_width==m_width,
					m_tile_y==0,
					m_tile_y+m_tile_height==m_height
			);
			
			::init(
					ArrayPart< double >(m_A_to, m_width, m_height, m_tile_x, m_tile_y, m_tile_width, m_tile_height),
					m_tile_x==0,
					m_tile_x+m_tile_width==m_width,
					m_tile_y==0,
					m_tile_y+m_tile_height==m_height
			);
			
			// Heating caches !
			iter_barrier.wait();
			for ( int iter=0; iter<std::min(10L, m_niter); ++iter ) {
				one_iter(m_A_from, m_A_to, m_width, m_height, m_tile_x+1, m_tile_y+1, m_tile_width-2, m_tile_height-2 );
				swap(m_A_from, m_A_to);
				iter_barrier.wait();
			}
			
			m_status = INITIALIZED;
			m_cond.notify_all();
			
			while ( m_status != RUN ) m_cond.wait(l);
			
			//get_time ( tick_start );
			
			for ( long iter=0; iter<m_niter; ++iter ) {
				one_iter(m_A_from, m_A_to, m_width, m_height, m_tile_x+1, m_tile_y+1, m_tile_width-2, m_tile_height-2 );
				swap(m_A_from, m_A_to);
				iter_barrier.wait();
			}
			//get_time ( tick_end );
			
			m_status = STOPPED;
			m_cond.notify_all();
			
		}
	}
	
	void join ()
	{
		unique_lock<mutex> l(m_mutex);
		while ( m_status != STOPPED ) m_cond.wait(l);
	}
	
	double* result ()
	{
		unique_lock<mutex> l(m_mutex);
		while ( m_status != STOPPED ) m_cond.wait(l);
		return m_A_from;
	}
};

class Workload
{
public:
	Worker* w;
	Workload(Worker* w): w(w) {}
	Workload(barrier& iter_barrier, int id): w(new Worker(iter_barrier, id)) {}
	void operator() () { (*w)(); }
};

thread_group threads;

barrier iter_barrier(nb_tiles_x*nb_tiles_y);

vector< Worker* > m_workers;

uint64_t compute ( size_t w, size_t h, unsigned long niter )
{
	double* A_from = new double[h*w];
	double* A_to = new double[h*w];
	
	if ( threads.size() == 0 ) {
		for ( int ii=0; ii<nb_tiles_x*nb_tiles_y; ++ii) {
			Worker* w = new Worker(iter_barrier, ii);
			threads.create_thread< Workload >(Workload(w));
			m_workers.push_back(w);
		}
	}
	
	size_t tile_height = (h-2)/nb_tiles_y;
	size_t tile_width = (w-2)/nb_tiles_x;
	
	vector< Worker* >::iterator it = m_workers.begin();
	for ( size_t tile_y = 0; tile_y < nb_tiles_y; ++ tile_y ) {
		for ( size_t tile_x = 0; tile_x < nb_tiles_x; ++ tile_x ) {
			size_t local_width = tile_width;
			if ( tile_x == nb_tiles_x-1 ) local_width = w-2 - tile_x*tile_width;
			size_t local_height = tile_height;
			if ( tile_y == nb_tiles_y-1 ) local_height = h-2 - tile_y*tile_height;
			(*it)->init(A_from, A_to, w, h, tile_x*tile_width, tile_y*tile_height, local_width+2, local_height+2, niter);
			++it;
		}
	}
	
	if ( debug & 1 ) {
		for ( size_t yy=0; yy<h; ++yy) {
			cerr << '[' << yy << "@START]\t";
			for ( size_t xx = 0; xx<w; ++xx ) {
				cerr << FROM(yy, xx) << '\t';
			}
			cerr << endl;
		}
		cerr << " ================================ " << endl;
	}
	
	for ( vector< Worker* >::iterator it = m_workers.begin(); it != m_workers.end(); ++it ) (*it)->prepare_to_run();

	my_time_t tick_start;
	get_time ( tick_start );
	
	for ( vector< Worker* >::iterator it = m_workers.begin(); it != m_workers.end(); ++it ) (*it)->run();
	for ( vector< Worker* >::iterator it = m_workers.begin(); it != m_workers.end(); ++it ) (*it)->join();
	
	my_time_t tick_end;
	get_time ( tick_end );
	
	double* A_result = m_workers[0]->result();
	
	if ( debug & 4 ) {
		for ( size_t yy=0; yy<h; ++yy) {
			cerr << '[' << yy << "@END]\t";
			for ( size_t xx = 0; xx<w; ++xx ) {
				cerr << RESULT(yy, xx) << '\t';
			}
			cerr << endl;
		}
		cerr << " ================================ " << endl;
	}
	
	delete[] A_from;
	delete[] A_to;
	
	return timing_diff_usec(tick_start, tick_end);
}

void one_run ( size_t width, size_t height )
{
	unsigned long niter = 100000000LL / (100LL*slow_start+width*height);
	if ( niter<2 ) niter = 2;
	
	cout << "# height(nbdouble): " << height << "\t width(nbdouble): " << width << "\t niter: " << niter << endl;
	
	vector< uint64_t > durations;
	durations.reserve(nxp);
	for ( unsigned xpid = 0; xpid < nxp; ++xpid ) {
		durations.push_back(compute(width, height, niter));
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

size_t one_size ( size_t above, size_t below )
{
	size_t step = 1;
	while ( step*step<=below ) step*=2; // lowest side size that doesn't fit in below (o)
	step /= 2; // highest side size that fit in below (o)
	step /= 4; // (4x4 == 16 == sizeof(double)x2) highest side size for 2 matrices to fit in below (nbdbl)
	step /= run_per_size; // get at least run_per_size run
	if ( step < 1 ) step = 1;
	
	size_t size = step;
	while ( size <= above ) size += step; // multiple of step above size
	
	cout << "# for " << size*size*sizeof(double)*2 << "o to " << below << "o step " << step*step*sizeof(double)*2 << "o" << endl;
	for ( ; size*size*sizeof(double)*2 <= below; size += step ) one_run(size, size); // in below
	size -= step;
	return size;
}

void usage ( const char* progname )
{
	cout << "usage: "<<progname<<" [nb_tiles_x] [nb_tiles_y]"<<endl;
	exit(1);
}

int main ( int argc, char* argv[] )
{
	if ( argc > 3 ) {
		usage(argv[0]);
	}
	
	if ( argc > 1 ) {
		char* end = argv[1];
		long ntx = strtol(argv[1], &end, 0);
		if ( *end || ntx<=0 ) usage(argv[0]);
		nb_tiles_x = ntx;
	}
	
	if ( argc > 2 ) {
		char* end = argv[2];
		long nty = strtol(argv[2], &end, 0);
		if ( *end || nty<=0 ) usage(argv[0]);
		nb_tiles_y = nty;
	}
	
	cout << "#[JacobiThread] go! ("<<nb_tiles_x<<" x tiles, "<<nb_tiles_y<<"y tiles)"<< endl;
	
// 	if ( nxp < 10 ) nxp = 10;
	uint32_t rps = run_per_size;
	run_per_size = 1;
	while ( run_per_size <= rps ) run_per_size *= 2;
	run_per_size /= 2;
	
	cout << "# L1 " << l1_size << "o == " << (l1_size/2/sizeof(double)) << " dbl/mat" << endl;
	size_t reached = one_size(nb_tiles_x*nb_tiles_y*2*sizeof(double)*4, l1_size);
	cout << "# L2 (" << l2_size << "o == " << (l2_size/2/sizeof(double)) << " dbl/mat" << ")" << endl;
	reached = one_size(reached, l2_size);
	cout << "# L3 (" << l3_size << "o == " << (l3_size/2/sizeof(double)) << " dbl/mat" << ")" << endl;
	reached = one_size(reached, l3_size);
	cout << "# RAM (" << ram_size << "o == " << (ram_size/2/sizeof(double)) << " dbl/mat" << ")" << endl;
	reached = one_size(reached, ram_size);
	cout << "# DONE (kthxbye)" << endl;

}


