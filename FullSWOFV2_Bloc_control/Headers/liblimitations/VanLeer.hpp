/******************************************************************************
 ************* VanLeer.hpp ****************************************************
 ******************************************************************************
 * 
 * Copyright CNRS - Universite d'Orleans - BRGM (France)
 * contributors: Olivier Delestre (2008) 
 * olivierdelestre41@yahoo.fr 
 *
 * This file is part of FullSWOF_2D software. 
 * FullSWOF_2D = Full Shallow-Water equations for Overland Flow, 
 * in two dimensions of space.
 * This software is a computer program whose purpose is to compute
 * solutions for 2D Shallow-Water equations.
 *
 * This software is governed by the CeCILL license under French law and
 * abiding by the rules of distribution of free software.  You can  use, 
 * modify and/ or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * "http://www.cecill.info". 
 *
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability. 
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or 
 * data to be ensured and,  more generally, to use and operate it in the 
 * same conditions as regards security. 
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 *
 ******************************************************************************/

#ifndef Class_limiter
#include "limiter.hpp"
#endif

#ifndef Class_VanLeer
#define Class_VanLeer

/*!
 * \file VanLeer.hpp
 * \brief VanLeer slope limiter
 * \author Olivier Delestre
 */

/*! \class VanLeer
 * \brief class which allows to calculate the VanLeer of two real numbers \f$(x,y)\f$ 
 *
 *
 \f[VL(x,y)=\left\{\begin{array}{ll}
                 0 & \mbox{if }xy\leq 0,\\
                 \displaystyle\frac{2xy}{x+y} & \mbox{else}.
                \end{array}\right.\f]
 */



class VanLeer: public limiter{

	public :
		/*!
		 * \brief Constructor
		 */
		VanLeer();

		/*!
		 * \brief calculates the VanLeer of a and b
		 */
		void calcul(SCALAR, SCALAR);

		/*!
		 * \brief Destructor
		 */
		virtual ~VanLeer();
};
#endif
