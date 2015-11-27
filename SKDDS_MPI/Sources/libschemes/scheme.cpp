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

  //no communications

  Controller<double,0,false> ch1l(h1l.getDMatrix());
  Controller<double,0,false> ch1r(h1r.getDMatrix());
  Controller<double,0,false> cdelz1(delz1.getDMatrix());
  Controller<double,0,false> ch1g(h1g.getDMatrix());
  Controller<double,0,false> ch1d(h1d.getDMatrix());

    for(int64_t yy = ch1l.start(); yy<ch1l.height();yy++)
    {
      for(int64_t xx = ch1l.start();xx<ch1l.width(); xx++)
      {
        ch1g(xx,yy)=max(0.,ch1l(xx,yy)-max(0.,cdelz1(xx,yy)));
        ch1d(xx,yy)=max(0.,ch1r(xx,yy)-max(0.,-cdelz1(xx,yy)));
      }
    }
  Controller<double,0,false> cu1l(u1l.getDMatrix());
  Controller<double,0,false> cu1r(u1r.getDMatrix());
  Controller<double,0,false> cv1l(v1l.getDMatrix());
  Controller<double,0,false> cv1r(v1r.getDMatrix());
  Controller<double,0,false> cf1(f1.getDMatrix());
  Controller<double,0,false> cf2(f2.getDMatrix());
  Controller<double,0,false> cf3(f3.getDMatrix());

  for(int64_t yy = cu1l.start(); yy<cu1l.height();yy++)
    {
      for(int64_t xx = cu1l.start();xx<cu1l.width(); xx++)
      {
        if (ch1g(xx,yy)<=0. && ch1d(xx,yy)<=0.)
        {
          cf1(xx,yy)=0.;
          cf2(xx,yy)=0.;
          cf3(xx,yy)=0.;
        }
        else
        {
          double c = max(fabs(cu1l(xx,yy))+sqrt(grav*ch1g(xx,yy)),fabs(cu1r(xx,yy))+sqrt(grav*ch1d(xx,yy)));
          double cd = c*0.5;
          double qd = cu1r(xx,yy)*ch1d(xx,yy);
          double qg = cu1l(xx,yy)*ch1g(xx,yy);
          cf1(xx,yy)=(qg+qd)*0.5-cd*(ch1d(xx,yy)-ch1g(xx,yy));
          cf2(xx,yy)=((cu1l(xx,yy)*qg)+(grav_dem*ch1g(xx,yy)*ch1g(xx,yy))+(cu1r(xx,yy)*qd)+(grav_dem*ch1d(xx,yy)*ch1d(xx,yy)))*0.5-cd*(qd-qg);
          cf3(xx,yy)=(qg*cv1l(xx,yy)+qd*cv1r(xx,yy))*0.5-cd*(ch1d(xx,yy)*cv1r(xx,yy)-ch1g(xx,yy)*cv1l(xx,yy));
        }
      }
    }

  Controller<double,0,false> ch2l(h2l.getDMatrix());
  Controller<double,0,false> ch2r(h2r.getDMatrix());
  Controller<double,0,false> cdelz2(delz2.getDMatrix());
  Controller<double,0,false> ch2g(h2g.getDMatrix());
  Controller<double,0,false> ch2d(h2d.getDMatrix());

    for(int64_t yy = ch2l.start(); yy<ch2l.height();yy++)
    {
      for(int64_t xx = ch2l.start();xx<ch2l.width(); xx++)
      {
        ch2g(xx,yy)=max(0.,ch2l(xx,yy)-max(0.,cdelz2(xx,yy)));
        ch2d(xx,yy)=max(0.,ch2r(xx,yy)-max(0.,-cdelz2(xx,yy)));
      }
    }

  Controller<double,0,false> cu2l(u2l.getDMatrix());
  Controller<double,0,false> cu2r(u2r.getDMatrix());
  Controller<double,0,false> cv2l(v2l.getDMatrix());
  Controller<double,0,false> cv2r(v2r.getDMatrix());
  Controller<double,0,false> cg1(g1.getDMatrix());
  Controller<double,0,false> cg2(g2.getDMatrix());
  Controller<double,0,false> cg3(g3.getDMatrix()); 

  for(int64_t yy = cu2l.start(); yy<cu2l.height();yy++)
    {
      for(int64_t xx = cu2l.start();xx<cu2l.width(); xx++)
        if (ch2g(xx,yy)<=0. && ch2d(xx,yy)<=0.)
        {
          cg1(xx,yy)=0.;
          cg2(xx,yy)=0.;
          cg3(xx,yy)=0.;
        }
        else
        {
          double c = max(fabs(cu2l(xx,yy))+sqrt(grav*ch2g(xx,yy)),fabs(cu2r(xx,yy))+sqrt(grav*ch2d(xx,yy)));
          double cd = c*0.5;
          double qd = cu2r(xx,yy)*ch2d(xx,yy);
          double qg = cu2l(xx,yy)*ch2g(xx,yy);
          cg1(xx,yy)=(qg+qd)*0.5-cd*(ch2d(xx,yy)-ch2g(xx,yy));
          cg2(xx,yy)=((cu2l(xx,yy)*qg)+(grav_dem*ch2g(xx,yy)*ch2g(xx,yy))+(cu2r(xx,yy)*qd)+(grav_dem*ch2d(xx,yy)*ch2d(xx,yy)))*0.5-cd*(qd-qg);
          cg3(xx,yy)=(qg*cv2l(xx,yy)+qd*cv2r(xx,yy))*0.5-cd*(ch2d(xx,yy)*cv2r(xx,yy)-ch2g(xx,yy)*cv2l(xx,yy));
        }
    }
}

