#ifndef Class_boundary_condition
#include "boundary_condition.hpp"
#endif

#ifndef Class_bc_wall
#include "bc_wall.hpp"
#endif

#ifndef Class_bc_neumann
#include "bc_neumann.hpp"
#endif

#ifndef Class_bc_liquid_left
#include "bc_liquid_left.hpp"
#endif

#ifndef Class_bc_discharge_left
#include "bc_discharge_left.hpp"
#endif

#ifndef Class_choice_condition_bottom
#define Class_choice_condition_bottom

/*!
 * \file choice_condition_bottom.hpp
 * \brief Allows to choose and calculate right boundary condition
 * \author Olivier Delestre
 */

/*! \class choice_condition_bottom
 * \brief Class which allows to choose right boundary conditions.
 */


class choice_condition_bottom {
private :
    /*!
     * \brief Constructor
     */
    choice_condition_bottom(int);

    static choice_condition_bottom * singleton;
    boundary_condition * C;

public :

    static choice_condition_bottom * getSingleton(int choice);

    /*!
     * \brief Calculates right boundary condition
     */
    void calcul(SCALAR, SCALAR, SCALAR, SCALAR, SCALAR);

    /*!
     * \brief Returns the height of water at the boundary
     */
    SCALAR get_hbound();

    /*!
     * \brief Returns the normal velocity of the flow at the boundary
     */
    SCALAR get_unormbound();

    /*!
     * \brief Returns the tangent velocity of the flow at the boundary
     */
    SCALAR get_utanbound();

    /*!
     * \brief Destructor
     */
    virtual ~ choice_condition_bottom();
};
#endif
