#ifndef JACOBI_MEMORY_H
#define JACOBI_MEMORY_H

namespace jacobi {

class Memory
{
public:
	/** \brief Allocates two matrices of size width*height double.
	 * \param matrix1 reference to a first matrix
	 * \param matrix2 reference to a second matrix
	 * \param width the width of the matrices to allocate
	 * \param height the height of the matrices to allocate
	 */
	virtual void alloc (double* &matrix1, double* &matrix2, long width, long height ) = 0;
	
	/** \brief Deallocates two matrices allocated using alloc.
	 * \param matrix1 reference to a first matrix
	 * \param matrix2 reference to a second matrix
	 */
	virtual void dealloc (double* &A_from, double* &A_to ) = 0;
	
};

}

#endif // JACOBI_MEMORY_H
