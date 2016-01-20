/******************************************************************************
 ************* choice_flux.cpp ************************************************
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

#include "choice_flux.hpp"

choice_flux * choice_flux::singleton = NULL;

/*choice_flux::choice_flux(string choice){*/
choice_flux::choice_flux(int choice){
	switch (choice){
/*	case "Rusanov":*/
	case 1:
		F = new F_Rusanov();
		break;
/*	case "HLL":*/
	case 2:
		F = new F_HLL();
		break;
	case 3:
		F = new F_HLL2();
		break;
	default:
	  cerr << "WARNING: wrong NUMERICAL FLUX " << choice <<", using HLL instead"<<endl;
	  cerr << "*********************************************************" << endl;
	  F = new F_HLL();
	}
}

choice_flux * choice_flux::getSingleton(int choice)
{
	if(NULL==singleton)
	{
		singleton = new choice_flux(choice);
	}
	return singleton;
}

void choice_flux::calcul(SCALAR hg,SCALAR ug,SCALAR vg,SCALAR hd,SCALAR ud,SCALAR vd){
	F->calcul(hg,ug,vg,hd,ud,vd);
}

void choice_flux::set_tx(SCALAR tx){
	F->set_tx(tx);
}

SCALAR choice_flux::get_f1(){
	return F->get_f1();
}

SCALAR choice_flux::get_f2(){
	return F->get_f2();
}

SCALAR choice_flux::get_f3(){
	return F->get_f3();
}

SCALAR choice_flux::get_cfl(){
	return F->get_cfl();
}

choice_flux::~choice_flux(){
	if (F!=NULL){
		delete F;
		F = NULL;
	}
}
