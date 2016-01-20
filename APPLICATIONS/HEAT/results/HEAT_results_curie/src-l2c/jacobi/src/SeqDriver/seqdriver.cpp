#include <iostream>
#include <algorithm>

#include <llcmcpp/llcmcpp3.h>
#include <stdint.h>
#include <sys/time.h>
#include <time.h>

#include <checkdata.h>
#include <computer.h>
#include <initializer.h>
#include <XPcomputer.h>


typedef struct timeval my_time_t;
#define get_time(a) gettimeofday(&a, NULL)
#define timing_diff_usec(a,b) (((long long)(b).tv_sec - (a).tv_sec)*1000000+((long long)(b).tv_usec - (a).tv_usec))
#define timing_usec(a) (((long long)(a).tv_sec)*1000000+((long long)(a).tv_usec))

#define FROM(y,x) A_from[ (x) + w * (y) ]
#define TO(y,x) A_to[ (x) + w * (y) ]


using ::std::cerr;
using ::std::cout;
using ::std::endl;
using ::std::swap;

namespace jacobi {

namespace impl {

class SeqDriver:
		virtual public XPComputer
{
public:
	uint8_t debug;
	uint8_t heating;
	
	CheckData* checkdata;
	
	Computer* computer;
	
	Initializer* initialization;

	SeqDriver ():
			debug(0xFF),
			heating(1),
			checkdata(0),
			computer(0),
			initialization(0)
	{
	}

	uint64_t compute(size_t w, size_t h , unsigned long niter)
	{
		if ( debug & 8 ) {
			cout << "[SeqDriver] compute(" << w << ", " << h << ", " << niter << ")!" << endl;
		}
		
		double* A_from = new double[h*w];
		double* A_to = new double[h*w];
		
		// matrix initialization
		initialization->init(ArrayPart< double >(A_from, w, h, 0, 0, w, h), true, true, true, true);
		initialization->init(ArrayPart< double >(A_to, w, h, 0, 0, w, h), true, true, true, true);
		
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
		
		// Heating caches !
		if (heating)
			for ( int iter=0; iter<std::min(10UL, niter); ++iter ) {
				computer->one_iter(A_from, A_to, w, h, 1, 1, w-2, h-2);
				swap(A_from, A_to);
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
			
			computer->one_iter(A_from, A_to, w, h, 1, 1, w-2, h-2);
			
			swap(A_from, A_to);
		}
		
		my_time_t tick_end;
		get_time ( tick_end );
		

		//		cerr << "Seq ts="<<timing_usec(tick_start)<<" te="<<timing_usec(tick_end)<<" tt="<<timing_diff_usec(tick_start, tick_end)<<endl;

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
		
		if (checkdata) checkdata->compare(ArrayPart<double>(A_from, w, h, 1, 1, w-2, h-2), "SeqDriver");
		
		delete[] A_from;
		delete[] A_to;
		
		return timing_diff_usec ( tick_start, tick_end );
	}
	
};

LCMP(SeqDriver)
	L_PROPERTY(uint8_t, debug);
	L_PROPERTY(uint8_t, heating);
	L_CPP_PROVIDE(XPComputer, xpcomputer);
	L_CPP_USE(CheckData, checkdata);
	L_CPP_USE(Computer, computer);
	L_CPP_USE(Initializer, initialization);
LEND

}

}
