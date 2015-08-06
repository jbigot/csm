#ifndef Class_scheme
#include "scheme.hpp"
#endif

#ifndef Class_order1
#include "order1.hpp"
#endif

#ifndef Class_order2
#include "order2.hpp"
#endif

#ifndef Class_choice_scheme
#define Class_choice_scheme

/*!
 * \file choice_scheme.hpp
 * \brief Allows to choose the scheme and to perform calculations
 * \author Olivier Delestre
 */

/*! \class choice_scheme
 * \brief class which allows to choose the scheme and to perform calculations.
 */


class choice_scheme{
	private :
		scheme * Sche;

	public :
		/*!
		 * \brief Constructor
		 */
		choice_scheme(parameters * &);

		/*!
		 * \brief Performs the scheme
		 */
		void calcul();

		/*!
		 * \brief Destructor
		 */
		virtual ~choice_scheme();
};
#endif
