#ifndef MISC_H
#define MISC_H

#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <cfloat>
#include <vector>

#ifndef SKELGIS_H
#include "../libskelgis/skelgis.hpp"
#endif

#include "../libskelgis/util/chronometer.hpp"

#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)
#define grav 9.81
#define grav_dem 4.905

#define he_ca 1.e-6
#define ve_ca 1.e-6

#define NB_CHAR 256
#define zero 0.
#define Ie_ca 1.e-8
#define Ic_max 1.e-3
#define epsilon 1.e-10
# define version "FullSWOF_2D - version v1.01.00 - October, 6, 2011"
/*NB_CHAR is the maximum length of a comment line */

using namespace std;
using namespace skelgis;

typedef double SCALAR;
typedef DMatrix<SCALAR,2,false> TAB;
typedef DMatrix<SCALAR,0,false> TAB0;
typedef DMatrix_vector<SCALAR,2,false> vTAB;
typedef DMatrix_vector<SCALAR,0,false> vTAB0;
typedef skelgis::iterator<SCALAR,2> iTAB;
typedef skelgis::iterator<SCALAR,0> iTAB0;
typedef skelgis::iterator_line<SCALAR,2> iTABL;
typedef skelgis::iterator_line<SCALAR,0> iTAB0L;


#endif

