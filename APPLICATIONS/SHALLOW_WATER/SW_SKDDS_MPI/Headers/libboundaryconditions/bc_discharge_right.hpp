#ifndef Class_boundary_condition
#include "boundary_condition.hpp"
#endif

#ifndef Class_bc_discharge_right
#define Class_bc_discharge_right

/*!
 * \file bc_discharge_imp.hpp
 * \brief Performs the scheme
 * \author Ulrich Razafison
 */

/*! \class bc_discharge_imp
 * \brief Class that computes boundary conditions where the discharge is imposed.
 */


class bc_discharge_right: public boundary_condition{
 protected :

  SCALAR tol;
  int maxiter;
public :
  /*!
   * \brief Constructor
   */
  bc_discharge_right();

  /*!
   * \brief Calculates boundary condition
   */
  void calcul(SCALAR ,SCALAR ,SCALAR ,SCALAR ,SCALAR);

  /*!
   * \brief Destructor
   */
  virtual ~bc_discharge_right();
};
#endif
