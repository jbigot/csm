#include <algorithm>
#include <iostream>
#include <vector>

#include <omp.h>
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

uint64_t compute(long w, long h , long niter)
{
	double* A_from = new double[h*w];
	double* A_to = new double[h*w];
	
	// Middle Init
	for ( size_t ii=0; ii<h*w; ++ii ) {
		A_from[ii] = inside;
		A_to[ii] = inside;
	}
	
	// Left/Right borders
	for ( size_t yy=0; yy<h; ++yy ) {
		FROM(yy, 0) = left;
		TO(yy, 0) = left;
		FROM(yy, w-1) = right;
		TO(yy, w-1) = right;
	}
	
	for ( size_t xx=1; xx<w-1; ++xx ) {
		FROM(0, xx) = bottom;
		TO(0, xx) = bottom;
	}
	// Bottom corners
	FROM(0, 0) = (left+bottom)/2;
	TO(0, 0) = (left+bottom)/2;
	FROM(0, w-1) = (right+bottom)/2;
	TO(0, w-1) = (right+bottom)/2;

	for ( size_t xx=1; xx<w-1; ++xx ) {
		FROM(h-1, xx) = top;
		TO(h-1, xx) = top;
	}
	// Top corners
	FROM(h-1, 0) = (left+top)/2;
	TO(h-1, 0) = (left+top)/2;
	FROM(h-1, w-1) = (right+top)/2;
	TO(h-1, w-1) = (right+top)/2;
	
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
	
	my_time_t tick_start;
	get_time ( tick_start );
	
	for ( int iter=0; iter<niter; ++iter ) {
	
#ifndef NDEBUG
		if ( debug & 2 ) {
		for ( size_t yy=0; yy<h; ++yy) {
			cerr << '[' << yy << "@" << iter << "]\t";
			for ( size_t xx = 0; xx<w; ++xx ) {
				cerr << FROM(yy, xx) << '\t';
			}
			cerr << endl;
		}
		cerr << " ================================ " << endl;
		}
#endif //NDEBUG
		
#pragma omp parallel for
		for ( int yy = 1; yy < (h -1) ; ++yy ) {
			for ( int xx = 1; xx < (w - 1) ; ++xx ) {
				TO(yy, xx) = 0.25*( FROM(yy-1, xx) + FROM(yy+1, xx) + FROM(yy, xx-1) + FROM(yy, xx+1) );
			}
		}
		
		swap(A_from, A_to);
	}
	
	my_time_t tick_end;
	get_time ( tick_end );
	
	if ( debug & 4 ) {
		for ( size_t yy=0; yy<h; ++yy) {
			cerr << '[' << yy << "@END]\t";
			for ( size_t xx = 0; xx<w; ++xx ) {
				cerr << FROM(yy, xx) << '\t';
			}
			cerr << endl;
		}
		cerr << " ================================ " << endl;
	}
	
	delete[] A_from;
	delete[] A_to;
	
	return timing_diff_usec ( tick_start, tick_end );
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

int main ()
{
	cout << "#[Jacobi] go!" << endl;
	
// 	if ( nxp < 10 ) nxp = 10;
	uint32_t rps = run_per_size;
	run_per_size = 1;
	while ( run_per_size <= rps ) run_per_size *= 2;
	run_per_size /= 2;
	
	cout << "# L1 " << l1_size << "o == " << (l1_size/2/sizeof(double)) << " dbl/mat" << endl;
	size_t reached = one_size(0, l1_size);
	cout << "# L2 (" << l2_size << "o == " << (l2_size/2/sizeof(double)) << " dbl/mat" << ")" << endl;
	reached = one_size(reached, l2_size);
	cout << "# L3 (" << l3_size << "o == " << (l3_size/2/sizeof(double)) << " dbl/mat" << ")" << endl;
	reached = one_size(reached, l3_size);
	cout << "# RAM (" << ram_size << "o == " << (ram_size/2/sizeof(double)) << " dbl/mat" << ")" << endl;
	reached = one_size(reached, ram_size);
	cout << "# DONE (kthxbye)" << endl;

}


