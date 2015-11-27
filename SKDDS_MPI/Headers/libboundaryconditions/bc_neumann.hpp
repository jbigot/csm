#ifndef Class_boundary_condition
#include "boundary_condition.hpp"
#endif

#ifndef Class_bc_neumann
#define Class_bc_neumann

/*!
 * \file bc_neumann.hpp
 * \brief Neumann boundary condition
 * \author Olivier Delestre
 */

/*! \class bc_neumann
 * \brief Class which allows to calculate Neumann boundary conditions.
 */


class bc_neumann: public boundary_condition{

	public :
		/*!
		 * \brief Constructor
		 */
  bc_neumann();

		/*!
		 * \brief Calculates boundary condition
		 */
  void calcul(SCALAR, SCALAR, SCALAR, SCALAR, SCALAR);

		/*!
		 * \brief Destructor
		 */
		virtual ~bc_neumann();
};
#endif
