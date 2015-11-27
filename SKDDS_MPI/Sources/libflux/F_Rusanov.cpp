/******************************************************************************
 ************* F_Rusanov.cpp **************************************************
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

#include "F_Rusanov.hpp"

using namespace std;


F_Rusanov::F_Rusanov(){
}

void F_Rusanov::calcul(SCALAR hg,SCALAR ug,SCALAR vg,SCALAR hd,SCALAR ud,SCALAR vd){

	SCALAR c;
	if (hg<=0. && hd<=0.){
		c = 0.;
		f1 = 0.;
		f2 = 0.;
		f3 = 0.;
		cfl = 0.;
	}else{
		c = max(fabs(ug)+sqrt(grav*hg),fabs(ud)+sqrt(grav*hd));
		SCALAR cd = c*0.5;
		SCALAR qd = ud*hd;
		SCALAR qg = ug*hg;
		f1 = (qg+qd)*0.5-cd*(hd-hg);
		f2 = ((ug*qg)+(grav_dem*hg*hg)+(ud*qd)+(grav_dem*hd*hd))*0.5-cd*(qd-qg);
		f3 = (qg*vg+qd*vd)*0.5-cd*(hd*vd-hg*vg);
		cfl = c;//*tx;
	}
}

F_Rusanov::~F_Rusanov(){}
