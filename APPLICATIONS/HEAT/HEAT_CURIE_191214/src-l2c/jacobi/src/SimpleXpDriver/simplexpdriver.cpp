#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>

#include <llcmcpp/go.h>
#include <llcmcpp/llcmcpp3.h>
#include <stdint.h>

#include "XPcomputer.h"


using ::std::cout;
using ::std::endl;
using ::std::sort;
using ::std::vector;

namespace jacobi {

namespace impl {

class SimpleXpDriver:
  virtual public llcmcpp::Go
{
public:
	SimpleXpDriver ():
			computer(0),
			niter(0),
			nxp(10),
			width(128),
			height(128)
	{
	}
	
	XPComputer* computer;
	
	uint32_t niter, nxp;
	
	uint64_t width, height;
	
	virtual void go ()
	{
	  //cout << "#[SimpleXpDriver] go!" << endl;
		
		if ( niter == 0 ) niter = 100000000LL / (width*height);
		if ( niter<2 ) niter = 2;
		
		//cout << "# height(nbdouble): " << height << "\t width(nbdouble): " << width << "\t niter: " << niter << endl;
		computer->compute(width, height, niter);

		/*vector< uint64_t > durations;
		durations.reserve(nxp);
		for ( unsigned xpid = 0; xpid < nxp; ++xpid ) {
			durations.push_back(computer->compute(width, height, niter));
		}
		sort(durations.begin(), durations.end());
		
		//cout << "size(nbdouble): " << width*height << "\tD0(ns): " << (1000*durations[0])/niter;
		for ( unsigned decile = 1; decile<10; ++decile ) {
			unsigned decimal = (static_cast< uint64_t >(decile)*nxp-5)%10;
			unsigned rank = (static_cast< uint64_t >(decile)*nxp-5)/10;
			uint64_t duration = ((10-decimal)*durations[rank]+decimal*durations[rank+1])/10;
			//cout << "\tD" << decile << "(ns): " << (1000*duration)/niter;
			}*/
		//cout << "\tD11(ns)" << (1000*durations[nxp-1])/niter << endl;
	}
	
};

LCMP(SimpleXpDriver)
	L_PROPERTY(uint32_t, niter);
	L_PROPERTY(uint32_t, nxp);
	L_PROPERTY(uint64_t, width);
	L_PROPERTY(uint64_t, height);
	L_CPP_PROVIDE(llcmcpp::Go, go);
	L_CPP_USE(XPComputer, computer);
LEND

}

}
