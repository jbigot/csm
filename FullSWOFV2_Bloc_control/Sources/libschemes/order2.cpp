#include "misc.hpp"
#include "order2.hpp"
#include "order2_parallel.hpp"

order2::order2(parameters * &par):scheme(par) {

  // variable avec cellule fictive
  hsa.init(head,0.);
  usa.init(head,0.);
  vsa.init(head,0.);

  // variable sans cellule fictive
  qsa1.init(head,0.);
  qsa2.init(head,0.);

  // variable a l'interface en x
  z1l.init(head2,0.);
  z1r.init(head2,0.);

  // variable a l'interface en y
  z2l.init(head3,0.);
  z2r.init(head3,0.);

  rec = rec_muscl::getSingleton(par->get_lim());

}

order2::~order2() {

    hsa.erase();
    usa.erase();
    vsa.erase();
    qsa1.erase();
    qsa2.erase();
    z1l.erase();
    z1r.erase();
    z2l.erase();
    z2r.erase();

    if (rec != NULL) {
        delete rec;
        rec = NULL;
    }

}

void order2:: Heun_hq(TAB & he, TAB0 & qe1, TAB0 & qe2, TAB & hes, TAB0 & qes1, TAB0 & qes2) {

  vTAB0 inputs;
  vTAB outputs;
  outputs.push_back(he);
  inputs.push_back(qe1);
  inputs.push_back(qe2);
  outputs.push_back(hes);
  inputs.push_back(qes1);
  inputs.push_back(qes2);

  ApplyList<SCALAR,0,SCALAR,2,false>::apply(Heun_hq_f,inputs,outputs);
}

void order2::calcul() {

    //time iteration's beginning
    Chronometer chrono;
    chrono.start();

    //while (T > tps) {
    while(tps<10){
        // save the data in huv_movie.dat
      //if (n % nsave == 0) {
      //out->write(h,u,v,z,tps);
      //}// end if

        //boundary conditions
        boundary(h,u,v);

        vTAB inputs;
	vTAB0 outputs;

        inputs.push_back(h);
        inputs.push_back(u);
        inputs.push_back(v);
        inputs.push_back(z);
        outputs.push_back(h1l);
        outputs.push_back(h1r);
        outputs.push_back(u1l);
        outputs.push_back(u1r);
        outputs.push_back(v1l);
        outputs.push_back(v1r);
        outputs.push_back(z1l);
        outputs.push_back(z1r);

        ApplyList<SCALAR,2,SCALAR,0,false>::apply(order2_huv1_f,inputs,outputs);

        outputs.clear();
        outputs.push_back(h2l);
        outputs.push_back(h2r);
        outputs.push_back(u2l);
        outputs.push_back(u2r);
        outputs.push_back(v2l);
        outputs.push_back(v2r);
        outputs.push_back(z2l);
        outputs.push_back(z2r);

	ApplyList<SCALAR,2,SCALAR,0,false>::apply(order2_huv2_f,inputs,outputs);

        inputs.clear();
        outputs.clear();
	
        //------------delz1,delz2,delzc1,delzc2------------//
        //delz1
        ApplyBinary<SCALAR,0,SCALAR,SCALAR,0,false>::apply(order2_delz_f,z1l,z1r,delz1);
        //delzc1
        ApplyBinary<SCALAR,0,SCALAR,SCALAR,0,false>::apply(order2_delzc1_f,z1l,z1r,delzc1);
        //delz2
        ApplyBinary<SCALAR,0,SCALAR,SCALAR,0,false>::apply(order2_delz_f,z2l,z2r,delz2);
        //delz1
        ApplyBinary<SCALAR,0,SCALAR,SCALAR,0,false>::apply(order2_delzc2_f,z2l,z2r,delzc2);
        //-------------------------------------------------//

        bloc1(flux_left,flux_right,flux_bottom,flux_top);
        bloc2(h,u,v,q1,q2,hs,qs1,qs2);
        check_ve_ca(hs,us,vs,qs1,qs2);
        //----------------------------------------------//

        //boundary conditions
        boundary(hs,us,vs);

	inputs.push_back(hs);
        inputs.push_back(us);
        inputs.push_back(vs);
        inputs.push_back(z);
        outputs.push_back(h1l);
        outputs.push_back(h1r);
        outputs.push_back(u1l);
        outputs.push_back(u1r);
        outputs.push_back(v1l);
        outputs.push_back(v1r);
        outputs.push_back(z1l);
        outputs.push_back(z1r);

        ApplyList<SCALAR,2,SCALAR,0,false>::apply(order2_huv1_f,inputs,outputs);

        outputs.clear();
        outputs.push_back(h2l);
        outputs.push_back(h2r);
        outputs.push_back(u2l);
        outputs.push_back(u2r);
        outputs.push_back(v2l);
        outputs.push_back(v2r);
        outputs.push_back(z2l);
        outputs.push_back(z2r);

        ApplyList<SCALAR,2,SCALAR,0,false>::apply(order2_huv2_f,inputs,outputs);

        inputs.clear();
        outputs.clear();

        //------------delz1,delz2,delzc1,delzc2------------//
        //delz1
        ApplyBinary<SCALAR,0,SCALAR,SCALAR,0,false>::apply(order2_delz_f,z1l,z1r,delz1);
        //delzc1
        ApplyBinary<SCALAR,0,SCALAR,SCALAR,0,false>::apply(order2_delzc1_f,z1l,z1r,delzc1);
        //delz2
        ApplyBinary<SCALAR,0,SCALAR,SCALAR,0,false>::apply(order2_delz_f,z2l,z2r,delz2);
        //delzc1
        ApplyBinary<SCALAR,0,SCALAR,SCALAR,0,false>::apply(order2_delzc2_f,z2l,z2r,delzc2);
        //-------------------------------------------------//
        bloc1(flux_left,flux_right,flux_bottom,flux_top);
        bloc2(hs,us,vs,qs1,qs2,hsa,qsa1,qsa2);
        //----------------------------------------------//
	Heun_hq(h,q1,q2,hsa,qsa1,qsa2);
        check_ve_ca(h,u,v,q1,q2);

        tps += 1;//dt;
        n++;

	//out->write(h,u,v,z,tps);

    } //end for n : loop in time

    //out->write(h,u,v,z,tps);

    chrono.stop();
    std::stringstream st;
    st<<" Time TOTAL CALCULATION : "<<chrono.dureeCalcule()<<" ";
    Mpi_::printScreen(st.str());


}
