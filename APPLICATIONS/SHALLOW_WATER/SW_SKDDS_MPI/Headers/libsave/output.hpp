#ifndef Class_parameters
#include "parameters.hpp"
#endif

#ifndef Class_output
#define Class_output

/*!
 * \file output.hpp
 * \brief Write the result in the output file
 * \author Christian Laguerre
 */

/*! \class output
 * \brief Class which allows to write the result in the output file.
 */


class output {
protected :
    int Nxcell, Nycell;
    SCALAR dx,dy;
    string outputDirectory,namefile;
public :

    /*!
     * \brief Constructor
     */
    output(parameters * &par);

    virtual void write(TAB &,TAB &,TAB  &,TAB &,SCALAR ) = 0;

    /*!
     * \brief Destructor
     */
    virtual ~output();
};
#endif

