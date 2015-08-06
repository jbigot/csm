#ifndef Class_parameters
#include "parameters.hpp"
#endif

#ifndef Class_choice_limiter
#include "choice_limiter.hpp"
#endif

#ifndef Class_rec_muscl
#define Class_rec_muscl

class rec_muscl {
private :
    rec_muscl(int);
    static rec_muscl * singleton;
    choice_limiter * limiter;
    SCALAR hl,hr,ul,ur,vl,vr,zl,zr;
    SCALAR dh,dh_z,du,dv;

public :
    static rec_muscl * getSingleton(int);

    /*!
     * \brief Calculates the rec_muscl reconstruction
     */
    void calcul(SCALAR,SCALAR,SCALAR,SCALAR,SCALAR,SCALAR,SCALAR,SCALAR,SCALAR,SCALAR,SCALAR,SCALAR);

    SCALAR get_hl();
    SCALAR get_hr();
    SCALAR get_ul();
    SCALAR get_ur();
    SCALAR get_vl();
    SCALAR get_vr();
    SCALAR get_zl();
    SCALAR get_zr();

    virtual ~ rec_muscl();
};
#endif
