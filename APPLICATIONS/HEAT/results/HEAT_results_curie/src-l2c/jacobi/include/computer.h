#ifndef JACOBI_COMPUTER_H
#define JACOBI_COMPUTER_H

#include <cstring> // size_t

namespace jacobi {

class Computer
{
public:
	/**
	 * \param from a matrix of size memory_rwidth*memory_rheight (will be acessed RO)
	 * \param to a matrix of size memory_rwidth*memory_rheight (will be acessed WO)
	 * \param memory_rwidth the width of the matrices, including the RO ghost
	 * \param memory_rheight the height of the matrices, including the RO ghost
	 * \param tile_wx position of the writeable tile in the matrices (x)
	 * \param tile_wy position of the writeable tile in the matrices (y)
	 * \param tile_wwidth width of the writeable tile
	 * \param tile_wheight height of the writeable tile
	 */
	virtual void one_iter ( double* from, double* to, const size_t memory_rwidth, const size_t memory_rheight, const size_t tile_wx, const size_t tile_wy, const size_t tile_wwidth, const size_t tile_wheight ) = 0;
	
};

}

#endif // JACOBI_COMPUTER_H
