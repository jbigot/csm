/******************************************************************************
 ************* F_HLL.hpp *****************************************************
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

#ifndef Class_F_HLL
#define Class_F_HLL

/*!
 * \file F_HLL.hpp
 * \brief Calculates the HLL flux
 * \author Olivier Delestre
 */

/*! \class F_HLL
 * \brief class which allows to calculate HLL flux
 *
 *\f[{\cal F}(U_G,U_D)=
  \left\{\begin{array}{l}
  {F}(U_G)\;{if}\; 0<c_{1} \\
  {\frac{c_{1}F(U_G)-c_{2}F(U_D)}{c_{2}-c_{1}}}+{\frac{c_{1}c_{2}}{c_{2}-c_{1}}}.(U_D-U_G)\;{if}\; c_{1}<0<c_{2} \\
  {F}(U_D)\;{if}\; c_{2}<0 \end{array}\right.,\f]
  with
  \f[c_{1}={\inf\limits_{U=U_G,U_D}}({\inf\limits_{j\in\{1,2\}}}|\lambda_{j}(U)|)\;{and}\;c_{2}={\sup\limits_{U=U_G,U_D}}({\sup\limits_{j\in\{1,2\}}}|\lambda_{j}(U)|),\f]
  where \f$\lambda_1(U)=u-\sqrt{gh}\f$ et \f$\lambda_2(U)=u+\sqrt{gh}\f$ are the eigenvalues of the shallow water system.
 */


class F_HLL: public flux{

	public :
		/*!
		 * \brief Constructor
		 */
		F_HLL();

		/*!
		 * \brief Calculates HLL flux
		 */
		void calcul(SCALAR,SCALAR,SCALAR,SCALAR,SCALAR,SCALAR);

		/*!
		 * \brief Destructor
		 */
		virtual ~F_HLL();
};
#endif
