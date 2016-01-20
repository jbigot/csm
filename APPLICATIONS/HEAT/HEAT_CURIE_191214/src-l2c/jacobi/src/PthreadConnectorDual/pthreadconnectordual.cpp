#include <iostream>
#include <pthread.h>

#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/lifecycle.h>
#include <stdint.h>
#include "exchange.h"

#define TO(y,x) A_to[ (x) + w * (y) ]

using ::std::cerr;
using ::std::cout;
using ::std::endl;
using ::llcmcpp::LifeCycle;

namespace jacobi
{

namespace impl
{

  class PthreadConnectorDual : virtual public LifeCycle
{

  struct MailBox {
	 pthread_mutex_t mtx;
	 pthread_cond_t cnd;
	 bool done_receive;
	 bool ready_to_receive;
	 bool done_send;
	 bool ready_to_send;

	 double* dst;
	 double* src;

	 std::string name;

	 MailBox() :
		done_receive ( false ),
		ready_to_receive ( false ),
		done_send ( false ),
		ready_to_send ( false )
	 {
		pthread_mutex_init ( &mtx, NULL );
		pthread_cond_init ( &cnd, NULL );
	 }

	 void setname(std::string& n) { name = n; }

	 inline void _do_copy()
	 {
		if ( ready_to_receive && ready_to_send ) {
			done_receive=true;
			done_send=true;
			ready_to_receive=false;
			ready_to_send=false;
#ifndef NDEBUG
			//cerr << "[PthreadConnector-"<<name<<"] broadcast\n";
#endif //NDEBUG
			pthread_cond_broadcast ( &cnd );
		}
	 }

	 inline void recv_ready ( double* A_to, const size_t width, const size_t height, const size_t tile_xx, const size_t tile_yy, const size_t tile_width, const size_t tile_height )
	 {
		pthread_mutex_lock ( &mtx );
		ready_to_receive=true;
		dst=A_to;
		_do_copy();
		pthread_mutex_unlock ( &mtx );
	 }

	 virtual void send_ready ( double* A_to, const size_t width, const size_t height, const size_t tile_xx, const size_t tile_yy, const size_t tile_width, const size_t tile_height )
	 {
		pthread_mutex_lock ( &mtx );
		ready_to_send=true;
		src=A_to;
		_do_copy();
		pthread_mutex_unlock ( &mtx );		
	}
	
	 virtual void recv_wait()
	 {
		pthread_mutex_lock ( &mtx );
		while ( !done_receive )  {
#ifndef NDEBUG
		//cerr << "[PthreadConnector-"<<name<<"] recv wait going to block\n";
#endif //NDEBUG
		  pthread_cond_wait ( &cnd, &mtx );
#ifndef NDEBUG
		//cerr << "[PthreadConnector-"<<name<<"] recv wait unblock\n";
#endif //NDEBUG
		} /*else {
#ifndef NDEBUG
		  cerr << "[PthreadConnector-"<<name<<"] recv wait no block\n";
#endif //NDEBUG
		}*/
		done_receive=false;
		pthread_mutex_unlock ( &mtx );
	 }
	 
	 virtual void send_wait()
	 {
		pthread_mutex_lock ( &mtx );
		while ( !done_send ) {
#ifndef NDEBUG
		//cerr << "[PthreadConnector-"<<name<<"] send wait going to block\n";
#endif //NDEBUG
		  pthread_cond_wait ( &cnd, &mtx );
#ifndef NDEBUG
		//cerr << "[PthreadConnector-"<<name<<"] send wait unblock\n";
#endif //NDEBUG
		} /*else {
#ifndef NDEBUG
		  cerr << "[PthreadConnector-"<<name<<"] send wait no block\n";
#endif //NDEBUG
		}*/
		done_send=false;
		pthread_mutex_unlock ( &mtx );
	 }

  };

  class PthreadConnectorInternal : virtual public Exchange {
  private:
	 MailBox& mbs; // sender
	 MailBox& mbr; // receiver

  public:
	 
	 PthreadConnectorInternal(MailBox& imbs, MailBox& imbr) : mbs(imbs), mbr(imbr)
	 {} 
	
	 virtual void recv_ready ( double* A_to, const size_t width, const size_t height, const size_t tile_xx, const size_t tile_yy, const size_t tile_width, const size_t tile_height )
	 {
		mbr.recv_ready(A_to, width, height, tile_xx, tile_yy, tile_width, tile_height);
	 }

	 virtual void recv_wait() {
		mbr.recv_wait();
	 }

	 virtual void send_ready ( double* A_to, const size_t width, const size_t height, const size_t tile_xx, const size_t tile_yy, const size_t tile_width, const size_t tile_height )
	 {
		mbs.send_ready (A_to, width, height, tile_xx, tile_yy, tile_width, tile_height );
	 }

	 virtual void send_wait() {
		mbs.send_wait();
	 }
	 
  };

  // LifeCycle
  void configure_complete() 
  {
	 std::string tmp;
	 tmp = name + "_mb1";
	 mb1.setname(tmp);
	 tmp = name + "_mb2";
	 mb2.setname(tmp);
  }


  MailBox mb1; // from 1 to 2
  MailBox mb2; // from 2 to 1

public:
  PthreadConnectorInternal exchange1;
  PthreadConnectorInternal exchange2;
  
  std::string name;

  PthreadConnectorDual() : exchange1(mb1, mb2), exchange2(mb2, mb1)
  {
  }

};

LCMP ( PthreadConnectorDual )
	L_CPP_PROVIDE2 ( Exchange, exchange1 )
	L_CPP_PROVIDE2 ( Exchange, exchange2 )
	L_PROPERTY ( std::string, name )
	L_CPP_PROVIDE(LifeCycle, lifecycle);
LEND

}

}
