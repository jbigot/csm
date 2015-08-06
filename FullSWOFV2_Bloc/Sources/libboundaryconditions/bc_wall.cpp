#include "bc_wall.hpp"

bc_wall::bc_wall(): boundary_condition() {
  unormbound = 0.;
  utanbound = 0.;
  hbound = 0.;
}

void bc_wall::calcul(SCALAR hin,SCALAR unorm_in,SCALAR utan_in,SCALAR hfix,SCALAR qfix) {
    unormbound = -unorm_in;
    utanbound = utan_in;
    hbound = hin;
}

bc_wall::~bc_wall() {
}
