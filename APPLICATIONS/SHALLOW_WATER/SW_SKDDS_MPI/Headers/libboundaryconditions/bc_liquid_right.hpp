#ifndef Class_boundary_condition
#include "boundary_condition.hpp"
#endif

#ifndef Class_bc_liquid_right
#define Class_bc_liquid_right

/*!
 * \file bc_liquid_right.hpp
 * \brief Right liquid boundary condition
 * \author Olivier Delestre
 */

/*! \class bc_liquid_right
 * \brief Class which allows to calculate right liquid boundary thanks to the method of caracteristics.
 */


class bc_liquid_right: public boundary_condition{

	public :
		/*!
		 * \brief Constructor
		 */
  bc_liquid_right();

		/*!
		 * \brief Calculates boundary condition
		 */
  void calcul(SCALAR, SCALAR, SCALAR, SCALAR, SCALAR);

		/*!
		 * \brief Destructor
		 */
		virtual ~bc_liquid_right();
};
#endif
