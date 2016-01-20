#include "bc_neumann.hpp"

bc_neumann::bc_neumann(): boundary_condition(){
  unormbound = 0.;
  utanbound = 0.;
  hbound = 0.;
}

void bc_neumann::calcul(SCALAR hin,SCALAR unorm_in,SCALAR utan_in,SCALAR hfix,SCALAR qfix){
	hbound = hin;
	unormbound = unorm_in;
	utanbound = utan_in;
}

bc_neumann::~bc_neumann(){
}
