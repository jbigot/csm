/******************************************************************************
 ************* parser.cpp *****************************************************
 ******************************************************************************
 * 
 * Copyright CNRS - Universite d'Orleans - BRGM (France)
 * contributors: Carine Lucas(2010) and Christian Laguerre (2010)
 * carine.lucas@univ-orleans.fr and christian.laguerre@math.cnrs.fr
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

#include <stdio.h>
#include <vector>
#include <fstream>
#include <cstdlib>

#include <iostream>
#include <cmath>
#include "parser.hpp"

using namespace std;

parser::parser(const char *fileName){
	
	i=0;
	j=0;
	nblines=0;
	
	ifstream entries(fileName,ios::in);
	if (!entries){ 
		cout << "Impossible to open the " << fileName <<" file\n";   
		exit(-1);
	}
  
	// the length of data is fixed equal to the number of lines of the parameter file 
	// (even if the comment lines should be eliminated)
  
	// Find the number of lines of the parameters file
  
	while (!entries.eof()){			// while the end of the file is not attained 	
		getline (entries, ch);		// read a line
		nblines ++;					// nblines is the number of lines of the parameter file
	}
	
	entries.clear();
	entries.seekg(0,ios::beg);		// the file will be read again from the beginning
	
	data = new string[nblines];
  
	// data is a tabular where the useful lines of the input parameter are copied
	// the comments (with a #) and the empty lines are not copied
  
	  while (!entries.eof()){
		getline (entries, ch);		// the whole line is copied in the string ch
		found = ch.find("#");
		j = int(found);				// j is the position of the first #
    if (ch!="" && j!=0){			// if the line is not empty and if it is not a comment line (which begins with a #)
		if (j>0){					// if there is a comment (which begins with a #) after the datas
			ch.erase (ch.begin()+j, ch.end());			// erase the comment
		}
		data[i] = ch;
		i++;
		}
	}
	
	nblines=i-1;					// number of non-empty lines of data
  
	entries.close();
}

string parser::GetValue(const char *tag){
  
	// Find the line of the tabular data that contains the string tag and copy the line in value
  
	i=0; j=0;
  
	while (j<=0 && i<=nblines){
		value = data[i];
		found=value.find(tag);
		j=int(found);  
		i++;
	}
  
	// If the string tag is not in the parameters file, the program must stop 
  
	if (j<=0){
		cout<< "No entry for the variable "<< tag<< endl;
		exit(-1);
	}
  
	// In value, erase the description of the variable
  
	found = value.find("::");	// j is the position of the ::
	j=int(found);
	if (j==-1){					// if the :: are not found
		cout << "Bad syntax for "<< tag<<" . The syntax is: description <variable>:: value "<<endl;
		exit(-1);
	}
  
	value.erase (0, j+2);	// erase the description of the variable, before the ::
	
	value.erase (0, int(value.find_first_not_of(" ")));		// erase the white spaces before the string 
		
	value.erase (value.begin()+int(value.find_last_not_of(" "))+1, value.end());	// erase the white spaces after the string
	
  return value;
}

parser::~parser(){
   delete [] data;
} 

