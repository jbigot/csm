#include "boundary_condition.hpp"

boundary_condition::boundary_condition() {}

SCALAR boundary_condition::get_hbound() const {
    return hbound;
}

SCALAR boundary_condition::get_unormbound() const {
    return unormbound;
}

SCALAR boundary_condition::get_utanbound() const {
    return utanbound;
}


boundary_condition::~boundary_condition() {}
