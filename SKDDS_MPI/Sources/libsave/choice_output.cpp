#include "choice_output.hpp"

choice_output * choice_output::singleton=NULL;

choice_output::choice_output(parameters * &par) {
    switch (par->get_output()) {
        /*case 1:
        	out = new gnuplot(par);
        	break;*/

    case 2:
        out = new vtk_out(par);
        break;

    default:

        cerr << "WARNING: wrong OUTPUT"<<endl;
        cerr << "*********************************************************" << endl;
        exit(EXIT_FAILURE);

    }
}

choice_output * choice_output::getSingleton() {
    if(NULL==singleton) {
        parameters * par = parameters::getSingleton();
        singleton = new choice_output(par);
    }
    return singleton;
}

void choice_output::write(TAB & h, TAB & u, TAB & v, TAB & z, SCALAR tps) {
    out->write(h,u,v,z,tps);
}

choice_output::~choice_output() {
    if (out != NULL) {
        delete out;
        out = NULL;
    }
}