void scheme:: bloc2(TAB & he, TAB & ve1, TAB & ve2, TAB0 & qe1, TAB0 & qe2, TAB & hes, TAB0 & qes1, TAB0 & qes2, TAB & us, TAB & vs) {

  //communications
  /*he.getDMatrix()->getBorders();
  ve1.getDMatrix()->getBorders();
  ve2.getDMatrix()->getBorders();
  hes.getDMatrix()->getBorders();*/

  Controller<double,2,false> ch(he.getDMatrix());
  Controller<double,2,false> cu(ve1.getDMatrix());
  Controller<double,2,false> cv(ve2.getDMatrix());
  Controller<double,0,false> cq1(qe1.getDMatrix());
  Controller<double,0,false> cq2(qe2.getDMatrix());
  Controller<double,0,false> ch1g(h1g.getDMatrix());
  Controller<double,0,false> ch1d(h1d.getDMatrix());
  Controller<double,0,false> ch1l(h1l.getDMatrix());
  Controller<double,0,false> ch1r(h1r.getDMatrix());
  Controller<double,0,false> ch2g(h2g.getDMatrix());
  Controller<double,0,false> ch2d(h2d.getDMatrix());
  Controller<double,0,false> ch2l(h2l.getDMatrix());
  Controller<double,0,false> ch2r(h2r.getDMatrix());
  Controller<double,0,false> cdelzc1(delzc1.getDMatrix());
  Controller<double,0,false> cdelzc2(delzc2.getDMatrix());
  Controller<double,0,false> cf1(f1.getDMatrix());
  Controller<double,0,false> cf2(f2.getDMatrix());
  Controller<double,0,false> cf3(f3.getDMatrix());
  Controller<double,0,false> cg1(g1.getDMatrix());
  Controller<double,0,false> cg2(g2.getDMatrix());
  Controller<double,0,false> cg3(g3.getDMatrix());
  Controller<double,2,false> chs(hes.getDMatrix());
  Controller<double,0,false> cqs1(qes1.getDMatrix());
  Controller<double,0,false> cqs2(qes2.getDMatrix());

  //check
  Controller<double,2,false> cus(us.getDMatrix());
  Controller<double,2,false> cvs(vs.getDMatrix());

    double tx=0.005;
    double ty=0.005;
    double dt=0.5;

    int64_t yyq=cq1.start();
    int64_t yy1=ch1r.start();
    int64_t yy2=ch2r.start();
    for(int64_t yy = ch.start(); yy<ch.height();yy++,yyq++,yy1++,yy2++)
    {
      int64_t xxq=cq1.start();
      int64_t xx1=ch1r.start();
      int64_t xx2=ch2r.start();
      for(int64_t xx = ch.start();xx<ch.width(); xx++,xxq++,xx1++,xx2++)
      {
	//std::cout<<"chs("<<xx<<","<<yy<<")="<<chs(xx,yy)<<std::endl;
	
        chs(xx,yy) = ch(xx,yy)-tx*(cf1(xx1+1,yy1)-cf1(xx1,yy1))-ty*(cg1(xx2,yy2)-cg1(xx2,yy2+1)) ;//+ cRain(xxq,yyq)*dt;
        cqs1(xxq,yyq) = cq1(xxq,yyq)-tx*(cf2(xx1+1,yy1)-cf2(xx1,yy1)+grav_dem*(ch1d(xx1,yy1)*ch1d(xx1,yy1)-ch1r(xx1,yy1)*ch1r(xx1,yy1)
          +ch1l(xx1+1,yy1)*ch1l(xx1+1,yy1)-ch1g(xx1+1,yy1)*ch1g(xx1+1,yy1)+(ch1r(xx1+1,yy1)+ch1l(xx1+1,yy1))*cdelzc1(xxq,yyq)))
          -ty*(cg2(xx2,yy2)-cg2(xx2,yy2+1));
        cqs2(xxq,yyq)= cq2(xxq,yyq)-tx*(cf3(xx1+1,yy1)-cf3(xx1,yy1))-ty*(cg3(xx2,yy2)-cg3(xx2,yy2+1)+grav_dem*(ch2d(xx2,yy2+1)
          *ch2d(xx2,yy2+1)-ch2r(xx2,yy2+1)*ch2r(xx2,yy2+1)+ch2l(xx2,yy2)*ch2l(xx2,yy2)-ch2g(xx2,yy2)*ch2g(xx2,yy2)+(ch2r(xx2,yy2+1)
          +ch2l(xx2,yy2))*cdelzc2(xxq,yyq)));

        //check checkus checkvs
        if(chs(xx,yy)<he_ca)
        {
          chs(xx,yy)=0.;
          cus(xx,yy)=0.;
          cvs(xx,yy)=0.;
          cqs1(xxq,yyq)=0.;
          cqs2(xxq,yyq)=0.;
        }
        else
        {
          cus(xx,yy)=cqs1(xxq,yyq)/chs(xx,yy);
          if(fabs(cus(xx,yy))<ve_ca)
          {
            cus(xx,yy)=0.;
            cqs1(xxq,yyq)=0.;
          }
          cvs(xx,yy)=cqs2(xxq,yyq)/chs(xx,yy);
          if(fabs(cvs(xx,yy))<ve_ca)
          {
            cvs(xx,yy)=0.;
            cqs2(xxq,yyq)=0.;
          }
        }
      }
    }
}

