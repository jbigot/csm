#ifndef JACOBI_INITIALIZER_H
#define JACOBI_INITIALIZER_H

#include "arraypart.h"

namespace jacobi {

class Initializer
{
public:
	/**
	 * \param data the tile to initialize including the RO ghost that will be initialized !
	 * \param on_left true if this is the leftmost tile in the global space
	 * \param on_right true if this is the rightmost tile in the global space
	 * \param on_bottom true if this is the bottommost tile in the global space
	 * \param on_top true if this is the topmost tile in the global space
	 */
	virtual void init ( ArrayPart< double > data, bool on_left, bool on_right, bool on_bottom, bool on_top ) = 0;
	
};

}

#endif // JACOBI_INITIALIZER_H
