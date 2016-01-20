#ifndef OUTPUT_HPP
#include "output.hpp"
#endif

#ifndef Classe_vtk_out
#define Classe_vtk_out

/*!
 * \file vtk_out.hpp
 * \brief Write the result in the output file for vtk
 * \author Christian Laguerre
 */

/*! \class Vtk_out
 * \brief Class which allows to write the result in the output file for vtk.
 */

class vtk_out: public output {
    int count;
public :
    /*!
     * \brief Constructor
     */
    vtk_out(parameters * &par);

    void write(TAB & ,TAB & ,TAB & ,TAB & , SCALAR ) ;


    /*!
     * \brief Destructor
     */
    virtual ~vtk_out();
};
#endif

