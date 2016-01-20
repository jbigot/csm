#include <stddef.h>

const size_t WIDTH = 16384;
const size_t HEIGHT = 16384;

#define FROM(x,y) A_from[ (x) + width * (y) ]
#define TO(x,y) A_to[ (x) + width * (y) ]

void one_iter (  double* A_from, double* A_to, const size_t width, const size_t height, const size_t tile_xx, const size_t tile_yy, const size_t tile_width, const size_t tile_height )
{
	for ( int yy = tile_yy; yy < (tile_height + tile_yy) ; ++yy ) {
		for ( int xx = tile_xx; xx < (tile_width + tile_xx) ; ++xx ) {
			TO(xx, yy) = 0.25*( FROM(xx, yy-1) + FROM(xx, yy+1) + FROM(xx-1, yy) + FROM(xx+1,yy) );
		}
	}
}

int main ()
{
	double* A_from = new double[WIDTH*HEIGHT];
	double* A_to = new double[WIDTH*HEIGHT];
	for ( unsigned repeat=0; repeat<10; ++repeat) one_iter(A_from, A_to, WIDTH, HEIGHT, 1, 1, WIDTH-2, HEIGHT-2);
	delete A_from;
	delete A_to;
	return 0;
}
