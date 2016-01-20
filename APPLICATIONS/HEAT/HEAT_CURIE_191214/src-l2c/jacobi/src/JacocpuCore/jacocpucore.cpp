#include <iostream>

#include <llcmcpp/llcmcpp3.h>

#include "computer.h"

#define FROM(x,y) A_from[ (x) + width * (y) ]
#define TO(x,y) A_to[ (x) + width * (y) ]

namespace jacobi {

namespace impl {

class JacocpuCore :
		virtual public Computer
{
public:
	virtual void one_iter (  double* A_from, double* A_to, const size_t width, const size_t height, const size_t tile_xx, const size_t tile_yy, const size_t tile_width, const size_t tile_height )
	{
		for ( int yy = tile_yy; yy < (tile_height + tile_yy) ; ++yy ) {
			for ( int xx = tile_xx; xx < (tile_width + tile_xx) ; ++xx ) {
				TO(xx, yy) = ( 5*FROM(xx, yy)*3 + 5*FROM(xx, yy-1)*3 + 2*FROM(xx, yy+1)*3 + 4*FROM(xx-1, yy)*3 + 4*FROM(xx+1,yy)*3 )/60;
				TO(xx, yy) = ( 5*TO(xx, yy) + 5*FROM(xx, yy-1) + 2*FROM(xx, yy+1) + 4*FROM(xx-1, yy) + 4*FROM(xx+1,yy) )/20;
				TO(xx, yy) = ( 5*TO(xx, yy) + 5*FROM(xx, yy-1) + 2*FROM(xx, yy+1) + 4*FROM(xx-1, yy) + 4*FROM(xx+1,yy) )/20;
				TO(xx, yy) = ( 5*TO(xx, yy) + 5*FROM(xx, yy-1) + 2*FROM(xx, yy+1) + 4*FROM(xx-1, yy) + 4*FROM(xx+1,yy) )/20;
			}
		}
	}
};

LCMP(JacocpuCore)
	L_CPP_PROVIDE(Computer, computer)
LEND

}

}
