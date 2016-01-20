/******************************************************************************
 ************* rain_read.cpp **************************************************
 ******************************************************************************
 * 
 * Copyright CNRS - Universite d'Orleans - BRGM (France)
 * contributor: Christian Laguerre (2011)
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


#include "rain_read.hpp"
#include "rain_parallel.hpp"

extern SCALAR rain_intensity;

rain_read::rain_read(parameters * &par):rain(par){
  SCALAR time_value, intensity_value; 
  string line;

  rain_namefile = par->get_rainNameFile();
  
  ifstream prain(rain_namefile.c_str(),ios::in);
  if (!prain){ 
    cout << "impossible to open the rain file\n"; // if the name of the file is different, then impossible to open
    exit(-1);
  }
  
  /*------------------------------------------------------------------------------ */  
  /* rain_tab is a table will contain the intensity or rain (mm/h)                 */
  /* rain_tps  will have the list of the time to a change of intensity              */
  while (!prain.eof()){			// while the end of the file is not attained 	
    getline (prain, line, '\n');       // read a line
    istringstream entree (line);		
    if (entree >> time_value >> intensity_value) {
      rain_tps.insert(rain_tps.end(),time_value);
      rain_tab.insert(rain_tab.end(),intensity_value);
    }
  }
  rain_tps.insert(rain_tps.end(),DBL_MAX); //The last time is the Maximum finite representable floating-point number,
                                           //so it's not necessary to write the final time in the rain file.


  ind =0;                      /*The index of the table rain_tps is initialized to zero*/
} 
rain_read::~rain_read(){
} 

void rain_read:: Rain_func(SCALAR time, TAB & Tab_rain){
  
  const int nodex=Nxcell+1;
  const int nodey=Nycell+1;
  

  if (time >= rain_tps[ind]){
    prev_ind=ind;
    ind++;

  rain_intensity=rain_tab[prev_ind];
  apply(rain_f,Tab_rain,Tab_rain);
  }
}
