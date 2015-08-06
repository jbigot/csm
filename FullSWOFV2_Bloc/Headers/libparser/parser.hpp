/******************************************************************************
 ************* parser.hpp *****************************************************
 ******************************************************************************
 * 
 * Copyright CNRS - Universite d'Orleans - BRGM (France)
 * contributors: Christian Laguerre (2010)
 * christian.laguerre@math.cnrs.fr
 *
 * This file is part of FullSWOF_2D software. 
 * FullSWOF_2D = Full Shallow-Water equations for Overland Flow, 
 * in two dimensions of space.
 * This software is a computer program whose purpose is to compute
 * solutions for 2D Shallow-Water equations.
 *
 * This software is governed by the CeCILL license under French law and
 * abiding by the rules of distribution of free software.  You can  use, 
 * modify and/ or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * "http://www.cecill.info". 
 *
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability. 
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or 
 * data to be ensured and,  more generally, to use and operate it in the 
 * same conditions as regards security. 
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 *
 ******************************************************************************/

#ifndef Class_parser
#define Class_parser

#include "misc.hpp"

// To be compiled independently:
// comment the previous line and uncomment the 5 following commands
/*
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
*/

/*!
 * \file parser.h
 * \brief This file is a parser which reads the input file writen as
 * description <variable>:: value # comment
 * and keep the values after the "::" ignoring the comments that begin with a "#"
 * \author Christian Laguerre
 */

/*! \class parser
 * \brief Class that reads the input file writen as
 * description <variable>:: value # comment
 * and keep the values after the "::" ignoring the comments that begin with a "#".
 */


class parser {
	protected :
	int i,j, nblines;
	string ch;
	string * data;
	string value;
	size_t found;	
	public:
	/*!
	 * \brief Constructor: read the input parameter and copy the data in a tabular 
	 * Inputs: filename, the name of the paramters file
	 * Outputs: data, tabular that contain the strings "description <variable>:: value"
	 *		  nblines, the length of data
	 */
	
	parser(const char *);
 	  
	/*!
	* \brief For each variable, return the value of the variable writen in the input tabular 
	* Inputs: tag, the name of the variable with delimiters
	* Outputs: value, the value of the variable as a string
	*/
	  
	string  GetValue(const char *);
	  
	/*!
	 * \brief Destructor
	 */
	virtual ~parser();
};
#endif