void scheme:: bloc22(TAB & he, TAB & ve1, TAB & ve2, TAB0 & qe1, TAB0 & qe2, TAB & hes, TAB0 & qes1, TAB0 & qes2, TAB & h, TAB0 & q1, TAB0& q2) {

  //communications
  /*he.getDMatrix()->getBorders();
  ve1.getDMatrix()->getBorders();
  ve2.getDMatrix()->getBorders();
  hes.getDMatrix()->getBorders();*/

  Controller<double,2,false> ch(he.getDMatrix());
  Controller<double,2,false> cu(ve1.getDMatrix());
  Controller<double,2,false> cv(ve2.getDMatrix());
  Controller<double,0,false> cq1(qe1.getDMatrix());
  Controller<double,0,false> cq2(qe2.getDMatrix());
  Controller<double,0,false> ch1g(h1g.getDMatrix());
  Controller<double,0,false> ch1d(h1d.getDMatrix());
  Controller<double,0,false> ch1l(h1l.getDMatrix());
  Controller<double,0,false> ch1r(h1r.getDMatrix());
  Controller<double,0,false> ch2g(h2g.getDMatrix());
  Controller<double,0,false> ch2d(h2d.getDMatrix());
  Controller<double,0,false> ch2l(h2l.getDMatrix());
  Controller<double,0,false> ch2r(h2r.getDMatrix());
  Controller<double,0,false> cdelzc1(delzc1.getDMatrix());
  Controller<double,0,false> cdelzc2(delzc2.getDMatrix());
  Controller<double,0,false> cf1(f1.getDMatrix());
  Controller<double,0,false> cf2(f2.getDMatrix());
  Controller<double,0,false> cf3(f3.getDMatrix());
  Controller<double,0,false> cg1(g1.getDMatrix());
  Controller<double,0,false> cg2(g2.getDMatrix());
  Controller<double,0,false> cg3(g3.getDMatrix());
  Controller<double,2,false> chs(hes.getDMatrix());
  Controller<double,0,false> cqs1(qes1.getDMatrix());
  Controller<double,0,false> cqs2(qes2.getDMatrix());

  //heun
  Controller<double,2,false> chh(h.getDMatrix());
  Controller<double,0,false> cqq1(q1.getDMatrix());
  Controller<double,0,false> cqq2(q2.getDMatrix());

    double tx=0.005;
    double ty=0.005;
    double dt=0.5;

    int64_t yyq=cq1.start();
    int64_t yy1=ch1r.start();
    int64_t yy2=ch2r.start();
    for(int64_t yy = ch.start(); yy<ch.height();yy++,yyq++,yy1++,yy2++)
    {
      int64_t xxq=cq1.start();
      int64_t xx1=ch1r.start();
      int64_t xx2=ch2r.start();
      for(int64_t xx = ch.start();xx<ch.width(); xx++,xxq++,xx1++,xx2++)
      {
        chs(xx,yy) = ch(xx,yy)-tx*(cf1(xx1+1,yy1)-cf1(xx1,yy1))-ty*(cg1(xx2,yy2)-cg1(xx2,yy2+1)) ;//+ cRain(xxq,yyq)*dt;
        cqs1(xxq,yyq) = cq1(xxq,yyq)-tx*(cf2(xx1+1,yy1)-cf2(xx1,yy1)+grav_dem*(ch1d(xx1,yy1)*ch1d(xx1,yy1)-ch1r(xx1,yy1)*ch1r(xx1,yy1)
          +ch1l(xx1+1,yy1)*ch1l(xx1+1,yy1)-ch1g(xx1+1,yy1)*ch1g(xx1+1,yy1)+(ch1r(xx1+1,yy1)+ch1l(xx1+1,yy1))*cdelzc1(xxq,yyq)))
          -ty*(cg2(xx2,yy2)-cg2(xx2,yy2+1));
        cqs2(xxq,yyq)= cq2(xxq,yyq)-tx*(cf3(xx1+1,yy1)-cf3(xx1,yy1))-ty*(cg3(xx2,yy2)-cg3(xx2,yy2+1)+grav_dem*(ch2d(xx2,yy2+1)
          *ch2d(xx2,yy2+1)-ch2r(xx2,yy2+1)*ch2r(xx2,yy2+1)+ch2l(xx2,yy2)*ch2l(xx2,yy2)-ch2g(xx2,yy2)*ch2g(xx2,yy2)+(ch2r(xx2,yy2+1)
          +ch2l(xx2,yy2))*cdelzc2(xxq,yyq)));

        //heun
        chh(xx,yy)=0.5*(chh(xx,yy)+chs(xx,yy));
        cqq1(xxq,yyq)=0.5*(cqq1(xxq,yyq)+cqs1(xxq,yyq));
        cqq2(xxq,yyq)=0.5*(cqq2(xxq,yyq)+cqs2(xxq,yyq));
      }
    }
}

