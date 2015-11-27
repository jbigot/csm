/******************************************************************************
 ************* F_HLL.cpp ******************************************************
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

#include "F_HLL.hpp"

using namespace std;


F_HLL::F_HLL(){
}


void F_HLL::calcul(SCALAR hg,SCALAR ug,SCALAR vg,SCALAR hd,SCALAR ud,SCALAR vd){

	if (hg<=0. && hd<=0.){
		f1 = 0.;
		f2 = 0.;
		f3 = 0.;
		cfl = 0.;
	}else{
		SCALAR grav_hg = grav*hg;
		SCALAR grav_hd = grav*hd;
		SCALAR qd = ud*hd;
		SCALAR qg = ug*hg;
		SCALAR c1 = min(ug-sqrt(grav_hg),ud-sqrt(grav_hd));
		SCALAR c2 = max(ug+sqrt(grav_hg),ud+sqrt(grav_hd));

		//cfl is the velocity to calculate the real cfl=max(fabs(c1),fabs(c2))*tx with tx=dt/dx
		if (c1==0. && c2==0.){ //dry
			f1=0.;
			f2=0.;
			f3=0.;
			cfl=0.; //max(fabs(c1),fabs(c2))=0
		}else if (c1>=0.){ //supercritical flow, from left to right : we have max(abs(c1),abs(c2))=c2>0 
			f1=qg;
			f2=qg*ug+grav*hg*hg*0.5;
			f3=qg*vg;
			cfl=c2; //max(fabs(c1),fabs(c2))=c2>0
		}else if (c2<=0.){ //supercritical flow, from right to left : we have max(abs(c1),abs(c2))=-c1>0
			f1=qd;
			f2=qd*ud+grav*hd*hd*0.5;
			f3=qd*vd;
			cfl=fabs(c1); //max(fabs(c1),fabs(c2))=fabs(c1)
		}else{ //subcritical flow
		        SCALAR tmp = 1./(c2-c1);
			f1=(c2*qg-c1*qd)*tmp+c1*c2*(hd-hg)*tmp;
			f2=(c2*(qg*ug+grav*hg*hg*0.5)-c1*(qd*ud+grav*hd*hd*0.5))*tmp+c1*c2*(qd-qg)*tmp;
			f3=(c2*(qg*vg)-c1*(qd*vd))*tmp+c1*c2*(hd*vd-hg*vg)*tmp;
			cfl=max(fabs(c1),fabs(c2));
		}
	}
}

F_HLL::~F_HLL(){}

