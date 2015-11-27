#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>

#include "choice_scheme.hpp"
#include "parameters.hpp"

using namespace std;

int main(int argc, char ** argv) {
    INITSKELGIS;
    parameters * par = parameters::getSingleton();
    par->setparameters("./Inputs/parameters.txt");
    choice_scheme * scheme;
    scheme = new choice_scheme(par);
//    cout << "scheme chosen" << endl;
    scheme->calcul();
//    cout << "\nComputation finished!" << endl;
    delete scheme;
    delete par;
    ENDSKELGIS;
    return 0;
}
