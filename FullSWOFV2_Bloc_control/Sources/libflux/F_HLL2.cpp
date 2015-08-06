/******************************************************************************
 ************* F_HLL2.cpp *****************************************************
 ******************************************************************************
 * 
 * Copyright CNRS - Universite d'Orleans - BRGM (France)
 * contributors: Olivier Delestre (2008) and Christian Laguerre (2010)
 * olivierdelestre41@yahoo.fr and christian.laguerre@math.cnrs.fr
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

#include "F_HLL2.hpp"

using namespace std;


F_HLL2::F_HLL2(){
}


void F_HLL2::calcul(SCALAR hg,SCALAR ug,SCALAR vg,SCALAR hd,SCALAR ud,SCALAR vd){

	if (hg<=0. && hd<=0.){
		f1 = 0.;
		f2 = 0.;
		f3 = 0.;
		cfl = 0.;
	}else{
		SCALAR grav_hg = grav*hg;
		SCALAR grav_hd = grav*hd;
		SCALAR sqrt_grav_hg = sqrt(grav_hg);
		SCALAR sqrt_grav_hd = sqrt(grav_hd);
		SCALAR qd = ud*hd;
		SCALAR qg = ug*hg;
		SCALAR c1 = min(ug-sqrt_grav_hg,ud-sqrt_grav_hd); //we already have ug-sqrt_grav_hg<ug+sqrt_grav_hg and ud-sqrt_grav_hd<ud+sqrt_grav_hd 
		SCALAR c2 = max(ug+sqrt_grav_hg,ud+sqrt_grav_hd); //so we do not need all the eigenvalues to get c1 and c2
		SCALAR tmp = 1./(c2-c1);
		SCALAR t1 = (min(c2,0.)-min(c1,0.))*tmp;
		SCALAR t2 = 1.-t1;
		SCALAR t3 = (c2*fabs(c1)-c1*fabs(c2))*0.5*tmp;

		f1 = t1*qd+t2*qg-t3*(hd-hg);
		f2 = t1*(qd*ud+grav_hd*hd*0.5)+t2*(qg*ug+grav_hg*hg*0.5)-t3*(qd-qg);
		f3 = t1*qd*vd+t2*qg*vg-t3*(hd*vd-hg*vg);
		cfl = max(fabs(c1),fabs(c2)); //cfl is the velocity to compute the cfl condition max(fabs(c1),fabs(c2))*tx with tx=dt/dx
	}
}

F_HLL2::~F_HLL2(){}

