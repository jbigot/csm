#ifndef _ITF_VECTORADD_BC_H
#define _ITF_VECTORADD_BC_H

#include "BasicBC_Redist.h"

namespace mxn {

  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  
  class VectorAddInternal // For BC
  {
  public:
	 virtual unsigned getRank()=0;
	 virtual void foo(const mxn::BasicBCDescr* descr, long *ptr)=0;
  };

  class VectorAddInternal2 // For BC
  {
  public:
	 virtual void foo(unsigned dest, const mxn::BasicBCDescr* descr, long *ptr)=0;
  };

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

} // namespace

#endif