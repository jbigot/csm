#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <pthread.h>

#include <stdint.h>

#include "llcmcpp/llcmcpp2.h"

#include "localbarrier.h"

using namespace llcmcpp;
using namespace threads::impl;
using namespace std;

////////////////////////////////////////////////////////////////////////////////

LocalBarrier::LocalBarrier() : m_nbpeers(0), barrier_total(0), barrier_current(0)
{
  pthread_mutexattr_t attr;
  //  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);

  pthread_mutex_init(&_mtx, NULL);
  pthread_cond_init(&_cnd, NULL);

}

void
LocalBarrier::id(std::string* v)
{
  cerr << "[LocalBarrier-"<<m_id<<"] id set to: "<<*v<<endl;
  m_id=*v;
}

void
LocalBarrier::nbpeers(uint32_t *v)
{
  //  cerr << "A\n";
  cerr << m_id<<endl;
  //cerr << *v<<endl;
  cerr << "[LocalBarrier-"<<m_id<<"] nbworkers set to: "<<*v<<endl;
  m_nbpeers=*v;
  barrier_total = *v;
}

////////////////////////////////////////////////////////////////////////////////

void
LocalBarrier::barrier ()
{
  pthread_mutex_lock(&_mtx);
  //  cerr << "[LocalBarrier-"<<m_id<<"] thread "<<(void*)pthread_self()<<endl;
  //  cerr << "[LocalBarrier-"<<m_id<<"] ack: "<<barrier_current<<"/"<<barrier_total<<endl;
  if (++barrier_current == barrier_total) {
	 //	 cerr << "[LocalBarrier-"<<m_id<<"] all received => awaking all blocking threads\n";
	 barrier_current=0;
	 pthread_cond_broadcast(&_cnd);
  } else {
	 //	 cerr << "[LocalBarrier-"<<m_id<<"] thread waiting "<<(void*)pthread_self()<<endl;
	 pthread_cond_wait(&_cnd, &_mtx);
  }
  //  cerr << "[LocalBarrier-"<<m_id<<"] do_barrier...done for thread "<<(void*)pthread_self()<<endl;
  pthread_mutex_unlock(&_mtx);
}

////////////////////////////////////////////////////////////////////////////////

void
LocalBarrier::activate() {
  cerr << "[LocalBarrier-"<<m_id<<"] activate...\n";
  this->barrier();
  cerr << "[LocalBarrier-"<<m_id<<"] \n";
  this->barrier();
  cerr << "[LocalBarrier-"<<m_id<<"] activate...done\n";
}

////////////////////////////////////////////////////////////////////////////////


LCMP(LocalBarrier)
	LPROVIDE(llcmcpp::Collective, collective);
	LPROVIDE(threads::MyBarrier, mb)
	LUSE(std::string, id)
	LUSE(uint32_t, nbpeers)
LEND

