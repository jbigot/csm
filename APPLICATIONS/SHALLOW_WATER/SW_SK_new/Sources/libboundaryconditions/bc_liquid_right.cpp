#include "bc_liquid_right.hpp"

bc_liquid_right::bc_liquid_right(): boundary_condition() {
    unormbound = 0.;
    utanbound = 0.;
    hbound = 0.;
}


void bc_liquid_right::calcul(SCALAR hin, SCALAR unorm_in, SCALAR utan_in, SCALAR hfix, SCALAR qfix) {

    if (hfix<he_ca) {
        unormfix = 0.;
    } else {
        unormfix = qfix/hfix;
    }

    if (hin<he_ca || fabs(unorm_in)-sqrt(grav*hin)>0.) { // dry or supercritical at left side
        if(unorm_in < 0.) { // inflow open boundary
            hbound = hfix;
            unormbound = unormfix;
        } else { //outflow open boundary
            hbound = hin;
            unormbound = unorm_in;
        }

    } else { // sub critical (fluvial)
        hbound=hfix;
        unormbound=unorm_in+2.*sqrt(grav)*(sqrt(hin)-sqrt(hbound));
        if (fabs(unormbound)-sqrt(grav*hbound)>0.) {
            if(unorm_in < 0.) { // inflow open boundary
                hbound = hfix;
                unormbound = unormfix;
            } else { //outflow open boundary
                hbound = hin;
                unormbound = unorm_in;
            }
        }
    }
    utanbound = utan_in;
}

bc_liquid_right::~bc_liquid_right() {
}
