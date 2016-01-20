#ifndef JACOBI_COMPUTERNITER_H
#define JACOBI_COMPUTERNITER_H

#include <stdint.h>

#include <cstring> // size_t

namespace jacobi {

class ComputerNiter
{
public:
	/**
	 * \param from a matrix of size memory_rwidth*memory_rheight
	 * \param to a matrix of size memory_rwidth*memory_rheight
	 * \param memory_rwidth the width of the matrices, including the RO ghost
	 * \param memory_rheight the height of the matrices, including the RO ghost
	 * \param tile_rx position of the readable tile in the matrices including RO ghost (x)
	 * \param tile_ry position of the readable tile in the matrices including RO ghost (y)
	 * \param tile_rwidth width of the readable tile including RO ghost
	 * \param tile_rheight height of the readable tile including RO ghost
	 * \param niter amount of iterations to execute
	 * \returns duration for the execution of the niter iter in us
	 */
	virtual uint64_t n_iter ( double* from, double* to, const size_t memory_rwidth, const size_t memory_rheight, const size_t tile_rx, const size_t tile_ry, const size_t tile_rwidth, const size_t tile_rheight, long niter ) = 0;
	
};

}

#endif // JACOBI_COMPUTERNITER_H
