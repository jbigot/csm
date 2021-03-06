#ifndef Class_scheme
#define Class_scheme

#include "misc.hpp"
#include "parameters.hpp"

#ifndef Class_hydrostatic
#include "hydrostatic.hpp"
#endif

#ifndef Class_choice_condition_left
#include "choice_condition_left.hpp"
#endif

#ifndef Class_choice_condition_right
#include "choice_condition_right.hpp"
#endif

#ifndef Class_choice_condition_bottom
#include "choice_condition_bottom.hpp"
#endif

#ifndef Class_choice_condition_top
#include "choice_condition_top.hpp"
#endif

#ifndef Class_choice_flux
#include "choice_flux.hpp"
#endif

//#ifndef Class_choice_friction
//#include "choice_friction.hpp"
//#endif

//#ifndef Class_choice_infiltration
//#include "choice_infiltration.hpp"
//#endif

//#ifndef Class_ChoiceRain
//#include "ChoiceRain.hpp"
//#endif

#ifndef Class_choice_output
#include "choice_output.hpp"
#endif

class scheme {
protected :
    int Nxcell,Nycell,ORDER,n,nsave;
    SCALAR T,dx,dy;
    time_t start,end;
    SCALAR dt,tps,timecomputation;
    SCALAR flux_left,flux_right,flux_bottom,flux_top;
//    SCALAR Vol_rain,Vol_of,Vol_inf,Vol_left,Vol_right,Vol_bottom,Vol_top;
//    SCALAR h_rain;

    TAB h,u,v,z;
    TAB0 q1, q2;

    TAB hs,us,vs;
    TAB0 qs1,qs2;

    TAB0 f1, f2, f3;
    TAB0 g1, g2, g3;

    TAB0 h1g, h1d;
    TAB0 h2g, h2d;
    TAB0 delz1, delz2;

    TAB0 h1r,u1r,v1r,h1l,u1l,v1l,h2r,u2r,v2r,h2l,u2l,v2l;
    TAB0 delzc1,delzc2;

    HEADER head,head2,head3;

    hydrostatic * rec_hydro;
    choice_condition_left * Lbound;
    choice_condition_right * Rbound;
    choice_condition_bottom * Bbound;
    choice_condition_top * Tbound;
    choice_flux * flux_num;
//    choice_friction * fric;
//    choice_infiltration * I;
//    choice_init_topo * topo;
//    choice_init_huv * huv_init;
//   ChoiceRain * Prain;

    choice_output * out;

  
public:

    scheme(parameters * &);

    void bloc1(SCALAR &, SCALAR &, SCALAR &, SCALAR &);

    void bloc2(TAB &, TAB &, TAB &, TAB0 &, TAB0 &, TAB &, TAB0 &, TAB0 &, TAB &, TAB &);
    void bloc22(TAB &, TAB &, TAB &, TAB0 &, TAB0 &, TAB &, TAB0 &, TAB0 &, TAB &, TAB0 &, TAB0&);

    void update_hq(TAB &, TAB0 &, TAB0 &, TAB &, TAB0 &, TAB0 &);

    void boundary(TAB &,TAB &,TAB &);

    void check_ve_ca(TAB &, TAB &, TAB &, TAB0 &, TAB0 &);

    void allocation(parameters * &);

    void desallocation();

    virtual void calcul() =0;

    virtual ~scheme();
};
#endif
