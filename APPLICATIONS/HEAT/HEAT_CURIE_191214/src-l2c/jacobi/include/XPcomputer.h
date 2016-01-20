#ifndef JACOBI_XPCOMPUTER_H
#define JACOBI_XPCOMPUTER_H

#include <stdint.h>

namespace jacobi {

class XPComputer
{
public:
	/*! \brief Times the jacobi method to a matrix of size w x h niter times.
	 * \param rwidth the width of the matrix including RO borders
	 * \param rheight the height of the matrix including RO borders
	 * \param niter amount of iterations to execute
	 * \returns duration for the execution of the niter iter in us
	 */
  virtual uint64_t compute ( size_t rwidth, size_t rheight, unsigned long niter ) = 0;
	
};

}

#endif // JACOBI_XPCOMPUTER_H
