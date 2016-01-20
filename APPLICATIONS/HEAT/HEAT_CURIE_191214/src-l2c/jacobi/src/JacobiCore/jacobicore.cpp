#include <iostream>

#include <llcmcpp/llcmcpp3.h>

#include "computer.h"

#define FROM(x,y) A_from[ (x) + width * (y) ]
#define TO(x,y) A_to[ (x) + width * (y) ]

namespace jacobi {

namespace impl {

class JacobiCore :
		virtual public Computer
{
public:
	virtual void one_iter (  double* A_from, double* A_to, const size_t width, const size_t height, const size_t tile_xx, const size_t tile_yy, const size_t tile_width, const size_t tile_height )
	{
		for ( int yy = tile_yy; yy < (tile_height + tile_yy) ; ++yy ) {
			for ( int xx = tile_xx; xx < (tile_width + tile_xx) ; ++xx ) {
			  TO(xx, yy) = (1-4*0.05)*FROM(xx, yy)+0.05*( FROM(xx, yy-1) + FROM(xx, yy+1) + FROM(xx-1, yy) + FROM(xx+1,yy) );
			}
		}
	}
};

LCMP(JacobiCore)
	L_CPP_PROVIDE(Computer, computer)
LEND

}

}
