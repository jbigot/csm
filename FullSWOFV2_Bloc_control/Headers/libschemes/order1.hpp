#ifndef Class_scheme
#include "scheme.hpp"
#endif

#ifndef Class_order1
#define Class_order1

class order1: public scheme {

public:

    order1(parameters * &par);

    virtual ~order1();

    void calcul();
};
#endif
