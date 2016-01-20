#ifndef MXN_BASICBCCONFIG_H
#define MXN_BASICBCCONFIG_H

#include "DistributionLibrary.h"

namespace mxn {

class BasicBCConfig :
		virtual public mxn::DistributionLibrary
{
public:
	 virtual void setEltSize(unsigned long size)=0;
	 virtual void setBlocSize(unsigned long bsz)=0; // bloc-> bc(0), cyclic -> bc(1)
	 virtual void setTotalNbElt(unsigned long elt_nb)=0;
	 virtual unsigned long getBlocSize()=0;
	 virtual unsigned long getLocalNbElt()=0;
	 virtual unsigned long getTotalNbElt()=0;
};

class BasicClientConfig :
		virtual public BasicBCConfig,
		virtual public DistributionLibraryClient
{
};

}

#endif // MXN_BASICBCCONFIG_H
