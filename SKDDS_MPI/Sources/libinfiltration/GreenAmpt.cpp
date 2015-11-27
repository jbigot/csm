/******************************************************************************
 ************* GreenAmpt.cpp **************************************************
 ******************************************************************************
 * 
 * Copyright CNRS - Universite d'Orleans - BRGM (France)
 * contributor: Marie Rousseau (2008)
 * ma.rousseau@brgm.fr
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

#include "GreenAmpt.hpp"

GreenAmpt::GreenAmpt(parameters & par):infiltration(par){
  imax = par.get_imax();
}

SCALAR GreenAmpt:: capacity(SCALAR h,SCALAR Vin_tot,SCALAR dt,SCALAR Kc,SCALAR Ks, SCALAR dtheta,SCALAR Psi, SCALAR zcrust){
  SCALAR Ic;
      if (Vin_tot<=zcrust*dtheta){
	Ic=Kc+ Kc*(Psi+h)*dtheta/Vin_tot;
      }else{
	SCALAR Zf=Vin_tot/dtheta;
	Ke= Zf/((Zf-zcrust)/Ks+zcrust/Kc);
	Ic=Ke + Ke*(Psi+h)*dtheta/Vin_tot;
	
      }
      return Ic;
}


void GreenAmpt:: calcul(SCALAR h,SCALAR Vin_tot,SCALAR dt,SCALAR Kc,SCALAR Ks, SCALAR dtheta,SCALAR Psi, SCALAR zcrust){

  if (Vin_tot <= Ie_ca){      
    hmod=max(0.,h-imax*dt);
    Vin = h-hmod;
  }else{
    SCALAR Ic=capacity(h,Vin_tot,dt,Kc, Ks,  dtheta, Psi, zcrust);
    
    if (Ic>imax) Ic=imax;    

    if ( h <= Ic*dt) { 
      Vin=h;
      hmod=0.;
      
    }else{ //runoff
      
      Vin = Ic*dt;
      
      hmod = h -Ic*dt;
      
    }//end for infiltration case
  }
  
}  



GreenAmpt::~GreenAmpt(){}



