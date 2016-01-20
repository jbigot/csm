#include <iostream>

#include <llcmcpp/llcmcpp3.h>
#include <stdint.h>

#include <memory.h>

using ::std::cerr;
using ::std::endl;

namespace jacobi {

namespace impl {

class MAlloc:
	virtual public Memory
{
public:
	uint8_t debug;

	MAlloc ():
			debug(0xFF)
	{
	}

	void alloc( double* &A_from, double* &A_to, long width, long height )
	{
		A_from = new double[height*width];
		A_to = new double[height*width];
		
	}

	void dealloc( double* &A_from, double* &A_to )
	{
		delete[] A_from;
		delete[] A_to;
	}
	
};

LCMP(MAlloc)
	L_CPP_PROVIDE(Memory, memory);
LEND

}

}
