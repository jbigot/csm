#ifndef LocalBarrier_is_defined
#define LocalBarrier_is_defined

#include <pthread.h>

#include <stdint.h>
#include <llcmcpp/lifecycle.h>
#include <llcmcpp/go.h>
#include <llcmcpp/collective.h>

#include "mybarrier.h"

namespace threads
{

namespace impl
{

  class LocalBarrier : 
	 virtual public llcmcpp::LifeCycle,
	 virtual public threads::MyBarrier,
	 virtual public llcmcpp::Collective
  {

public:
	 LocalBarrier();
	 ~LocalBarrier() {}

	 void id(std::string* v);
	 void nbpeers(uint32_t* v);

	 // MyBarrier
	 virtual void barrier();

	 // From LifeCycle
	 //! The component has been configured (no more set pending)
	 virtual void configure_complete () {}
	 //! All components have been configured: can start using port
	 virtual void activate ();

	 //! This component has to stop
	 virtual void deactivate () {}
	 //! This component has to be destroyed (needed or should we rely on destructor?)
	 virtual void destroy () {}	 

  protected:
	 std::string m_id;

	 uint32_t m_nbpeers;

	 unsigned barrier_total;
	 volatile unsigned barrier_current;

	 pthread_mutex_t _mtx;
	 pthread_cond_t _cnd;
  };

}

}


#endif
