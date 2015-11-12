#include "scheme.hpp"
#include "scheme_parallel.hpp"

scheme::scheme(parameters * & par) {
    Nxcell = par->get_Nxcell();
    Nycell = par->get_Nycell();
    ORDER = par->get_order();
    T = par->get_T();
    dt=  par->get_dtfix();
    dx = par->get_dx();
    dy = par->get_dy();
    nsave = par->get_nsave();


    allocation(par);

    flux_num = choice_flux::getSingleton(par->get_flux());
    rec_hydro = hydrostatic::getSingleton();
    Lbound = choice_condition_left::getSingleton(par->get_Lbound());
    Rbound = choice_condition_right::getSingleton(par->get_Rbound());
    Bbound = choice_condition_bottom::getSingleton(par->get_Bbound());
    Tbound = choice_condition_top::getSingleton(par->get_Tbound());
//    fric = new choice_friction::getSingleton(par);
//   Prain = new ChoiceRain(par);
//   I = new choice_infiltration(par);
    out = choice_output::getSingleton();

    tps=0;
    n = 0;
    ApplyUnary<SCALAR,2,SCALAR,2,false,true>::apply(boundaryz_f,z,z);

    /*
        Vol_inf = 0.;
        Vol_rain = 0.;
        Vol_left = 0.;
        Vol_right = 0.;
        Vol_bottom = 0.;
        Vol_top = 0.;
       */
    flux_left = 0.;
    flux_right = 0.;
    flux_top = 0.;
    flux_bottom = 0.;

}

scheme::~scheme() {
    desallocation();
}

void scheme:: bloc1(SCALAR & flux_left_tmp, SCALAR & flux_right_tmp, SCALAR & flux_bottom_tmp, SCALAR & flux_top_tmp) {
  vTAB0 inputs, outputs;
  inputs.push_back(h1l);
  inputs.push_back(h1r);
  inputs.push_back(u1l);
  inputs.push_back(u1r);
  inputs.push_back(v1l);
  inputs.push_back(v1r);
  inputs.push_back(delz1);
  outputs.push_back(h1g);
  outputs.push_back(h1d);
  outputs.push_back(f1);
  outputs.push_back(f2);
  outputs.push_back(f3);
  ApplyList<SCALAR,0,SCALAR,0,false>::apply(bloc1x_f,inputs,outputs);
  ApplyList<SCALAR,0,SCALAR,0,false>::apply(bloc1x_f2,inputs,outputs);

  inputs.clear();
  outputs.clear();
  inputs.push_back(h2l);
  inputs.push_back(h2r);
  inputs.push_back(u2l);
  inputs.push_back(u2r);
  inputs.push_back(v2l);
  inputs.push_back(v2r);
  inputs.push_back(delz2);
  outputs.push_back(h2g);
  outputs.push_back(h2d);
  outputs.push_back(g1);
  outputs.push_back(g2);
  outputs.push_back(g3);
  ApplyList<SCALAR,0,SCALAR,0,false>::apply(bloc1y_f,inputs,outputs);
  ApplyList<SCALAR,0,SCALAR,0,false>::apply(bloc1y_f2,inputs,outputs);
}

void scheme:: bloc2(TAB & he, TAB & ve1, TAB & ve2, TAB0 & qe1, TAB0 & qe2, TAB & hes, TAB0 & qes1, TAB0 & qes2, TAB & us, TAB & vs) {
  vTAB inputs;
  vTAB0 outputs;
  inputs.push_back(he);
  inputs.push_back(ve1);
  inputs.push_back(ve2);
   //pb je recupere les voisins alors que autre version non
  inputs.push_back(hes);

  //check
  inputs.push_back(us);
  inputs.push_back(vs);

  //R=0 pas de pb
  outputs.push_back(qe1);
  outputs.push_back(qe2);
  outputs.push_back(h1l);
  outputs.push_back(h1r);
  outputs.push_back(h1g);
  outputs.push_back(h1d);
  outputs.push_back(h2l);
  outputs.push_back(h2r);
  outputs.push_back(h2g);
  outputs.push_back(h2d);
  outputs.push_back(delzc1);
  outputs.push_back(delzc2);
  outputs.push_back(f1);
  outputs.push_back(f2);
  outputs.push_back(f3);
  outputs.push_back(g1);
  outputs.push_back(g2);
  outputs.push_back(g3);

  //outputs.push_back(hes);
  outputs.push_back(qes1);
  outputs.push_back(qes2);

  ApplyList<SCALAR,2,SCALAR,0,false/*,true*/>::apply(bloc2_f,inputs,outputs);
}

void scheme:: bloc22(TAB & he, TAB & ve1, TAB & ve2, TAB0 & qe1, TAB0 & qe2, TAB & hes, TAB0 & qes1, TAB0 & qes2, TAB & h, TAB0 & q1, TAB0& q2) {
  vTAB inputs;
  vTAB0 outputs;
  inputs.push_back(he);
  inputs.push_back(ve1);
  inputs.push_back(ve2);
   //pb je recupere les voisins alors que autre version non
  inputs.push_back(hes);

  //heun
  inputs.push_back(h);

  //R=0 pas de pb
  outputs.push_back(qe1);
  outputs.push_back(qe2);
  outputs.push_back(h1l);
  outputs.push_back(h1r);
  outputs.push_back(h1g);
  outputs.push_back(h1d);
  outputs.push_back(h2l);
  outputs.push_back(h2r);
  outputs.push_back(h2g);
  outputs.push_back(h2d);
  outputs.push_back(delzc1);
  outputs.push_back(delzc2);
  outputs.push_back(f1);
  outputs.push_back(f2);
  outputs.push_back(f3);
  outputs.push_back(g1);
  outputs.push_back(g2);
  outputs.push_back(g3);

  //outputs.push_back(hes);
  outputs.push_back(qes1);
  outputs.push_back(qes2);

  //heun
  outputs.push_back(q1);
  outputs.push_back(q2);

  ApplyList<SCALAR,2,SCALAR,0,false/*,true*/>::apply(bloc2_f2,inputs,outputs);
}

