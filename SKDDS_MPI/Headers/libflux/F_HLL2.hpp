/******************************************************************************
 ************* F_HLL2.hpp *****************************************************
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

#ifndef Class_flux
#include "flux.hpp"
#endif

#ifndef Class_F_HLL2
#define Class_F_HLL2

/*!
 * \file F_HLL2.hpp
 * \brief Calculates the HLL2 flux
 * \author Olivier Delestre
 */

/*! \class F_HLL2
 * \brief class which allows to calculate HLL flux written under a shorter form
 *
 *\f[{\cal F}(U_G,U_D)=t_{1}F(U_D)+t_{2}F(U_G)-t_{3}(U_D-U_G),\f]
  with
  \f[t_1=\frac{\min(c_2,0)-\min(c_1,0)}{c_2-c_1},\;t_2=1-t_1,\;t_3=\frac{c_2|c_1|-c_1|c_2|}{2(c_2-c_1)}.\f]
 */


class F_HLL2: public flux{

	public :
		/*!
		 * \brief Constructor
		 */
		F_HLL2();

		/*!
		 * \brief Calculates HLL flux
		 */
		void calcul(SCALAR,SCALAR,SCALAR,SCALAR,SCALAR,SCALAR);

		/*!
		 * \brief Destructor
		 */
		virtual ~F_HLL2();
};
#endif
