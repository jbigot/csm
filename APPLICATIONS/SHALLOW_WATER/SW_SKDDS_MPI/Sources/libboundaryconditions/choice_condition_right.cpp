#include "choice_condition_right.hpp"

choice_condition_right * choice_condition_right::singleton = NULL;

choice_condition_right::choice_condition_right(int choice) {
    switch (choice) {
    case 1:
        C = new bc_wall();
        break;
    case 2:
        C = new bc_neumann();
        break;
    case 3:
        C = new bc_liquid_right();
        break;
    case 4:
        C = new bc_discharge_right();
        break;
    default:
        cerr << "WARNING: wrong BOUNDARY CONDITION " << choice <<", change the parameters file"<<endl;
        cerr << "*********************************************************" << endl;
        exit(EXIT_FAILURE);
    }
}

choice_condition_right * choice_condition_right::getSingleton(int choice)
{
	if(NULL==singleton)
	{
		singleton = new choice_condition_right(choice);
	}
	return singleton;
}

void choice_condition_right::calcul(SCALAR hin, SCALAR unorm_in, SCALAR utan_in, SCALAR hfix, SCALAR qfix) {
    C->calcul(hin,unorm_in,utan_in,hfix,qfix);
}

SCALAR choice_condition_right::get_hbound() {
    return C->get_hbound();
}

SCALAR choice_condition_right::get_unormbound() {
    return C->get_unormbound();
}

SCALAR choice_condition_right::get_utanbound() {
    return C->get_utanbound();
}

choice_condition_right::~choice_condition_right() {
    if (C!=NULL) {
        delete C;
        C=NULL;
    }
}
