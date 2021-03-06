#include <iostream>

#include <llcmcpp/llcmcpp3.h>
#include <stdint.h>
#include <sys/time.h>
#include <time.h>
#include <llcmcpp/collective.h>


#include <exchange.h>
#include <computer.h>
#include <initializer.h>
#include <XPcomputer.h>
#include <chronometer.hpp>

typedef struct timeval my_time_t;
#define get_time(a) gettimeofday(&a, NULL)
#define timing_diff_usec(a,b) (((long long)(b).tv_sec - (a).tv_sec)*1000000+((long long)(b).tv_usec - (a).tv_usec))


#define FROM(y,x) A_from[ (x) + w * (y) ]
#define TO(y,x) A_to[ (x) + w * (y) ]

using ::std::cerr;
using ::std::cout;
using ::std::endl;
using ::std::swap;
using ::llcmcpp::Collective;

namespace jacobi {

namespace impl {

class Jacobi4Connectors:
	virtual public XPComputer
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
	
	Jacobi4Connectors ():
			debug(0xFF),
			nb_tiles_y(0),
			nb_tiles_x(0),
			tile_x(0),
			tile_y(0),
			collective(0),
			computer(0),
			exchangeUp(0),
			exchangeDown(0),
			exchangeLeft(0),
			exchangeRight(0),
			initialization(0)
	{
	}

	uint64_t compute ( size_t global_rwidth, size_t global_rheight, unsigned long niter )
	{
#ifndef NDEBUG
	  //cerr << "[MPIDriver] compute(" << global_rwidth << ", " << global_rheight << ", " << niter << ')' << endl;
#endif //NDEBUG

		size_t w = ((global_rwidth-2)/nb_tiles_x)+2;
		size_t h = ((global_rheight-2)/nb_tiles_y)+2;
		if ( tile_x == nb_tiles_x-1 ) w = global_rwidth - tile_x*(w-2);
		
		if ( tile_y == nb_tiles_y-1 ) h = global_rheight - tile_y*(h-2);
		
		double* A_from = new double[h*w];
		double* A_to = new double[h*w];
		
		initialization->init(ArrayPart< double >(A_from, w, h, 0, 0, w, h), tile_x==0, tile_x == nb_tiles_x-1, tile_y == 0, tile_y == nb_tiles_y-1 );
		initialization->init(ArrayPart< double >(A_to, w, h, 0, 0, w, h), tile_x==0, tile_x == nb_tiles_x-1, tile_y == 0, tile_y == nb_tiles_y-1);
		
		if ( debug & 1 ) {
					for ( size_t yy=0; yy<h; ++yy) {
						cerr << '[' << yy << '(' << tile_x << ',' << tile_y << ")@START]\t";
						for ( size_t xx = 0; xx<w; ++xx ) {
							cerr << FROM(yy, xx) << '\t';
						}
						cerr << endl;
					}
					cerr << " ================================ " << endl;
		}
		
		if (collective) collective->barrier();
		
		my_time_t tick_start;
		get_time ( tick_start );
		
		//-----Time calculation
		Chronometer chrono;
		chrono.start();
		//-----

		for ( int iter=0; iter<5000/*niter*/; ++iter ) {
			
		  /*#ifndef NDEBUG
			if ( debug & 2 ) {
						for ( size_t yy=0; yy<h; ++yy) {
							cerr << '[' << yy << '(' << tile_x << ',' << tile_y << ")@" << iter << "]\t";
							for ( size_t xx = 0; xx<w; ++xx ) {
								cerr << FROM(yy, xx) << '\t';
							}
							cerr << endl;
						}
						cerr << " ================================ " << endl;
			}
			#endif //NDEBUG*/
			if (exchangeUp) exchangeUp->recv_ready(A_to, w, h, 0, 0, w, h);
			if (exchangeDown) exchangeDown->recv_ready(A_to, w, h, 0, 0, w, h);
			if (exchangeLeft) exchangeLeft->recv_ready(A_to, w, h, 0, 0, w, h);
			if (exchangeRight) exchangeRight->recv_ready(A_to, w, h, 0, 0, w, h);
			
			// compute core
			computer->one_iter(A_from, A_to, w, h, 1, 1, w-2, h-2);
			
			if (exchangeUp) exchangeUp->send_ready(A_to, w, h, 0, 0, w, h);
			if (exchangeDown) exchangeDown->send_ready(A_to, w, h, 0, 0, w, h);
			if (exchangeLeft) exchangeLeft->send_ready(A_to, w, h, 0, 0, w, h);
			if (exchangeRight) exchangeRight->send_ready(A_to, w, h, 0, 0, w, h);

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

		//-----Time calculation
		chrono.stop();
		cout<<" Time TOTAL CALCULATION : "<<chrono.dureeCalcule()<<endl;
		//-----
		
		if (collective) collective->barrier();

		my_time_t tick_end;
		get_time ( tick_end );
		
		if ( debug & 4 ) {
					for ( size_t yy=0; yy<h; ++yy) {
						cerr << '[' << yy << '(' << tile_x << ',' << tile_y << ")@END]\t";
						for ( size_t xx = 0; xx<w; ++xx ) {
							cerr << FROM(yy, xx) << '\t';
						}
						cerr << endl;
					}
					cerr << " ================================ " << endl;
		}
		
		delete[] A_to;
		delete[] A_from;
		
		return timing_diff_usec ( tick_start, tick_end );

	}
	
};

LCMP(Jacobi4Connectors)
	L_CPP_PROVIDE(XPComputer, xpcomputer);
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