void scheme:: update_hq(TAB & he, TAB0 & qe1, TAB0 & qe2, TAB & hes, TAB0 & qes1, TAB0 & qes2) {

  /*vTAB0 inputs;
  vTAB outputs;
  outputs.push_back(he);
  outputs.push_back(hes);
  //outputs.push_back(he);
  inputs.push_back(qe1);
  inputs.push_back(qe2);
  //outputs.push_back(hes);
  inputs.push_back(qes1);
  inputs.push_back(qes2);

  ApplyList<SCALAR,0,SCALAR,2,false>::apply(update_hq_f,inputs,outputs);*/

  //no communications

}

void scheme:: boundary(TAB & he,TAB & ve1 ,TAB & ve2) {

  //no communications

  ControllerPHBD<double,2,false> ch(he.getDMatrix());
  ControllerPHBD<double,2,false> cu(ve1.getDMatrix());
  ControllerPHBD<double,2,false> cv(ve2.getDMatrix());
    
    for(int64_t yy = ch.startHeight(); yy<ch.height();yy++)                                                                                                               
      {
       for(int64_t xx = ch.startWidth();xx<ch.width(); xx++)
       {
        ch(xx,yy) = ch(xx,yy-1);
        cu(xx,yy) = -cu(xx,yy-1);
        cv(xx,yy) = cv(xx,yy-1);
       }                                                                                                                                                                                           
      }

  ControllerPHBL<double,2,false> chl(he.getDMatrix());
  ControllerPHBL<double,2,false> cul(ve1.getDMatrix());
  ControllerPHBL<double,2,false> cvl(ve2.getDMatrix());

    for(int64_t yy = chl.startHeight(); yy<chl.height();yy++)                                                                                                               
      {
       for(int64_t xx = chl.startWidth();xx<chl.width(); xx++)
       {
        chl(xx,yy) = chl(xx+1,yy);
        cul(xx,yy) = -cul(xx+1,yy);
        cvl(xx,yy) = cvl(xx+1,yy);
       }                                                                                                                                                                                           
      }

  ControllerPHBR<double,2,false> chr(he.getDMatrix());
  ControllerPHBR<double,2,false> cur(ve1.getDMatrix());
  ControllerPHBR<double,2,false> cvr(ve2.getDMatrix());

    for(int64_t yy = chr.startHeight(); yy<chr.height();yy++)                                                                                                               
      {
       for(int64_t xx = chr.startWidth();xx<chr.width(); xx++)
       {
        chr(xx,yy) = chr(xx-1,yy);
        cur(xx,yy) = -cur(xx-1,yy);
        cvr(xx,yy) = cvr(xx-1,yy);
       }                                                                                                                                                                                           
      }

  ControllerPHBU<double,2,false> chu(he.getDMatrix());
  ControllerPHBU<double,2,false> cuu(ve1.getDMatrix());
  ControllerPHBU<double,2,false> cvu(ve2.getDMatrix());

    for(int64_t yy = chu.startHeight(); yy<chu.height();yy++)                                                                                                               
      {
       for(int64_t xx = chu.startWidth();xx<chu.width(); xx++)
       {
        chu(xx,yy) = chu(xx,yy+1);
        cuu(xx,yy) = -cuu(xx,yy+1);
        cvu(xx,yy) = cvu(xx,yy+1);
       }                                                                                                                                                                                           
       }
}

void scheme:: check_ve_ca(TAB & he, TAB & ve1, TAB & ve2, TAB0 & qe1, TAB0 & qe2) {

	//integrated in bloc2

  /*vTAB0 inputs;
  inputs.push_back(qe1);
  inputs.push_back(qe2);
  vTAB outputs;
  outputs.push_back(he);
  outputs.push_back(ve1);
  outputs.push_back(ve2);

  ApplyList<SCALAR,0,SCALAR,2,false>::apply(ve_ca_f,inputs,outputs);*/
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
