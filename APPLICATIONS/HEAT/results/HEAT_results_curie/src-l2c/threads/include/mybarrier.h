#ifndef MYBARRIER_IS_DEFINED
#define MYBARRIER_IS_DEFINED

namespace threads {
  class MyBarrier {
  public:
	 virtual void barrier()=0;
  };
}

#endif
