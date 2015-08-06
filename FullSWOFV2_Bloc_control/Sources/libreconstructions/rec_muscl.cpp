#include "rec_muscl.hpp"


rec_muscl::rec_muscl(int lim) {
    limiter = new choice_limiter(lim);
}

//-------------------
rec_muscl * rec_muscl::singleton=NULL;
rec_muscl * rec_muscl::getSingleton(int lim) {
    if(singleton==NULL)
        singleton =  new rec_muscl(lim);
    return singleton;
}
//-------------------

//rec_muscl reconstructions (left and right)
void rec_muscl::calcul(SCALAR h,SCALAR u,SCALAR v, SCALAR z, SCALAR dh1, SCALAR dh2, SCALAR du1, SCALAR du2, SCALAR dv1, SCALAR dv2, SCALAR dz1, SCALAR dz2) {
    limiter->calcul(dh1,dh2);
    dh = limiter->get_rec();
    hl = h-0.5*dh;
    hr = h+0.5*dh;
    limiter->calcul(dh1+dz1,dh2+dz2);
    dh_z = limiter->get_rec();
    zl = z-0.5*(dh_z-dh);
    zr = z+0.5*(dh_z-dh);
    limiter->calcul(du1,du2);
    du = limiter->get_rec();
    limiter->calcul(dv1,dv2);
    dv = limiter->get_rec();
    if(h>he_ca) {
        ul=u-0.5*du*hr/h;
        ur=u+0.5*du*hl/h;
        vl=v-0.5*dv*hr/h;
        vr=v+0.5*dv*hl/h;
    } else {
        ul=u-0.5*du;
        ur=u+0.5*du;
        vl=v-0.5*dv;
        vr=v+0.5*dv;
    }
}

SCALAR rec_muscl::get_hl() {
    return hl;
}

SCALAR rec_muscl::get_hr() {
    return hr;
}

SCALAR rec_muscl::get_ul() {
    return ul;
}

SCALAR rec_muscl::get_ur() {
    return ur;
}

SCALAR rec_muscl::get_vl() {
    return vl;
}

SCALAR rec_muscl::get_vr() {
    return vr;
}

SCALAR rec_muscl::get_zl() {
    return zl;
}

SCALAR rec_muscl::get_zr() {
    return zr;
}

rec_muscl::~rec_muscl() {
    if (limiter!=NULL) {
        delete limiter;
        limiter = NULL;
    }
}
