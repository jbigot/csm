#include <algorithm>
#include <iostream>

#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/collective.h>
#include <sys/time.h>
#include <time.h>

#include <computerniter.h>
#include <exchange.h>
#include <computer.h>
#include <stdint.h>
#include <initializer.h>


typedef struct timeval my_time_t;
#define get_time(a) gettimeofday(&a, NULL)
#define timing_diff_usec(a,b) (((long long)(b).tv_sec - (a).tv_sec)*1000000+((long long)(b).tv_usec - (a).tv_usec))

#define FROM(x,y) A_from[ (x) + width * (y) ]
#define TO(x,y) A_to[ (x) + width * (y) ]

using ::std::cerr;
using ::std::cout;
using ::std::endl;
using ::std::min;
using ::std::swap;
using ::llcmcpp::Collective;

namespace jacobi
{

namespace impl {

class JacobiCoreNiter :
			virtual public ComputerNiter
{
public:

	uint8_t debug;

	uint16_t nb_tiles_y, nb_tiles_x, tile_x, tile_y;

	Exchange* exchangeUp;
	Exchange* exchangeDown;
	Exchange* exchangeLeft;
	Exchange* exchangeRight;

	Computer* computer;

	Initializer* initialization;

	Collective* collective;

	JacobiCoreNiter() :
			debug(0),
			nb_tiles_x(0),
			nb_tiles_y(0),
			tile_x(0),
			tile_y(0),
			exchangeUp(0),
			exchangeDown(0),
			exchangeLeft(0),
			exchangeRight(0),
			computer(0),
			initialization(0),
			collective(0) {
	}

	virtual uint64_t n_iter(double* A_from, double* A_to, const size_t width, const size_t height, const size_t tile_xx, const size_t tile_yy, const size_t tile_width, const size_t tile_height, long niter) {
#ifndef NDEBUG
		cerr << "[JacobiCoreNiter("<<tile_x<<","<<tile_y<<")] n_iter("<<A_from<<", "<<A_to<<", "<<width<<", "<<height<<", "<<tile_xx<<", "<<tile_yy<<", "<<tile_width<<", "<<tile_height<<", "<<niter<<')' << endl;
#endif //NDEBUG

		initialization->init(ArrayPart< double > (A_from, width, height, tile_xx, tile_yy, tile_width, tile_height), tile_x==0, tile_x == nb_tiles_x-1, tile_y == 0, tile_y == nb_tiles_y-1);
		initialization->init(ArrayPart< double > (A_to, width, height, tile_xx, tile_yy, tile_width, tile_height), tile_x==0, tile_x == nb_tiles_x-1, tile_y == 0, tile_y == nb_tiles_y-1);

#ifndef NDEBUG
		cerr << "[JacobiCoreNiter("<<tile_x<<","<<tile_y<<")] n_iter INITIALIZED"<< endl;
#endif //NDEBUG

		if (debug & 1) {
			for (size_t yy=0; yy<height; ++yy) {
				cerr << '[' << yy << '(' << tile_x << ',' << tile_y << ")@START]\t";
				for (size_t xx = 0; xx<width; ++xx) {
					cerr << FROM(yy, xx) << '\t';
				}
				cerr << endl;
			}
			cerr << " ================================ " << endl;
		}

		if (collective) collective->barrier();

#ifndef NDEBUG
		cerr << "[JacobiCoreNiter("<<tile_x<<","<<tile_y<<")] n_iter 1st barrier done"<< endl;
#endif //NDEBUG

		// heating cache
		for (int iter=0; iter<min(10L, niter); ++iter) {
			if (exchangeUp) exchangeUp->recv_ready(A_to, width, height, tile_xx, tile_yy+tile_height-1, tile_width, 1);
			if (exchangeDown) exchangeDown->recv_ready(A_to, width, height, tile_xx, tile_yy, tile_width, 1);
			if (exchangeLeft) exchangeLeft->recv_ready(A_to, width, height, tile_xx, tile_yy, 1, tile_height);
			if (exchangeRight) exchangeRight->recv_ready(A_to, width, height, tile_xx+tile_width-1, tile_yy, 1, tile_height);
			
			computer->one_iter(A_from, A_to, width, height, tile_xx+1, tile_yy+1, tile_width-2, tile_height-2);
			
			if (exchangeUp) exchangeUp->send_ready(A_to, width, height, tile_xx, tile_yy+tile_height-2, tile_width, 1);
			if (exchangeDown) exchangeDown->send_ready(A_to, width, height, tile_xx, tile_yy+1, tile_width, 1);
			if (exchangeLeft) exchangeLeft->send_ready(A_to, width, height, tile_xx+1, tile_yy, 1, tile_height);
			if (exchangeRight) exchangeRight->send_ready(A_to, width, height, tile_xx+tile_width-2, tile_yy, 1, tile_height);
			
			if (exchangeUp) exchangeUp->recv_wait();
			if (exchangeDown) exchangeDown->recv_wait();
			if (exchangeLeft) exchangeLeft->recv_wait();
			if (exchangeRight) exchangeRight->recv_wait();
			
			if (exchangeUp) exchangeUp->send_wait();
			if (exchangeDown) exchangeDown->send_wait();
			if (exchangeLeft) exchangeLeft->send_wait();
			if (exchangeRight) exchangeRight->send_wait();
			
			swap(A_from, A_to);
		}

		if (collective) collective->barrier();

		my_time_t tick_start;
		get_time(tick_start);

		for (int iter=0; iter<niter; ++iter) {

#ifndef NDEBUG
			if (debug & 2) {
				for (size_t yy=0; yy<tile_height; ++yy) {
					cerr << '[' << yy << '(' << tile_xx << ',' << tile_yy << ")@" << iter << "]\t";
					for (size_t xx = 0; xx<tile_width; ++xx) {
						cerr << FROM(yy, xx) << '\t';
					}
					cerr << endl;
				}
				cerr << " ================================ " << endl;
			}
#endif //NDEBUG
			if (exchangeUp) exchangeUp->recv_ready(A_to, width, height, tile_xx, tile_yy+tile_height-1, tile_width, 1);
			if (exchangeDown) exchangeDown->recv_ready(A_to, width, height, tile_xx, tile_yy, tile_width, 1);
			if (exchangeLeft) exchangeLeft->recv_ready(A_to, width, height, tile_xx, tile_yy, 1, tile_height);
			if (exchangeRight) exchangeRight->recv_ready(A_to, width, height, tile_xx+tile_width-1, tile_yy, 1, tile_height);

#ifndef NDEBUG
			if (debug & 2) {
				for (size_t yy=0; yy<tile_height; ++yy) {
					cerr << '[' << yy << '(' << tile_xx << ',' << tile_yy << ")@" << iter << "]\t";
					for (size_t xx = 0; xx<tile_width; ++xx) {
						cerr << FROM(yy, xx) << '\t';
					}
					cerr << endl;
				}
				cerr << " before iter ================================ " << endl;
			}
#endif //NDEBUG
			// compute core
			computer->one_iter(A_from, A_to, width, height, tile_xx+1, tile_yy+1, tile_width-2, tile_height-2);

#ifndef NDEBUG
			if (debug & 2) {
				for (size_t yy=0; yy<tile_height; ++yy) {
					cerr << '[' << yy << '(' << tile_xx << ',' << tile_yy << ")@" << iter << "]\t";
					for (size_t xx = 0; xx<tile_width; ++xx) {
						cerr << TO(yy, xx) << '\t';
					}
					cerr << endl;
				}
				cerr << " after iter ================================ " << endl;
			}
#endif //NDEBUG

			if (exchangeUp) exchangeUp->send_ready(A_to, width, height, tile_xx, tile_yy+tile_height-2, tile_width, 1);
			if (exchangeDown) exchangeDown->send_ready(A_to, width, height, tile_xx, tile_yy+1, tile_width, 1);
			if (exchangeLeft) exchangeLeft->send_ready(A_to, width, height, tile_xx+1, tile_yy, 1, tile_height);
			if (exchangeRight) exchangeRight->send_ready(A_to, width, height, tile_xx+tile_width-2, tile_yy, 1, tile_height);

			if (exchangeUp) exchangeUp->recv_wait();
			if (exchangeDown) exchangeDown->recv_wait();
			if (exchangeLeft) exchangeLeft->recv_wait();
			if (exchangeRight) exchangeRight->recv_wait();

			if (exchangeUp) exchangeUp->send_wait();
			if (exchangeDown) exchangeDown->send_wait();
			if (exchangeLeft) exchangeLeft->send_wait();
			if (exchangeRight) exchangeRight->send_wait();

			swap(A_from, A_to);
		}


		if (collective) collective->barrier();

		my_time_t tick_end;
		get_time(tick_end);

		if (debug & 4) {
			for (size_t yy=0; yy<height; ++yy) {
				cerr << '[' << yy << '(' << tile_x << ',' << tile_y << ")@END]\t";
				for (size_t xx = 0; xx<width; ++xx) {
					cerr << FROM(yy, xx) << '\t';
				}
				cerr << endl;
			}
			cerr << " ================================ " << endl;
		}

		return timing_diff_usec(tick_start, tick_end);
	}

};

LCMP(JacobiCoreNiter)
	L_CPP_PROVIDE(ComputerNiter, computerniter)
	L_CPP_USE(Computer, computer);
	L_CPP_USE(Exchange, exchangeUp);
	L_CPP_USE(Exchange, exchangeDown);
	L_CPP_USE(Exchange, exchangeLeft);
	L_CPP_USE(Exchange, exchangeRight);
	L_CPP_USE(Initializer, initialization);
	L_CPP_USE(Collective, collective);
	L_PROPERTY(uint8_t, debug);
	L_PROPERTY(uint16_t, nb_tiles_y);
	L_PROPERTY(uint16_t, nb_tiles_x);
	L_PROPERTY(uint16_t, tile_x);
	L_PROPERTY(uint16_t, tile_y);
LEND

}

}
