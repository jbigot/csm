#include "order1.hpp"
#include "order1_parallel.hpp"

order1::order1(parameters * & par):scheme(par) {
  ApplyUnary<SCALAR,2,SCALAR,0,false>::apply(order1_delz1_f,z,delz1);
  ApplyUnary<SCALAR,2,SCALAR,0,false>::apply(order1_delz2_f,z,delz2);
}

order1::~order1() {}

void order1::calcul() {


    //time(&start);
    //out = choice_output::getSingleton();
    //to give the initial situation
    //out->write(h,u,v,z,tps);

    //time iteration's beginning
 //    Chronometer chrono;
 //    chrono.start();
 //    while (T > tps) {
 //        // save the data in huv_movie.dat
 //    //  if (n % nsave == 0) {
 //    //  out->write(h,u,v,z,tps);
 //        //h->print();
 //    //  }// end if

 //        //boundary conditions
 //        boundary(h,u,v);

 //        vTAB inputs;
	// vTAB0 outputs;

 //        inputs.push_back(h);
 //        inputs.push_back(u);
 //        inputs.push_back(v);
 //        outputs.push_back(h1l);
 //        outputs.push_back(h1r);
 //        outputs.push_back(u1l);
 //        outputs.push_back(u1r);
 //        outputs.push_back(v1l);
 //        outputs.push_back(v1r);

 //        ApplyList<SCALAR,2,SCALAR,0,false>::apply(order1_huv1_f,inputs,outputs);

 //        outputs.clear();
 //        outputs.push_back(h2l);
 //        outputs.push_back(h2r);
 //        outputs.push_back(u2l);
 //        outputs.push_back(u2r);
 //        outputs.push_back(v2l);
 //        outputs.push_back(v2r);

 //        ApplyList<SCALAR,2,SCALAR,0,false>::apply(order1_huv2_f,inputs,outputs);

 //        inputs.clear();
 //        outputs.clear();

 //        bloc1(flux_left,flux_right,flux_bottom,flux_top);

 //        //bloc2(h,u,v,q1,q2,hs,qs1,qs2);

 //        update_hq(h,q1,q2,hs,qs1,qs2);

 //        check_ve_ca(h,u,v,q1,q2);

 //        tps += dt;
 //        n++;

 //    } //end for n : loop in time

 //    //out->write(h,u,v,z,tps);

 //    chrono.stop();
 //    std::stringstream st;
 //    st<<" Time TOTAL CALCULATION : "<<chrono.dureeCalcule()<<" ";
 //    Mpi_::printScreen(st.str());

 //    h1r.print();
 //    h1l.print();

}
