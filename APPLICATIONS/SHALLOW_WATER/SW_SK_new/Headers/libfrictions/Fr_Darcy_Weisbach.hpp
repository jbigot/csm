/******************************************************************************
 ************* Fr_Darcy_Weisbach.hpp ******************************************
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

#ifndef Class_friction
#include "friction.hpp"
#endif

#ifndef Class_Fr_Darcy_Weisbach
#define Class_Fr_Darcy_Weisbach

/*!
 * \file Fr_Darcy_Weisbach.hpp
 * \brief Calculates the Darcy-Weisbach friction term
 * \brief by the semi-implicit method
 * \author Olivier Delestre
 */

/*! \class Fr_Darcy_Weisbach
 * \brief class which allows to calculate Darcy-Weisbach friction term
 *
 *\f[S_f=\frac{fu|u|}{gh}\f]
 *thanks to a semi-implicit method :
 \f[q_i^{n+1}=\frac{q_{i*}^{n+1}}{1+\Delta{t}\frac{f|u_i^n|}{8h_i^{n+1}}}.\f]
 *where \f$f\f$ is the friction coefficient.
 */


class Fr_Darcy_Weisbach: public friction{

	public :
		/*!
		 * \brief Constructor
		 */
		Fr_Darcy_Weisbach();

		/*!
		 * \brief Calculates the Darcy-Weisbach friction term
		 */
		void calcul(SCALAR, SCALAR, SCALAR, SCALAR, SCALAR, SCALAR, SCALAR);


		/*!
		 * \brief Calculates explicit formula of the Darcy-Weisbach friction term
		 */
		void calculSf(SCALAR, SCALAR, SCALAR, SCALAR );

		/*!
		 * \brief Destructor
		 */
		virtual ~Fr_Darcy_Weisbach();
};
#endif
