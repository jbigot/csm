#include <iostream>

#include <llcmcpp/llcmcpp3.h>
#include <stdint.h>
#include <sys/time.h>
#include <time.h>

#include "XPcomputer.h"
#include "checkdata.h"

typedef struct timeval my_time_t;
#define get_time(a) gettimeofday(&a, NULL)
#define timing_diff_usec(a,b) (((long long)(b).tv_sec - (a).tv_sec)*1000000+((long long)(b).tv_usec - (a).tv_usec))


#define FROM(y,x) A_from[ (x) + w * (y) ]
#define TO(y,x) A_to[ (x) + w * (y) ]


using ::std::cerr;
using ::std::cout;
using ::std::endl;
using ::std::swap;

namespace jacobi {

namespace impl {

class JacobiSeq:
		virtual public XPComputer
{
public:
	uint8_t debug;
	
	double top, bottom, left, right, inside;
	
	CheckData* checkdata;
	
	JacobiSeq ():
			debug(0xFF),
			top(10),
			bottom(0),
			left(5),
			right(5),
			inside(-10),
			checkdata(0)
	{
	}

	uint64_t compute(size_t w, size_t h , unsigned long niter)
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
		
		for ( int iter=0; iter<std::min(10UL, niter); ++iter ) {
			for ( int yy = 1; yy < (h -1) ; ++yy ) {
			  for ( int xx = 1; xx < (w - 1) ; ++xx ) {
			    TO(yy, xx) = 0.25*( FROM(yy-1, xx) + FROM(yy+1, xx) + FROM(yy, xx-1) + FROM(yy, xx+1) );
			  }
			}
			swap(A_from, A_to);
		}
		
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
		
		if (checkdata) checkdata->compare(ArrayPart<double>(A_from, w, h, 1, 1, w-2, h-2), "JacobiSeq");
		
		delete[] A_from;
		delete[] A_to;
		
		return timing_diff_usec ( tick_start, tick_end );
	}
	
};

LCMP(JacobiSeq)
	L_CPP_PROVIDE(XPComputer, xpcomputer)
	L_CPP_USE(CheckData, checkdata)
	L_PROPERTY(uint8_t, debug)
	L_PROPERTY(double, top)
	L_PROPERTY(double, bottom)
	L_PROPERTY(double, left)
	L_PROPERTY(double, right)
	L_PROPERTY(double, inside)
LEND

}

}
