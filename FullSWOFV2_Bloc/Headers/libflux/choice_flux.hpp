#ifndef Class_flux
#include "flux.hpp"
#endif

#ifndef Class_F_Rusanov
#include "F_Rusanov.hpp"
#endif

#ifndef Class_F_HLL
#include "F_HLL.hpp"
#endif

#ifndef Class_F_HLL2
#include "F_HLL2.hpp"
#endif

#ifndef Class_choice_flux
#define Class_choice_flux

/*!
 * \file choice_flux.hpp
 * \brief Allows to choose and calculate numerical flux
 * \author Olivier Delestre
 */

/*! \class choice_flux
 * \brief class which allows to choose and calculate the numerical flux.
 */


class choice_flux{
	private :
		/*		choice_flux(string);*/
		/*!
		 * \brief Constructor
		 */
		choice_flux(int);

		static choice_flux * singleton;
		flux * F;

	public :
		static choice_flux * getSingleton(int choice);

		/*!
		 * \brief Calculates the numerical flux
		 */
		void calcul(SCALAR,SCALAR,SCALAR,SCALAR,SCALAR,SCALAR);

		/*!
		 * \brief Sets the variable tx
		 */
		void set_tx(SCALAR);

		/*!
		 * \brief Returns the first component of the numerical flux
		 */
		SCALAR get_f1();

		/*!
		 * \brief Returns the second component of the numerical flux
		 */
		SCALAR get_f2();

		/*!
		 * \brief Returns the third component of the numerical flux
		 */
		SCALAR get_f3();

		/*!
		 * \brief Returns the CFL
		 */
		SCALAR get_cfl();

		/*!
		 * \brief Destructor
		 */
		virtual ~ choice_flux();
};
#endif
