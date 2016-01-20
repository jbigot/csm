#include "bc_discharge_left.hpp"

bc_discharge_left::bc_discharge_left(): boundary_condition() {

    tol = 1e-5;
    maxiter = 100;
    unormbound = 0.;
    utanbound = 0.;
    hbound = 0.;
}

void bc_discharge_left::calcul(SCALAR hin, SCALAR unorm_in, SCALAR utan_in, SCALAR hfix, SCALAR qfix) {
    if (hfix<he_ca) {
        unormfix = 0.;
    } else {
        unormfix = qfix/hfix;
    }
    if (hin<he_ca || fabs(unorm_in)-sqrt(grav*hin)>0.) { // dry or supercritical at right side
        if(unorm_in >0.) { // inflow open boundary
            hbound = hfix;
            unormbound = unormfix;
        } else { //outflow open boundary
            hbound = hin;
            unormbound = unorm_in;
        }

    } else {
        int iter = 0;
        SCALAR norm,F,gradF,graddF,h_tmp,alpha,c;
        alpha = unorm_in-2.*sqrt(grav*hin);
        h_tmp = hin; // init value
        c = sqrt(grav*h_tmp);
        F = 2.*c*h_tmp+alpha*h_tmp-qfix;
        norm = fabs(F);
        while ((norm > tol) && (iter <= maxiter)) {
            gradF = 3.*c+alpha;
            graddF = 1.5*sqrt(grav/(h_tmp));
            h_tmp += - (F*gradF)/(pow(gradF,2.)-F*graddF);
            c = sqrt(grav*h_tmp);
            F = 2.*c*h_tmp+alpha*h_tmp-qfix;
            norm = fabs(F);
            iter += 1;
        }// end while
        if (iter == maxiter+1) {
            cout << "Warning: Newton solver did not converge" << endl;
        }
        hbound = h_tmp;
        unormbound = qfix/hbound;
        if (fabs(unormbound)-sqrt(grav*hbound)>0.) {
            if(unorm_in >0.) { // inflow open boundary
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


bc_discharge_left::~bc_discharge_left() {
}
