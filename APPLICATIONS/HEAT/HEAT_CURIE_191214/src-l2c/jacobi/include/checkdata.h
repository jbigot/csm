#ifndef JACOBI_CHECKDATA_H
#define JACOBI_CHECKDATA_H

#include <string>

#include "arraypart.h"

namespace jacobi {

class CheckData
{
public:
	/**
	 * \param data the tile to compare excluding the RO ghost !
	 */
	virtual void compare ( ArrayPart< double > data, std::string msg ) = 0;
};

}

#endif // JACOBI_CHECKDATA_H
