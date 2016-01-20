#ifndef SCHEME_PARALLEL_H
#define SCHEME_PARALLEL_H

#define grav 9.81
#define grav_dem 4.905

#define he_ca 1.e-6
#define ve_ca 1.e-6

#include "util/macros.hpp"

using namespace skelgis;

//BLOC1x
//-------------------------------------------------------------------------------
BEGINApplyListBlock(bloc1x_f,inputs,outputs)
//-------------------------------------------------------------------------------
{
  Data * h1l=inputs[0];
  Data * h1r=inputs[1];
  Data * u1l=inputs[2];
  Data * u1r=inputs[3];
  Data * v1l=inputs[4];
  Data * v1r=inputs[5];
  Data * delz1=inputs[6]; 

  Data * h1g=outputs[0];
  Data * h1d=outputs[1];
  Data * f1=outputs[2];
  Data * f2=outputs[3];
  Data * f3=outputs[4];

  Controller<double> ch1l(h1l);
    Controller<double> ch1r(h1r);
    Controller<double> cu1l(u1l);
    Controller<double> cu1r(u1r);
    Controller<double> cv1l(v1l);
    Controller<double> cv1r(v1r);
    Controller<double> cdelz1(delz1);
    Controller<double> ch1g(h1g);
    Controller<double> ch1d(h1d);
    Controller<double> cf1(f1);
    Controller<double> cf2(f2);
    Controller<double> cf3(f3);

    for(int64_t yy = ch1l.start(); yy<ch1l.height();yy++)
    {
      for(int64_t xx = ch1l.start();xx<ch1l.width(); xx++)
      {
        ch1g(xx,yy)=max(0.,ch1l(xx,yy)-max(0.,cdelz1(xx,yy)));
        ch1d(xx,yy)=max(0.,ch1r(xx,yy)-max(0.,-cdelz1(xx,yy)));
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

  //iData0 it = h1l.begin();
  //iData0 itEnd = h1l.end();

  // parameters * par = parameters::getSingleton();
  // hydrostatic * rec_hydro = hydrostatic::getSingleton();
  // choice_flux * flux_num = choice_flux::getSingleton(par->get_flux());

  // SCALAR cfl_fix = par->get_cflfix();
  // SCALAR cfl = 0;
  // SCALAR tx = par->get_dtfix()/par->get_dx();

  // for(; it<=itEnd; ++it) {
  //   rec_hydro->calcul(h1l[it],h1r[it],delz1[it]);
  //   h1g[it]=rec_hydro->hg();
  //   h1d[it]=rec_hydro->hd();
  //   flux_num->calcul(h1g[it],u1l[it],v1l[it],h1d[it],u1r[it],v1r[it]);
  //   f1[it]=flux_num->get_f1();
  //   f2[it]=flux_num->get_f2();
  //   f3[it]=flux_num->get_f3();

  //   cfl=max(cfl,flux_num->get_cfl());
  // }

  // if(cfl*tx>cfl_fix)
  //   std::cout<<"cfl conditioin is not satisfied in x"<<std::endl;
}
END(bloc1x_f);
//-------------------------------------------------------------------------------

//BLOC1y
//-------------------------------------------------------------------------------
BEGINApplyListBlock(bloc1y_f,inputs,outputs)
//-------------------------------------------------------------------------------
{
  Data * h2l=inputs[0];
  Data * h2r=inputs[1];
  Data * u2l=inputs[2];
  Data * u2r=inputs[3];
  Data * v2l=inputs[4];
  Data * v2r=inputs[5];
  Data * delz2=inputs[6];

  Data * h2g=outputs[0];
  Data * h2d=outputs[1];
  Data * g1=outputs[2];
  Data * g2=outputs[3];
  Data * g3=outputs[4];

  Controller<double> ch2l(h2l);
    Controller<double> ch2r(h2r);
    Controller<double> cu2l(u2l);
    Controller<double> cu2r(u2r);
    Controller<double> cv2l(v2l);
    Controller<double> cv2r(v2r);
    Controller<double> cdelz2(delz2);
    Controller<double> ch2g(h2g);
    Controller<double> ch2d(h2d);
    Controller<double> cg1(g1);
    Controller<double> cg2(g2);
    Controller<double> cg3(g3); 

    for(int64_t yy = ch2l.start(); yy<ch2l.height();yy++)
    {
      for(int64_t xx = ch2l.start();xx<ch2l.width(); xx++)
      {
        ch2g(xx,yy)=max(0.,ch2l(xx,yy)-max(0.,cdelz2(xx,yy)));
        ch2d(xx,yy)=max(0.,ch2r(xx,yy)-max(0.,-cdelz2(xx,yy)));
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
  
  // iData0 it = h2l.begin();
  // iData0 itEnd = h2l.end();

  // parameters * par = parameters::getSingleton();
  // hydrostatic * rec_hydro = hydrostatic::getSingleton();
  // choice_flux * flux_num = choice_flux::getSingleton(par->get_flux());

  // SCALAR cfl_fix = par->get_cflfix();
  // SCALAR cfl = 0;
  // SCALAR ty = par->get_dtfix()/par->get_dy();

  // for(; it<=itEnd; ++it) {
  //   rec_hydro->calcul(h2l[it],h2r[it],delz2[it]);
  //   h2g[it]=rec_hydro->hg();
  //   h2d[it]=rec_hydro->hd();
  //   flux_num->calcul(h2g[it],v2l[it],u2l[it],h2d[it],v2r[it],u2r[it]);
  //   g1[it]=flux_num->get_f1();
  //   g3[it]=flux_num->get_f2();
  //   g2[it]=flux_num->get_f3();

  //   cfl=max(cfl,flux_num->get_cfl());
  // }

  // if(cfl*ty>cfl_fix)
  //   std::cout<<"cfl conditioin is not satisfied in y"<<std::endl;
}
END(bloc1y_f);
//-------------------------------------------------------------------------------

//BLOC2
//-------------------------------------------------------------------------------
BEGINApplyListBlock(bloc2_f,inputs,outputs)
//-------------------------------------------------------------------------------
{
  Data * he=inputs[0];
  Data * ve1=inputs[1];
  Data * ve2=inputs[2];
  Data * hes=inputs[3];

  Data * qe1=outputs[0];
  Data * qe2=outputs[1];
  Data * h1l=outputs[2];
  Data * h1r=outputs[3];
  Data * h1g=outputs[4];
  Data * h1d=outputs[5];
  Data * h2l=outputs[6];
  Data * h2r=outputs[7];
  Data * h2g=outputs[8];
  Data * h2d=outputs[9];
  Data * delzc1=outputs[10];
  Data * delzc2=outputs[11];
  Data * f1=outputs[12];
  Data * f2=outputs[13];
  Data * f3=outputs[14];
  Data * g1=outputs[15];
  Data * g2=outputs[16];
  Data * g3=outputs[17];
  Data * qes1=outputs[18];
  Data * qes2=outputs[19];

  Controller<double> ch(he);
    Controller<double> cu(ve1);
    Controller<double> cv(ve2);
    Controller<double> cq1(qe1);
    Controller<double> cq2(qe2);
    Controller<double> ch1g(h1g);
    Controller<double> ch1d(h1d);
    Controller<double> ch1l(h1l);
    Controller<double> ch1r(h1r);
    Controller<double> ch2g(h2g);
    Controller<double> ch2d(h2d);
    Controller<double> ch2l(h2l);
    Controller<double> ch2r(h2r);
    Controller<double> cdelzc1(delzc1);
    Controller<double> cdelzc2(delzc2);
    Controller<double> cf1(f1);
    Controller<double> cf2(f2);
    Controller<double> cf3(f3);
    Controller<double> cg1(g1);
    Controller<double> cg2(g2);
    Controller<double> cg3(g3);
    //Controller<double> crainm(rainm);
    //Controller<double> cmcf(mcf);
    Controller<double> chs(hes);
    Controller<double> cqs1(qes1);
    Controller<double> cqs2(qes2);
    //Controller<double> cRain(Rain);

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
        chs(xx,yy) = ch(xx,yy)-tx*(cf1(xx1+1,yy1)-cf1(xx1,yy1))-ty*(cg1(xx2,yy2)-cg1(xx2,yy2+1));//+ cRain(xxq,yyq)*dt;
        cqs1(xxq,yyq) = cq1(xxq,yyq)-tx*(cf2(xx1+1,yy1)-cf2(xx1,yy1)+grav_dem*(ch1d(xx1,yy1)*ch1d(xx1,yy1)-ch1r(xx1,yy1)*ch1r(xx1,yy1)
          +ch1l(xx1+1,yy1)*ch1l(xx1+1,yy1)-ch1g(xx1+1,yy1)*ch1g(xx1+1,yy1)+(ch1r(xx1+1,yy1)+ch1l(xx1+1,yy1))*cdelzc1(xxq,yyq)))
          -ty*(cg2(xx2,yy2)-cg2(xx2,yy2+1));
        cqs2(xxq,yyq)= cq2(xxq,yyq)-tx*(cf3(xx1+1,yy1)-cf3(xx1,yy1))-ty*(cg3(xx2,yy2)-cg3(xx2,yy2+1)+grav_dem*(ch2d(xx2,yy2+1)
          *ch2d(xx2,yy2+1)-ch2r(xx2,yy2+1)*ch2r(xx2,yy2+1)+ch2l(xx2,yy2)*ch2l(xx2,yy2)-ch2g(xx2,yy2)*ch2g(xx2,yy2)+(ch2r(xx2,yy2+1)
          +ch2l(xx2,yy2))*cdelzc2(xxq,yyq)));
        cqs1(xxq,yyq)= cqs1(xxq,yyq)/(1.+/*cmcf(xxq,yyq)**/sqrt(cu(xx,yy)*cu(xx,yy)+cv(xx,yy)*cv(xx,yy))*dt/(8.*chs(xx,yy)));
        cqs2(xxq,yyq)= cqs2(xxq,yyq)/(1.+/*cmcf(xxq,yyq)**/sqrt(cu(xx,yy)*cu(xx,yy)+cv(xx,yy)*cv(xx,yy))*dt/(8.*chs(xx,yy)));
      }
    }

 //  parameters * par = parameters::getSingleton();
 //  //    choice_friction * fric = choice_friction::getSingleton(par);
 //  SCALAR ty = par->get_dtfix()/par->get_dy();
 //  SCALAR tx = par->get_dtfix()/par->get_dx();
 //  SCALAR dt = par->get_dtfix();

 //  iData0L itc_line = delzc1.begin_line();
 //  iData0L itl_line = f1.begin_line();
 //  iDataL it_line = he.begin_line();
 //  iDataL it_line_End = he.end_line();
 //  iData0L itu_line = g1.begin_line();
 //  iData0L itd_line = g1.begin_line();
 //  ++itd_line;

 //  for(;it_line<=it_line_End;++it_line,++itc_line,++itl_line,++itu_line,++itd_line)
 //    {
 //      iData0 itc = itc_line.begin();
 //      iData it = it_line.begin();
 //      iData itEnd = it_line.end();
 //      iData0 itl = itl_line.begin();
 //      iData0 itr = itl_line.begin();
 //      ++itr;
 //      iData0 itu = itu_line.begin();
 //      iData0 itd = itd_line.begin();

 //      for(;it<=itEnd;++it,++itc,++itl,++itr,++itu,++itd)
	// {
	//   hes[it] = he[it]-tx*(f1[itr]-f1[itl])-ty*(g1[itu]-g1[itd]);
	//   qes1[itc] = qe1[itc]-tx*(f2[itr]-f2[itl]+grav_dem*(h1d[itl]*h1d[itl]-h1r[itl]*h1r[itl]+h1l[itr]*h1l[itr]-h1g[itr]*h1g[itr]+(h1r[itl]+h1l[itr])*delzc1[itc]))-ty*(g2[itu]-g2[itd]);
	//   qes2[itc] = qe2[itc]-tx*(f3[itr]-f3[itl])-ty*(g3[itu]-g3[itd]+grav_dem*(h2d[itd]*h2d[itd]-h2r[itd]*h2r[itd]+h2l[itu]*h2l[itu]-h2g[itu]*h2g[itu]+(h2r[itd]+h2l[itu])*delzc2[itc]));
	// }
 //    }
}
END(bloc2_f);
//-------------------------------------------------------------------------------

//BUNDARY
//-------------------------------------------------------------------------------
BEGINApplyListBlock(boundary_f,inputs,outputs)
//-------------------------------------------------------------------------------
{
  Data * he=outputs[0];
  Data * ve1=outputs[1];
  Data * ve2=outputs[2];

  ControllerPHBD<double> ch(he);
  ControllerPHBD<double> cu(ve1);
  ControllerPHBD<double> cv(ve2);
    
    for(int64_t yy = ch.startHeight(); yy<ch.height();yy++)                                                                                                               
      {
       for(int64_t xx = ch.startWidth();xx<ch.width(); xx++)
       {
        ch(xx,yy) = ch(xx,yy-1);
        cu(xx,yy) = -cu(xx,yy-1);
        cv(xx,yy) = cv(xx,yy-1);
       }                                                                                                                                                                                           
      }

    for(int64_t yy = ch.startHeight(); yy<ch.height();yy++)                                                                                                               
      {
       for(int64_t xx = ch.startWidth();xx<ch.width(); xx++)
       {
        ch(xx,yy) = ch(xx+1,yy);
        cu(xx,yy) = -cu(xx+1,yy);
        cv(xx,yy) = cv(xx+1,yy);
       }                                                                                                                                                                                           
      }

    for(int64_t yy = ch.startHeight(); yy<ch.height();yy++)                                                                                                               
      {
       for(int64_t xx = ch.startWidth();xx<ch.width(); xx++)
       {
        ch(xx,yy) = ch(xx-1,yy);
        cu(xx,yy) = -cu(xx-1,yy);
        cv(xx,yy) = cv(xx-1,yy);
       }                                                                                                                                                                                           
      }

    for(int64_t yy = ch.startHeight(); yy<ch.height();yy++)                                                                                                               
      {
       for(int64_t xx = ch.startWidth();xx<ch.width(); xx++)
       {
        ch(xx,yy) = ch(xx,yy+1);
        cu(xx,yy) = -cu(xx,yy+1);
        cv(xx,yy) = cv(xx,yy+1);
       }                                                                                                                                                                                           
      }
  
  // parameters * par = parameters::getSingleton();
  // choice_condition_left * Lbound = choice_condition_left::getSingleton(par->get_Lbound());
  // choice_condition_right * Rbound = choice_condition_right::getSingleton(par->get_Rbound());
  // choice_condition_bottom * Bbound = choice_condition_bottom::getSingleton(par->get_Bbound());
  // choice_condition_top * Tbound = choice_condition_top::getSingleton(par->get_Tbound());

  // SCALAR left_imp_discharge = par->get_left_imp_discharge();
  // SCALAR left_imp_h = par->get_left_imp_h();
  // SCALAR right_imp_discharge = par->get_right_imp_discharge();
  // SCALAR right_imp_h = par->get_right_imp_h();
  // SCALAR bottom_imp_discharge = par->get_bottom_imp_discharge();
  // SCALAR bottom_imp_h = par->get_bottom_imp_h();
  // SCALAR top_imp_discharge = par->get_top_imp_discharge();
  // SCALAR top_imp_h = par->get_top_imp_h();

  // iterator_phb_left<SCALAR,2> lit = he.begin_phb_left();
  // iterator_phb_left<SCALAR,2> lit_End = he.end_phb_left();

  // iterator_phb_right<SCALAR,2> rit = he.begin_phb_right();
  // iterator_phb_right<SCALAR,2> rit_End = he.end_phb_right();

  // iterator_phb_up<SCALAR,2> uit = he.begin_phb_up();
  // iterator_phb_up<SCALAR,2> uit_End = he.end_phb_up();

  // iterator_phb_down<SCALAR,2> dit = he.begin_phb_down();
  // iterator_phb_down<SCALAR,2> dit_End = he.end_phb_down();

  // for(;lit<=lit_End;++lit)
  //   {
  //     Lbound->calcul(he.getInRight(lit),ve1.getInRight(lit),ve2.getInRight(lit),left_imp_h,left_imp_discharge);
  //     he[lit]=Lbound->get_hbound();
  //     ve1[lit]=Lbound->get_unormbound();
  //     ve2[lit]=Lbound->get_utanbound();
  //   }

  // for(;rit<=rit_End;++rit)
  //   {
  //     Rbound->calcul(he.getInLeft(rit),ve1.getInLeft(rit),ve2.getInLeft(rit),right_imp_h,right_imp_discharge);
  //     he[rit]=Rbound->get_hbound();
  //     ve1[rit]=Rbound->get_unormbound();
  //     ve2[rit]=Rbound->get_utanbound();
  //   }

  // for(;uit<=uit_End;++uit)
  //   {
  //     Tbound->calcul(he.getInDown(uit),ve2.getInDown(uit),ve1.getInDown(uit),top_imp_h,top_imp_discharge);
  //     he[uit]=Tbound->get_hbound();
  //     ve2[uit]=Tbound->get_unormbound();
  //     ve1[uit]=Tbound->get_utanbound();
  //   }

  // for(;dit<=dit_End;++dit)
  //   {
  //     Bbound->calcul(he.getInUp(dit),ve2.getInUp(dit),ve1.getInUp(dit),bottom_imp_h,bottom_imp_discharge);
  //     he[dit]=Bbound->get_hbound();
  //     ve2[dit]=Bbound->get_unormbound();
  //     ve1[dit]=Bbound->get_utanbound();
  //   }
}
END(boundary_f);
//-------------------------------------------------------------------------------

//VERIFICATION 0
//-------------------------------------------------------------------------------
BEGINApplyListBlock(ve_ca_f,inputs,outputs)
//-------------------------------------------------------------------------------
{
  Data * h=outputs[0];
  Data * u=outputs[1];
  Data * v=outputs[2];
  Data * q1=inputs[0];
  Data * q2=inputs[1];

  Controller<double> chs(h);
    Controller<double> cus(u);
    Controller<double> cvs(v);
    Controller<double> cqs1(q1);
    Controller<double> cqs2(q2);

    int64_t yyq=cqs1.start();
    for(int64_t yy = chs.start(); yy<chs.height();yy++,yyq++)
    {
      int64_t xxq=cqs1.start();
      for(int64_t xx = chs.start();xx<chs.width(); xx++,xxq++)
      {
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
  
 //  iData it_2 = h.begin();
 //  iData0 it_0 = q1.begin();
 //  iData it_hEnd = h.end();

 //  for(; it_2<=it_hEnd; it_2++,++it_0) {
 //    if (h[it_2]<he_ca) {
 //      h[it_2]=0.;
 //      u[it_2]=0.;
 //      v[it_2]=0.;
 //      q1[it_0]=0.;
 //      q2[it_0]=0.;
 //    } else {
 //      u[it_2]=q1[it_0]/h[it_2];
 //      if (fabs(u[it_2])<ve_ca) {
	// u[it_2]=0.;
	// q1[it_0]=0.;
 //      }
 //      v[it_2]=q2[it_0]/h[it_2];
 //      if (fabs(v[it_2])<ve_ca) {
	// v[it_2]=0.;
	// q2[it_0]=0.;
 //      }
 //    }
 //  }
}
END(ve_ca_f);
//-------------------------------------------------------------------------------

//UPDATE HQ
//-------------------------------------------------------------------------------
BEGINApplyListBlock(update_hq_f,inputs,outputs)
//-------------------------------------------------------------------------------
{
  // Data h(outputs[0]);
  // Data hs(outputs[1]);
  // Data0 q1(inputs[0]);
  // Data0 q2(inputs[1]);
  // Data0 qs1(inputs[2]);
  // Data0 qs2(inputs[3]);

  // iData it_2 = h.begin();
  // iData0 it_0 = q1.begin();
  // iData it_hEnd = h.end();

  // for(; it_2<=it_hEnd; it_2++,++it_0) {
  //   h[it_2]=hs[it_2];
  //   q1[it_0]=qs1[it_0];
  //   q2[it_0]=qs2[it_0];
  // }
}
END(update_hq_f);
//-------------------------------------------------------------------------------

//BOUNDARY Z
//-------------------------------------------------------------------------------
BEGINApplyUnaryBlock(boundaryz_f,zi,z)
//-------------------------------------------------------------------------------
{
  // iterator_phb_left<SCALAR,2> lit = z.begin_phb_left();
  // iterator_phb_left<SCALAR,2> lit_End = z.end_phb_left();

  // iterator_phb_right<SCALAR,2> rit = z.begin_phb_right();
  // iterator_phb_right<SCALAR,2> rit_End = z.end_phb_right();

  // iterator_phb_up<SCALAR,2> uit = z.begin_phb_up();
  // iterator_phb_up<SCALAR,2> uit_End = z.end_phb_up();

  // iterator_phb_down<SCALAR,2> dit = z.begin_phb_down();
  // iterator_phb_down<SCALAR,2> dit_End = z.end_phb_down();

  // for(;lit<=lit_End;++lit)
  //   z[lit]=z.getInRight(lit);

  // for(;rit<=rit_End;++rit)
  //   z[rit]=z.getInLeft(rit);

  // for(;uit<=uit_End;++uit)
  //   z[uit]=z.getInDown(uit);

  // for(;dit<=dit_End;++dit)
  //   z[dit]=z.getInUp(dit);
}
END(boundaryz_f);
//-------------------------------------------------------------------------------

#endif
