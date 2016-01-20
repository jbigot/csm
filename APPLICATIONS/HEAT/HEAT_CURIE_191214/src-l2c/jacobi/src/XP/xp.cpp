#include <iostream>
#include <string>

#include <llcmcpp/llcmcpp3.h>
#include <stdint.h>
#include <sys/time.h>
#include <time.h>

#include <checkdata.h>
#include <computerniter.h>
#include <memory.h>
#include <XPcomputer.h>

using ::std::cerr;
using ::std::cout;
using ::std::endl;
using ::std::string;
using ::std::swap;

namespace jacobi {

namespace impl {

class XP:
	virtual public XPComputer
{
public:
	uint16_t nb_tiles_x;
	uint16_t nb_tiles_y;
	uint16_t tile_x;
	uint16_t tile_y;
	string name;
	
	CheckData* checkdata;
	
	ComputerNiter* computerniter;
	
	Memory* memory;
	
	XP ():
			checkdata(0),
			nb_tiles_y(0),
			nb_tiles_x(0),
			tile_x(0),
			tile_y(0),
			name("XP"),
			computerniter(0)
	{
	}

	uint64_t compute( size_t total_width, size_t total_height, unsigned long niter )
	{
#ifndef NDEBUG
		cerr << "[XP] compute(" << total_width << ", " << total_height << ", " << niter << ')' << endl;
#endif //NDEBUG

		size_t w = ((total_width-2)/nb_tiles_x)+2;
		size_t h = ((total_height-2)/nb_tiles_y)+2;
		
		if ( tile_y == nb_tiles_y-1 ) h = total_height - tile_y*(h-2);
		if ( tile_x == nb_tiles_x-1 ) w = total_width - tile_x*(w-2);
		
		double* A_from;
		double* A_to;
		uint64_t duration;

		if (memory) memory->alloc(A_from, A_to, w, h );

		if (computerniter) duration = computerniter->n_iter(A_from, A_to, w, h, 0, 0, w, h, niter );
		
		if (checkdata) checkdata->compare(ArrayPart<double>(A_from, w, h, 1, 1, w-2, h-2), name);

		if (memory) memory->dealloc(A_from, A_to);
		
		return duration;
	}
	
};

LCMP(XP)
	L_PROPERTY(uint16_t, nb_tiles_y);
	L_PROPERTY(uint16_t, nb_tiles_x);
	L_PROPERTY(uint16_t, tile_x);
	L_PROPERTY(uint16_t, tile_y);
	L_PROPERTY(string, name);
	L_CPP_PROVIDE(XPComputer, xpcomputer);
	L_CPP_USE(CheckData, checkdata);
	L_CPP_USE(ComputerNiter,computerniter);
	L_CPP_USE(Memory,memory);
LEND

}

}
