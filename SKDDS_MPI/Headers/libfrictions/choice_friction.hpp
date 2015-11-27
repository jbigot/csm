/******************************************************************************
 ************* choice_friction.hpp ********************************************
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

#ifndef Class_Fr_Manning
#include "Fr_Manning.hpp"
#endif

#ifndef Class_Fr_Darcy_Weisbach
#include "Fr_Darcy_Weisbach.hpp"
#endif

#ifndef Class_choice_friction
#define Class_choice_friction

/*!
 * \file choice_friction.hpp
 * \brief Allows to choose and calculate the friction term
 * \brief by the semi-implicit method
 * \author Olivier Delestre
 */

/*! \class choice_friction
 * \brief class which allows to choose and calculate the friction term.
 */


class choice_friction{
	private :
		friction * fric;

	public :
		/*!
		 * \brief Constructor
		 */
		choice_friction(int);

		/*!
		 * \brief Calculates the friction term
		 */
		void calcul(SCALAR, SCALAR, SCALAR, SCALAR, SCALAR, SCALAR, SCALAR);


		/*!
		 * \brief Returns the discharge in direction 1 after friction
		 */
		SCALAR get_q1mod();

		/*!
		 * \brief Returns the discharge in direction 2 after friction
		 */
		SCALAR get_q2mod();


		/*!
		 * \brief Calculates explicit formula of the friction term
		 */
		void calculSf(SCALAR, SCALAR, SCALAR, SCALAR );
		
		/*!
		 * \brief Returns the explicit friction term in direction 1
		 */
		SCALAR get_Sf1();
		
		/*!
		 * \brief Returns the explicit friction term in direction 2
		 */
		SCALAR get_Sf2();

		/*!
		 * \brief Destructor
		 */
		virtual ~choice_friction();
};
#endif
