/******************************************************************************
 ************* choice_limiter.cpp *********************************************
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

#include "choice_limiter.hpp"

choice_limiter::choice_limiter(int choice){
	switch (choice){
		case 1:
			Lim = new Minmod();
			break;
		case 2:
			Lim = new VanAlbada();
			break;
	        case 3:
		        Lim = new VanLeer();
		        break;
		default :
		  cerr << "WARNING: wrong LIMITER " << choice <<", using Minmod instead"<<endl;
		  cerr << "*********************************************************" << endl;
		  Lim = new Minmod();
		  
	}
}

void choice_limiter::calcul(SCALAR a,SCALAR b){
	Lim->calcul(a,b);
}

SCALAR choice_limiter::get_rec() const{
	return Lim->get_rec();
}

choice_limiter::~choice_limiter(){
	if (Lim != NULL){
		delete Lim;
		Lim = NULL;
	}
}

