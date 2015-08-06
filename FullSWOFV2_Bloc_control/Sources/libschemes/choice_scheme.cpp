#include "choice_scheme.hpp"

choice_scheme::choice_scheme(parameters * & par) {
    switch (par->get_order()) {
    case 1:
        Sche = new order1(par);
        break;
    case 2:
        Sche = new order2(par);
        break;
    default:
        cerr << "WARNING: wrong ORDER OF THE SCHEME " << par->get_order() <<", using order 1 instead "<<endl;
        cerr << "*********************************************************" << endl;
        Sche = new order1(par);
    }
}

void choice_scheme::calcul() {
    Sche->calcul();
}

choice_scheme::~choice_scheme() {
    delete Sche;
}

