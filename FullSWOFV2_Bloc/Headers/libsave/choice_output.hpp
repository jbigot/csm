#ifndef Class_output
#include "output.hpp"
#endif

#ifndef Class_vtk_out
#include "vtk_out.hpp"
#endif

#ifndef Class_choice_output
#define Class_choice_output

/*!
 * \file choice_output.hpp
 * \brief Allows to choose the style of the output file
 * \author Christian Laguerre
 */

/*! \class choice_output
 * \brief Class which allows to choose the style of the output file.
 */


class choice_output {
private :

    /*!
    * \brief Constructor
    */
    choice_output(parameters * &);

    static choice_output * singleton;
    output * out;

public :

    static choice_output * getSingleton();

    /*!
     * \brief Write the result in the output file
     */
    void write(TAB & ,TAB & ,TAB & ,TAB & ,SCALAR );

    /*!
     * \brief Destructor
     */
    virtual ~choice_output();
};
#endif
