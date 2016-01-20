#ifndef Class_boundary_condition
#include "boundary_condition.hpp"
#endif

#ifndef Class_bc_wall
#define Class_bc_wall

/*!
 * \file bc_wall.hpp
 * \brief Wall boundary condition
 * \author Olivier Delestre
 */

/*! \class bc_wall
 * \brief Class which allows to calculate wall boundary condition.
 */


class bc_wall: public boundary_condition{

public :
  /*!
   * \brief Constructor
   */
  bc_wall();

  /*!
   * \brief Calculates boundary condition
   */
  void calcul(SCALAR, SCALAR, SCALAR, SCALAR, SCALAR);

  /*!
   * \brief Destructor
   */
  virtual ~bc_wall();
};
#endif
