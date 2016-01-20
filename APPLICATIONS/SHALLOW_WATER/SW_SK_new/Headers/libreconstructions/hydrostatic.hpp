/******************************************************************************
 ************* hydrostatic.hpp ************************************************
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

#include "misc.hpp"

#ifndef Class_hydrostatic
#define Class_hydrostatic

/*!
 * \file hydrostatic.hpp
 * \brief Performs the hydrostatic reconstruction
 * \author Olivier Delestre
 */

/*! \class hydrostatic
 * \brief class which allows to perform the hydrostatic reconstruction.
 *
 * At first order, we get \f$U_{\bullet L}\f$
 * and \f$U_{\bullet R}\f$ variables from \f$U_\bullet\f$ variables thanks to the hydrostatic reconstruction 
 *\f[\left\{\begin{array}{l}
U_{i+1/2L}=(h_{i+1/2L},h_{i+1/2L}u_i),\;h_{i+1/2L}=(h_i+z_i-\max(z_i,z_{i+1}))_+\\
U_{i+1/2R}=(h_{i+1/2R},h_{i+1/2R}u_i),\;h_{i+1/2R}=(h_{i+1}+z_{i+1}-\max(z_i,z_{i+1}))_+
\end{array}\right.\f]
 */


class hydrostatic{
	private :
		SCALAR hg_rec, hd_rec;

	protected:
		/*!
		 * \brief Constructor
		 */
		hydrostatic();
		static hydrostatic * singleton;

	public :
		static hydrostatic * getSingleton();

		/*!
		 * \brief Calculates the hydrostatic reconstruction
		 */
		void calcul(SCALAR,SCALAR,SCALAR);

		/*!
		 * \brief Returns left height of water reconstruction
		 */
		SCALAR hg();

		/*!
		 * \brief Returns right height of water reconstruction
		 */
		SCALAR hd();
};
#endif
