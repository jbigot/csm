#include <iostream>
#include <pthread.h>

#include <llcmcpp/llcmcpp3.h>
#include <stdint.h>
#include "exchange.h"

#define TO(y,x) A_to[ (x) + w * (y) ]

using ::std::cerr;
using ::std::cout;
using ::std::endl;


namespace jacobi
{

namespace impl
{

class PthreadConnector :
			virtual public Exchange
{
private:
	pthread_mutex_t mtx;
	pthread_cond_t cnd;
	bool done_receive;
	bool ready_to_receive;
	bool done_send;
	bool ready_to_send;

	double* dst;
	double* src;

	void _do_copy()
	{
		//TODO: handle copy if necessary
	}

public:

	PthreadConnector() :
			done_receive ( false ),
			ready_to_receive ( false ),
			done_send ( false ),
			ready_to_send ( false )
	{
		pthread_mutex_init ( &mtx, NULL );
		pthread_cond_init ( &cnd, NULL );
	}
	
	virtual void recv_ready ( double* A_to, const size_t width, const size_t height, const size_t tile_xx, const size_t tile_yy, const size_t tile_width, const size_t tile_height )
	{
		pthread_mutex_lock ( &mtx );
		ready_to_receive=true;
		dst=A_to;
		if ( ready_to_receive && ready_to_send ) {
			_do_copy();
			done_receive=done_send=true;
			pthread_cond_signal ( &cnd );
		}
		pthread_mutex_unlock ( &mtx );
	};

	virtual void send_ready ( double* A_to, const size_t width, const size_t height, const size_t tile_xx, const size_t tile_yy, const size_t tile_width, const size_t tile_height )
	{
		pthread_mutex_lock ( &mtx );
		ready_to_send=true;
		src=A_to;
		if ( ready_to_receive && ready_to_send ) {
			_do_copy();
			done_receive=done_send=true;
			pthread_cond_signal ( &cnd );
		}
		pthread_cond_signal ( &cnd );
		pthread_mutex_unlock ( &mtx );
		
	};
	
	virtual void recv_wait()
	{
		pthread_mutex_lock ( &mtx );
		if ( !done_receive )  {
#ifndef NDEBUG
		  cerr << "[PthreadConnector] recv wait going to block\n";
#endif //NDEBUG
		  pthread_cond_wait ( &cnd, &mtx );
#ifndef NDEBUG
		  cerr << "[PthreadConnector] recv wait going to block\n";
#endif //NDEBUG
		}
		ready_to_receive=false;
		done_receive=false;
		pthread_mutex_unlock ( &mtx );
	};

	virtual void send_wait()
	{
		pthread_mutex_lock ( &mtx );
		if ( !done_send ) {
#ifndef NDEBUG
		  cerr << "[PthreadConnector] send wait going to block\n";
#endif //NDEBUG
		  pthread_cond_wait ( &cnd, &mtx );
#ifndef NDEBUG
		  cerr << "[PthreadConnector] send wait unblock\n";
#endif //NDEBUG
		}
		ready_to_send=false;
		done_send=false;
		pthread_mutex_unlock ( &mtx );
	};

};

LCMP ( PthreadConnector )
	L_CPP_PROVIDE ( Exchange, exchange )
LEND

}

}