void scheme:: update_hq(TAB & he, TAB0 & qe1, TAB0 & qe2, TAB & hes, TAB0 & qes1, TAB0 & qes2) {

  vTAB0 inputs;
  vTAB outputs;
  outputs.push_back(he);
  outputs.push_back(hes);
  //outputs.push_back(he);
  inputs.push_back(qe1);
  inputs.push_back(qe2);
  //outputs.push_back(hes);
  inputs.push_back(qes1);
  inputs.push_back(qes2);

  ApplyList<SCALAR,0,SCALAR,2,false>::apply(update_hq_f,inputs,outputs);
}

void scheme:: boundary(TAB & he,TAB & ve1 ,TAB & ve2) {

  vTAB0 inputs;
  vTAB outputs;
  outputs.push_back(he);
  outputs.push_back(ve1);
  outputs.push_back(ve2);

  ApplyList<SCALAR,0,SCALAR,2,false>::apply(boundary_f,inputs,outputs);
  /*he.printAll();
  ve1.printAll();
  ve2.printAll();*/
}

void scheme:: check_ve_ca(TAB & he, TAB & ve1, TAB & ve2, TAB0 & qe1, TAB0 & qe2) {

  vTAB0 inputs;
  inputs.push_back(qe1);
  inputs.push_back(qe2);
  vTAB outputs;
  outputs.push_back(he);
  outputs.push_back(ve1);
  outputs.push_back(ve2);
  //outputs.push_back(qe1);
  //outputs.push_back(qe2);

  ApplyList<SCALAR,0,SCALAR,2,false>::apply(ve_ca_f,inputs,outputs);
}

void scheme::allocation(parameters * & par) {
    //HEADER head;
    head.width = Nxcell;
    head.height = Nycell;
    head.x = 0;
    head.y = 0;
    head.spacing = dx;
    head.nodata = -9999;

    // variable avec cellule fictive
    //sans fichiers
    h.init(head,1.0);
    z.init(head,1.0);

    u.init(head,1.0);
    v.init(head,1.0);

    hs.init(head,1.0);
    us.init(head,1.0);
    vs.init(head,1.0);

    // variable sans cellule fictive

    q1.init(head,0.);
    q2.init(head,0.);
    qs1.init(head,0.);
    qs2.init(head,0.);
    delzc1.init(head,0.);
    delzc2.init(head,0.);

    // variable a l'interface en x
    //HEADER head2;
    head2.width = head.width+h.getCols();
    head2.height = head.height;
    head2.x = head.x;
    head2.y = head.y;
    head2.spacing = head.spacing;
    head2.nodata = head.nodata;
    f1.init(head2,0.);
    f2.init(head2,0.);
    f3.init(head2,0.);
    h1g.init(head2,0.);
    h1l.init(head2,0.);
    u1l.init(head2,0.);
    v1l.init(head2,0.);
    h1d.init(head2,0.);
    h1r.init(head2,0.);
    u1r.init(head2,0.);
    v1r.init(head2,0.);
    delz1.init(head2,0.);

    // variable a l'interface en y
    //HEADER head3;
    head3.width = head.width;
    head3.height = head.height+h.getRows();
    head3.x = head.x;
    head3.y = head.y;
    head3.spacing = head.spacing;
    head3.nodata = head.nodata;
    g1.init(head3,0.);
    g2.init(head3,0.);
    g3.init(head3,0.);
    h2g.init(head3,0.);
    h2l.init(head3,0.);
    u2l.init(head3,0.);
    v2l.init(head3,0.);
    h2d.init(head3,0.);
    h2r.init(head3,0.);
    u2r.init(head3,0.);
    v2r.init(head3,0.);
    delz2.init(head3,0.);
}

void scheme::desallocation() {
  delete Lbound;
    delete Rbound;
    delete Bbound;
    delete Tbound;
    delete flux_num;
    delete rec_hydro;

//delete Prain ;
//delete topo;
//delete huv_init;

//delete fric;
//delete I;
    delete out;
    z.erase();
    h.erase();
    u.erase();
    v.erase();
    q1.erase();
    q2.erase();
    hs.erase();
    us.erase();
    vs.erase();
    qs1.erase();
    qs2.erase();
    f1.erase();
    f2.erase();
    f3.erase();
    g1.erase();
    g2.erase();
    g3.erase();
    h1g.erase();
    h1l.erase();
    u1l.erase();
    v1l.erase();
    h1d.erase();
    h1r.erase();
    u1r.erase();
    v1r.erase();
    h2g.erase();
    h2l.erase();
    u2l.erase();
    v2l.erase();
    h2d.erase();
    h2r.erase();
    u2r.erase();
    v2r.erase();
    delz1.erase();
    delz2.erase();
    delzc1.erase();
    delzc2.erase();
}
