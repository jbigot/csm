/******************************************************************************
 ************* GreenAmpt.hpp **************************************************
 ******************************************************************************
 * 
 * Copyright CNRS - Universite d'Orleans - BRGM (France)
 * contributor: Marie Rousseau (2008)
 * ma.rousseau@brgm.fr
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


#ifndef Class_infiltration
#include "infiltration.hpp"
#endif

#ifndef Class_GreenAmpt
#define Class_GreenAmpt

/*!
 * \file GreenAmpt.hpp
 * \brief Calculates the infiltration capacity and infiltrated volum
 * \author Marie Rousseau
 */

/*! \class GreenAmpt
 * \brief Class which allows to calculate the infiltration capacity and infiltrated volum.
 *
 *
 *\f[I^{n}_{C}=
  \left\{\begin{array}{l}
  {Kc}(1+\frac{h_{f}-h^{n}_{sur}}{{Z}^{n}_{f}}) \;{if}\; {Z}^{n}_{f} <= {Zc} \\
\\
  {K}^{n}_{e}(1+\frac{h_{f}-h^{n}_{sur}}{{Z}^{n}_{f}}) \end{array}\right.,\f]
  with hydraulic conductivity
  \f[{K}^{n}_{e}=\frac{{Z}^{n}_{f}}{[(Z_{f}^{n}-Z_{c})/K_{s}]+Z_{c}/K_{c}},\f]
  *
  * Where -h^{n}_{sur} \geq 0 is the water height at the topography surface (obtained thanks to Shallow Water model)
 */


class GreenAmpt: public infiltration{

  SCALAR Ke,Zf,imax;
	public :
		/*!
		 * \brief Constructor
		 */
		GreenAmpt(parameters &);

		/*!
		 * \brief Calculates infiltration capacity
		 */
		SCALAR capacity(SCALAR ,SCALAR ,SCALAR ,SCALAR,SCALAR , SCALAR ,SCALAR , SCALAR );

		/*!
		 * \brief Calculates infiltrated volum
		 */
  void calcul(SCALAR ,SCALAR ,SCALAR  ,SCALAR,SCALAR , SCALAR ,SCALAR , SCALAR );

		/*!
		 * \brief Destructor
		 */
		virtual ~GreenAmpt();
};
#endif
