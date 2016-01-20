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

class XPDriver:
  virtual public llcmcpp::Go
{
public:
	XPDriver ():
			computer(0),
			nxp(10),
			run_per_size(16),
			slow_start(0),
			magic(100000000LL),
			l1_size(65536), // 64kio
			l2_size(524288), // 512kio
			l3_size(6291456), // 6Mio
			ram_size(2147483648) // 2Gio
	{
	}
	
	XPComputer* computer;
	
	uint16_t nxp;
	
	uint32_t run_per_size;
	
	uint32_t slow_start;
	
	uint64_t magic;
	
	uint64_t l1_size, l2_size, l3_size, ram_size;
	
	virtual void go ()
	{
		cout << "#[XPDriver] go!" << endl;
		
		if ( nxp < 10 ) nxp = 10;
		uint32_t rps = run_per_size;
		run_per_size = 1;
		while ( run_per_size <= rps ) run_per_size *= 2;
		run_per_size /= 2;
		
		cout << "# L1 " << l1_size << "o == " << (l1_size/2/sizeof(double)) << " dbl/mat" << endl;
		size_t reached = one_size(0, l1_size);
		cout << "# L2 (" << l2_size << "o == " << (l2_size/2/sizeof(double)) << " dbl/mat" << ")" << endl;
		reached = one_size(reached, l2_size);
		cout << "# L3 (" << l3_size << "o == " << (l3_size/2/sizeof(double)) << " dbl/mat" << ")" << endl;
		reached = one_size(reached, l3_size);
		cout << "# RAM (" << ram_size << "o == " << (ram_size/2/sizeof(double)) << " dbl/mat" << ")" << endl;
		reached = one_size(reached, ram_size);
		cout << "# DONE (kthxbye)" << endl;
	}
	
private:
	size_t one_size ( size_t above, size_t below )
	{
		size_t step = 1;
		while ( step*step<=below ) step*=2; // lowest side size that doesn't fit in below (o)
		step /= 2; // highest side size that fit in below (o)
		step /= 4; // (4x4 == 16 == sizeof(double)x2) highest side size for 2 matrices to fit in below (nbdbl)
		step /= run_per_size; // get at least run_per_size run
		if ( step < 1 ) step = 1;
		
		size_t size = step;
		while ( size <= above ) size += step; // multiple of step above size
		
		cout << "# for " << size*size*sizeof(double)*2 << "o to " << below << "o step " << step*step*sizeof(double)*2 << "o" << endl;
		for ( ; size*size*sizeof(double)*2 <= below; size += step ) one_run(size, size); // in below
		size -= step;
		return size;
	}
	
	void one_run ( size_t width, size_t height )
	{
		unsigned long niter = magic / (1LL*slow_start+width*height);
		if ( niter<2 ) niter = 2;
		
		cout << "# height(nbdouble): " << height << "\t width(nbdouble): " << width << "\t niter: " << niter << endl;
		
		vector< uint64_t > durations;
		durations.reserve(nxp);
		for ( unsigned xpid = 0; xpid < nxp; ++xpid ) {
			durations.push_back(computer->compute(width, height, niter));
		}
		sort(durations.begin(), durations.end());
		
		cout << "size(nbdouble): " << width*height;
		for ( unsigned decile = 1; decile<10; ++decile ) {
			unsigned decimal = (static_cast< uint64_t >(decile)*nxp-5)%10;
			unsigned rank = (static_cast< uint64_t >(decile)*nxp-5)/10;
			uint64_t duration = ((10-decimal)*durations[rank]+decimal*durations[rank+1])/10;
			cout << "\tD" << decile << "(ns): " << (1000*duration)/niter;
		}
		cout << endl;
	}
	
};

LCMP(XPDriver)
	L_PROPERTY(uint16_t, nxp);
	L_PROPERTY(uint32_t, run_per_size);
	L_PROPERTY(uint32_t, slow_start);
	L_PROPERTY(uint64_t, magic);
	L_PROPERTY(uint64_t, l1_size);
	L_PROPERTY(uint64_t, l2_size);
	L_PROPERTY(uint64_t, l3_size);
	L_PROPERTY(uint64_t, ram_size);
	L_CPP_PROVIDE(llcmcpp::Go, go);
	L_CPP_USE(XPComputer, computer);
LEND

}

}
