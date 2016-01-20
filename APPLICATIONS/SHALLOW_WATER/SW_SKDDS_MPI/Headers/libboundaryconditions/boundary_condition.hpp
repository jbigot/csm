#ifndef Class_boundary_condition
#define Class_boundary_condition

/*!
 * \file boundary_condition.hpp
 * \brief Boundary condition
 * \author Olivier Delestre
  */

/*! \class boundary_condition
 * \brief Class which allows to calculate boundary conditions.
 */

#include "parameters.hpp"

class boundary_condition {
protected :
    SCALAR hbound,unormbound,utanbound,unormfix;

public :
    /*!
     * \brief Constructor
     */
    boundary_condition();

    virtual void calcul(SCALAR, SCALAR, SCALAR, SCALAR, SCALAR) =0;

    /*!
     * \brief Returns the height of water at the boundary
     */
    SCALAR get_hbound() const;

    /*!
     * \brief Returns the normal velocity of the flow at the boundary
     */
    SCALAR get_unormbound() const;

    /*!
     * \brief Returns the tangent velocity of the flow at the boundary
     */
    SCALAR get_utanbound() const;

    /*!
     * \brief Destructor
     */
    virtual ~boundary_condition();
};
#endif

