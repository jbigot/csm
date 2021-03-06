#include "misc.hpp"
#include "order2.hpp"
#include "order2_parallel.hpp"

order2::order2(parameters * &par):scheme(par) {

  // variable avec cellule fictive
  hsa.init(head,1.0);
  usa.init(head,1.0);
  vsa.init(head,1.0);

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

  //=========================================================
  // INIT iteration
  //=========================================================

  //boundary conditions
  boundary(h,u,v);

  //========== order1huv1
  //communications
  h.getDMatrix()->getBorders();
  u.getDMatrix()->getBorders();
  v.getDMatrix()->getBorders();
  z.getDMatrix()->getBorders();


  parameters * par = parameters::getSingleton();
  rec_muscl * rec = rec_muscl::getSingleton(par->get_lim());

  Controller<double,2,false> ch(h.getDMatrix());
  Controller<double,2,false> cu(u.getDMatrix());
  Controller<double,2,false> cv(v.getDMatrix());
  Controller<double,2,false> cz(z.getDMatrix());
  Controller<double,0,false> ch1l(h1l.getDMatrix());
  Controller<double,0,false> ch1r(h1r.getDMatrix());
  Controller<double,0,false> cu1l(u1l.getDMatrix());
  Controller<double,0,false> cu1r(u1r.getDMatrix());
  Controller<double,0,false> cv1l(v1l.getDMatrix());
  Controller<double,0,false> cv1r(v1r.getDMatrix());
  Controller<double,0,false> cz1l(z1l.getDMatrix());
  Controller<double,0,false> cz1r(z1r.getDMatrix());

  double dh1,dh2,du1,du2,dv1,dv2,dz1,dz2;
  double hl,ul,vl,zl,hr,ur,vr,zr;
 
  int64_t yy1 = ch1l.start();
  for(int64_t yy = ch.start(); yy<ch.height();yy++,yy1++)
    {
      int64_t x = ch.start();
      dh1 = ch(x-1,yy)-ch(x-2,yy);
      du1 = cu(x-1,yy)-cu(x-2,yy);
      dv1 = cv(x-1,yy)-cv(x-2,yy);
      dz1 = cz(x-1,yy)-cz(x-2,yy);
      dh2 = ch(x,yy)-ch(x-1,yy);
      du2 = cu(x,yy)-cu(x-1,yy);
      dv2 = cv(x,yy)-cv(x-1,yy);
      dz2 = cz(x,yy)-cz(x-1,yy);
      hl = ch(x-1,yy)+0.5*Minmodln(dh1,dh2);
      hr = ch(x-1,yy)+0.5*Minmodln(dh1,dh2);
      ur = cu(x-1,yy)+0.5*Minmodln(du1,du2)*hl/ch(x-1,yy);
      vr = cv(x-1,yy)+0.5*Minmodln(dv1,dv2)*hl/ch(x-1,yy);
      zr = cz(x-1,yy)+0.5*(Minmodln(dz1+dh1,dz2+dh2)-Minmodln(dh1,dh2));
      int64_t xx1 = ch1l.start();
      for(int64_t xx = ch.start()+1;xx<ch.width()-1; xx++,xx1++)
	{
	  ch1l(xx1,yy1)=hr;
	  cu1l(xx1,yy1)=ur;
	  cv1l(xx1,yy1)=vr;
	  cz1l(xx1,yy1)=zr;
	  dh1 = dh2;
	  du1 = du2;
	  dv1 = dv2;
	  dz1 = dz2;
	  dh2 = ch(xx+1,yy)-ch(xx,yy);
	  du2 = cu(xx+1,yy)-cu(xx,yy);
	  dv2 = cv(xx+1,yy)-cv(xx,yy);
	  dz2 = cz(xx+1,yy)-cz(xx,yy);
	  hr = ch(xx,yy)+0.5*Minmodln(dh1,dh2);
	  hl = ch(xx,yy)-0.5*Minmodln(dh1,dh2);
	  ch1r(xx1,yy1)=hl;
	  cu1r(xx1,yy1)=cu(xx,yy)-0.5*Minmodln(du1,du2)*hr/ch(xx,yy);
	  cv1r(xx1,yy1)=cv(xx,yy)-0.5*Minmodln(dv1,dv2)*hr/ch(xx,yy);
	  cz1r(xx1,yy1)=cz(xx,yy)-0.5*(Minmodln(dz1+dh1,dz2+dh2)-Minmodln(dh1,dh2));
	  ur = cu(xx,yy)+0.5*Minmodln(du1,du2)*hl/ch(xx,yy);
	  vr=cv(xx,yy)+0.5*Minmodln(dv1,dv2)*hl/ch(xx,yy);
	  zr=cz(xx,yy)+0.5*(Minmodln(dz1+dh1,dz2+dh2)-Minmodln(dh1,dh2));
	}
      x = ch.width()-1;
      xx1 = ch1l.width()-1;
      ch1l(xx1,yy1)=hr;
      cu1l(xx1,yy1)=ur;
      cv1l(xx1,yy1)=vr;
      cz1l(xx1,yy1)=zr;
      dh1 = dh2;
      du1 = du2;
      dv1 = dv2;
      dz1 = dz2;
      dh2=ch(x+2,yy)-ch(x+1,yy);
      du2=ch(x+2,yy)-cu(x+1,yy);
      dv2=ch(x+2,yy)-cv(x+1,yy);
      dz2=ch(x+2,yy)-cz(x+1,yy);
      ch1r(xx1,yy1) = ch(x+1,yy)+0.5*Minmodln(dh1,dh2);
      cu1r(xx1,yy1) = cu(x+1,yy)+0.5*Minmodln(du1,du2)*hl/ch(x+1,yy);
      cv1r(xx1,yy1) = cv(x+1,yy)+0.5*Minmodln(dv1,dv2)*hl/ch(x+1,yy);
      cz1r(xx1,yy1) = cz(x+1,yy)+0.5*(Minmodln(dz1+dh1,dz2+dh2)-Minmodln(dh1,dh2));
    }
  //========== order1huv1

  //========== order2huv2

  Controller<double,0,false> ch2l(h2l.getDMatrix());
  Controller<double,0,false> ch2r(h2r.getDMatrix());
  Controller<double,0,false> cu2l(u2l.getDMatrix());
  Controller<double,0,false> cu2r(u2r.getDMatrix());
  Controller<double,0,false> cv2l(v2l.getDMatrix());
  Controller<double,0,false> cv2r(v2r.getDMatrix());
  Controller<double,0,false> cz2l(z2l.getDMatrix());
  Controller<double,0,false> cz2r(z2r.getDMatrix());

  //first line
  int64_t y = ch.start();
  int64_t yy2 = ch2r.start();
  int64_t xx2 = ch2r.start();
  for(int64_t xx = ch.start();xx<ch.width(); xx++,xx2++)
    {
      dh1 = ch(xx,y-1)-ch(xx,y);
      du1 = cu(xx,y-1)-cu(xx,y);
      dv1 = cv(xx,y-1)-cv(xx,y);
      dz1 = cz(xx,y-1)-cz(xx,y);
      dh2 = ch(xx,y-2)-ch(xx,y-1);
      du2 = cu(xx,y-2)-cu(xx,y-1);
      dv2 = cv(xx,y-2)-cv(xx,y-1);
      dz2 = cz(xx,y-2)-cz(xx,y-1);
      hl = ch(xx,y-1)-0.5*Minmodln(dh1,dh2);
      hr = ch(xx,y-1)+0.5*Minmodln(dh1,dh2);
      ch2r(xx2,yy2)=hl;
      cu2r(xx2,yy2)=cu(xx,y-1)-0.5*Minmodln(du1,du2)*hr/ch(xx,y-1);
      cv2r(xx2,yy2)=cv(xx,y-1)-0.5*Minmodln(dv1,dv2)*hl/ch(xx,y-1);
      cz2r(xx2,yy2)=cz(xx,y-1)-0.5*(Minmodln(dz1+dh1,dz2+dh2)-Minmodln(dh1,dh2));
    }

  //other lines
  int64_t yy2l = ch2l.start();
  int64_t yy2r = ch2r.start()+1;
  for(int64_t yy = ch.start()+1; yy<ch.height()-1;yy++,yy2l++,yy2r++)
    {
      int64_t xx2l = ch2l.start();
      int64_t xx2r = ch2r.start();
      for(int64_t xx = ch.start();xx<ch.width(); xx++,xx2l++,xx2r++)
	{
	  dh1 = ch(xx,yy)-ch(xx,yy+1);
	  du1 = cu(xx,yy)-cu(xx,yy+1);
	  dv1 = cv(xx,yy)-cv(xx,yy+1);
	  dz1 = cz(xx,yy)-cz(xx,yy+1);
	  dh2 = ch(xx,yy-1)-ch(xx,yy);
	  du2 = cu(xx,yy-1)-cu(xx,yy);
	  dv2 = cv(xx,yy-1)-cv(xx,yy);
	  dz2 = cz(xx,yy-1)-cz(xx,yy);
	  hl = ch(xx,yy)-0.5*Minmodln(dh1,dh2);
	  hr = ch(xx,yy)+0.5*Minmodln(dh1,dh2);
	  ch2r(xx2r,yy2r)=hl;
	  ch2l(xx2l,yy2l)=hr;
	  cu2r(xx2r,yy2r)=cu(xx,yy)-0.5*Minmodln(du1,du2)*hr/ch(xx,yy);
	  cu2l(xx2l,yy2l)=cu(xx,yy)+0.5*Minmodln(du1,du2)*hl/ch(xx,yy);
	  cv2r(xx2r,yy2r)=cv(xx,yy)-0.5*Minmodln(dv1,dv2)*hr/ch(xx,yy);
	  cv2l(xx2l,yy2l)=cv(xx,yy)+0.5*Minmodln(dv1,dv2)*hl/ch(xx,yy);
	  cz2r(xx2r,yy2r)=cz(xx,yy)-0.5*(Minmodln(dz1+dh1,dz2+dh2)-Minmodln(dh1,dh2));
	  cz2l(xx2l,yy2l)=cz(xx,yy)+0.5*(Minmodln(dz1+dh1,dz2+dh2)-Minmodln(dh1,dh2));
	}
    }

  //last line
  y = ch.height()-1;
  yy2 = ch2l.height()-1;
  xx2 = ch2l.start();
  for(int64_t xx = ch.start();xx<ch.width(); xx++,xx2++)
    {
      dh1 = ch(xx,y+1)-ch(xx,y+2);
      du1 = cu(xx,y+1)-cu(xx,y+2);
      dv1 = cv(xx,y+1)-cv(xx,y+2);
      dz1 = cz(xx,y+1)-cz(xx,y+2);
      dh2 = ch(xx,y)-ch(xx,y+1);
      du2 = cu(xx,y)-cu(xx,y+1);
      dv2 = cv(xx,y)-cv(xx,y+1);
      dz2 = cz(xx,y)-cz(xx,y+1);
      hl = ch(xx,y+1)-0.5*Minmodln(dh1,dh2);
      hr = ch(xx,y+1)+0.5*Minmodln(dh1,dh2);
      ch2l(xx2,yy2)=hr;
      cu2l(xx2,yy2)=cu(xx,y+1)+0.5*Minmodln(du1,du2)*hl/ch(xx,y+1);
      cv2l(xx2,yy2)=cv(xx,y+1)+0.5*Minmodln(dv1,dv2)*hl/ch(xx,y+1);
      cz2l(xx2,yy2)=cz(xx,y+1)+0.5*(Minmodln(dz1+dh1,dz2+dh2)-Minmodln(dh1,dh2));
    }

  //------------delz1,delz2,delzc1,delzc2------------//
  //delz1 no communications
  Controller<double,0,false> cdelz(delz1.getDMatrix());

  for(int64_t yy = cdelz.start(); yy<cdelz.height();yy++)
    for(int64_t xx = cdelz.start();xx<cdelz.width(); xx++)
      cdelz(xx,yy) = cz1r(xx,yy) - cz1l(xx,yy);
  //delzc1 no communications
  Controller<double,0,false> cdelzc1(delzc1.getDMatrix());
  Controller<double,0,false> cdelzc2(delzc2.getDMatrix());

  for(int64_t yy = cdelzc1.start(); yy<cdelzc1.height();yy++)
    {
      for(int64_t xx = cdelzc1.start();xx<cdelzc1.width(); xx++)
	{
	  cdelzc1(xx,yy) = cz1r(xx+1,yy) - cz1l(xx,yy);
	  cdelzc2(xx,yy) = cz2r(xx,yy) - cz2l(xx,yy+1);
	}
    }
  //delz2 no communications
  Controller<double,0,false> cdelz2(delz2.getDMatrix());

  for(int64_t yy = cdelz2.start(); yy<cdelz2.height();yy++)
    for(int64_t xx = cdelz2.start();xx<cdelz2.width(); xx++)
      cdelz2(xx,yy) = cz2r(xx,yy) - cz2l(xx,yy);
  //-------------------------------------------------//

  bloc1(flux_left,flux_right,flux_bottom,flux_top);
  bloc2(h,u,v,q1,q2,hs,qs1,qs2,us,vs); //with check
  //----------------------------------------------//

  //boundary conditions
  boundary(hs,us,vs);

  //========== order1huv1
  //communications
  hs.getDMatrix()->getBorders();
  us.getDMatrix()->getBorders();
  vs.getDMatrix()->getBorders();
  z.getDMatrix()->getBorders();

  Controller<double,2,false> chs(hs.getDMatrix());
  Controller<double,2,false> cus(us.getDMatrix());
  Controller<double,2,false> cvs(vs.getDMatrix());

 
  yy1 = ch1l.start();
  for(int64_t yy = chs.start(); yy<chs.height();yy++,yy1++)
    {
      int64_t x = ch.start();
      dh1 = chs(x-1,yy)-chs(x-2,yy);
      du1 = cus(x-1,yy)-cus(x-2,yy);
      dv1 = cvs(x-1,yy)-cvs(x-2,yy);
      dz1 = cz(x-1,yy)-cz(x-2,yy);
      dh2 = chs(x,yy)-chs(x-1,yy);
      du2 = cus(x,yy)-cus(x-1,yy);
      dv2 = cvs(x,yy)-cvs(x-1,yy);
      dz2 = cz(x,yy)-cz(x-1,yy);
      hl = chs(x-1,yy)+0.5*Minmodln(dh1,dh2);
      hr = chs(x-1,yy)+0.5*Minmodln(dh1,dh2);
      ur = cus(x-1,yy)+0.5*Minmodln(du1,du2)*hl/chs(x-1,yy);
      vr = cvs(x-1,yy)+0.5*Minmodln(dv1,dv2)*hl/chs(x-1,yy);
      zr = cz(x-1,yy)+0.5*(Minmodln(dz1+dh1,dz2+dh2)-Minmodln(dh1,dh2));
      int64_t xx1 = ch1l.start();
      for(int64_t xx = chs.start()+1;xx<chs.width()-1; xx++,xx1++)
	{
	  ch1l(xx1,yy1)=hr;
	  cu1l(xx1,yy1)=ur;
	  cv1l(xx1,yy1)=vr;
	  cz1l(xx1,yy1)=zr;
	  dh1 = dh2;
	  du1 = du2;
	  dv1 = dv2;
	  dz1 = dz2;
	  dh2 = chs(xx+1,yy)-chs(xx,yy);
	  du2 = cus(xx+1,yy)-cus(xx,yy);
	  dv2 = cvs(xx+1,yy)-cvs(xx,yy);
	  dz2 = cz(xx+1,yy)-cz(xx,yy);
	  hr = chs(xx,yy)+0.5*Minmodln(dh1,dh2);
	  hl = chs(xx,yy)-0.5*Minmodln(dh1,dh2);
	  ch1r(xx1,yy1)=hl;
	  cu1r(xx1,yy1)=cus(xx,yy)-0.5*Minmodln(du1,du2)*hr/chs(xx,yy);
	  cv1r(xx1,yy1)=cvs(xx,yy)-0.5*Minmodln(dv1,dv2)*hr/chs(xx,yy);
	  cz1r(xx1,yy1)=cz(xx,yy)-0.5*(Minmodln(dz1+dh1,dz2+dh2)-Minmodln(dh1,dh2));
	  ur = cus(xx,yy)+0.5*Minmodln(du1,du2)*hl/chs(xx,yy);
	  vr=cvs(xx,yy)+0.5*Minmodln(dv1,dv2)*hl/chs(xx,yy);
	  zr=cz(xx,yy)+0.5*(Minmodln(dz1+dh1,dz2+dh2)-Minmodln(dh1,dh2));
	}
      x = ch.width()-1;
      xx1 = ch1l.width()-1;
      ch1l(xx1,yy1)=hr;
      cu1l(xx1,yy1)=ur;
      cv1l(xx1,yy1)=vr;
      cz1l(xx1,yy1)=zr;
      dh1 = dh2;
      du1 = du2;
      dv1 = dv2;
      dz1 = dz2;
      dh2=chs(x+2,yy)-chs(x+1,yy);
      du2=chs(x+2,yy)-cus(x+1,yy);
      dv2=chs(x+2,yy)-cvs(x+1,yy);
      dz2=chs(x+2,yy)-cz(x+1,yy);
      ch1r(xx1,yy1) = chs(x+1,yy)+0.5*Minmodln(dh1,dh2);
      cu1r(xx1,yy1) = cus(x+1,yy)+0.5*Minmodln(du1,du2)*hl/chs(x+1,yy);
      cv1r(xx1,yy1) = cvs(x+1,yy)+0.5*Minmodln(dv1,dv2)*hl/chs(x+1,yy);
      cz1r(xx1,yy1) = cz(x+1,yy)+0.5*(Minmodln(dz1+dh1,dz2+dh2)-Minmodln(dh1,dh2));
    }
  //========== order1huv1

  //========== order2huv2

  //first line
  y = ch.start();
  yy2 = ch2r.start();
  xx2 = ch2r.start();
  for(int64_t xx = chs.start();xx<chs.width(); xx++,xx2++)
    {
      dh1 = chs(xx,y-1)-chs(xx,y);
      du1 = cus(xx,y-1)-cus(xx,y);
      dv1 = cvs(xx,y-1)-cvs(xx,y);
      dz1 = cz(xx,y-1)-cz(xx,y);
      dh2 = chs(xx,y-2)-chs(xx,y-1);
      du2 = cus(xx,y-2)-cus(xx,y-1);
      dv2 = cvs(xx,y-2)-cvs(xx,y-1);
      dz2 = cz(xx,y-2)-cz(xx,y-1);
      hl = chs(xx,y-1)-0.5*Minmodln(dh1,dh2);
      hr = chs(xx,y-1)+0.5*Minmodln(dh1,dh2);
      ch2r(xx2,yy2)=hl;
      cu2r(xx2,yy2)=cus(xx,y-1)-0.5*Minmodln(du1,du2)*hr/chs(xx,y-1);
      cv2r(xx2,yy2)=cvs(xx,y-1)-0.5*Minmodln(dv1,dv2)*hl/chs(xx,y-1);
      cz2r(xx2,yy2)=cz(xx,y-1)-0.5*(Minmodln(dz1+dh1,dz2+dh2)-Minmodln(dh1,dh2));
    }

  //other lines
  yy2l = ch2l.start();
  yy2r = ch2r.start()+1;
  for(int64_t yy = chs.start()+1; yy<chs.height()-1;yy++,yy2l++,yy2r++)
    {
      int64_t xx2l = ch2l.start();
      int64_t xx2r = ch2r.start();
      for(int64_t xx = chs.start();xx<chs.width(); xx++,xx2l++,xx2r++)
	{
	  dh1 = chs(xx,yy)-chs(xx,yy+1);
	  du1 = cus(xx,yy)-cus(xx,yy+1);
	  dv1 = cvs(xx,yy)-cvs(xx,yy+1);
	  dz1 = cz(xx,yy)-cz(xx,yy+1);
	  dh2 = chs(xx,yy-1)-chs(xx,yy);
	  du2 = cus(xx,yy-1)-cus(xx,yy);
	  dv2 = cvs(xx,yy-1)-cvs(xx,yy);
	  dz2 = cz(xx,yy-1)-cz(xx,yy);
	  hl = chs(xx,yy)-0.5*Minmodln(dh1,dh2);
	  hr = chs(xx,yy)+0.5*Minmodln(dh1,dh2);
	  ch2r(xx2r,yy2r)=hl;
	  ch2l(xx2l,yy2l)=hr;
	  cu2r(xx2r,yy2r)=cus(xx,yy)-0.5*Minmodln(du1,du2)*hr/chs(xx,yy);
	  cu2l(xx2l,yy2l)=cus(xx,yy)+0.5*Minmodln(du1,du2)*hl/chs(xx,yy);
	  cv2r(xx2r,yy2r)=cvs(xx,yy)-0.5*Minmodln(dv1,dv2)*hr/chs(xx,yy);
	  cv2l(xx2l,yy2l)=cvs(xx,yy)+0.5*Minmodln(dv1,dv2)*hl/chs(xx,yy);
	  cz2r(xx2r,yy2r)=cz(xx,yy)-0.5*(Minmodln(dz1+dh1,dz2+dh2)-Minmodln(dh1,dh2));
	  cz2l(xx2l,yy2l)=cz(xx,yy)+0.5*(Minmodln(dz1+dh1,dz2+dh2)-Minmodln(dh1,dh2));
	}
    }

  //last line
  y = chs.height()-1;
  yy2 = ch2l.height()-1;
  xx2 = ch2l.start();
  for(int64_t xx = chs.start();xx<chs.width(); xx++,xx2++)
    {
      dh1 = chs(xx,y+1)-chs(xx,y+2);
      du1 = cus(xx,y+1)-cus(xx,y+2);
      dv1 = cvs(xx,y+1)-cvs(xx,y+2);
      dz1 = cz(xx,y+1)-cz(xx,y+2);
      dh2 = chs(xx,y)-chs(xx,y+1);
      du2 = cus(xx,y)-cus(xx,y+1);
      dv2 = cvs(xx,y)-cvs(xx,y+1);
      dz2 = cz(xx,y)-cz(xx,y+1);
      hl = chs(xx,y+1)-0.5*Minmodln(dh1,dh2);
      hr = chs(xx,y+1)+0.5*Minmodln(dh1,dh2);
      ch2l(xx2,yy2)=hr;
      cu2l(xx2,yy2)=cus(xx,y+1)+0.5*Minmodln(du1,du2)*hl/chs(xx,y+1);
      cv2l(xx2,yy2)=cvs(xx,y+1)+0.5*Minmodln(dv1,dv2)*hl/chs(xx,y+1);
      cz2l(xx2,yy2)=cz(xx,y+1)+0.5*(Minmodln(dz1+dh1,dz2+dh2)-Minmodln(dh1,dh2));
    }

  //------------delz1,delz2,delzc1,delzc2------------//
  //delz1 no communications
  for(int64_t yy = cdelz.start(); yy<cdelz.height();yy++)
    for(int64_t xx = cdelz.start();xx<cdelz.width(); xx++)
      cdelz(xx,yy) = cz1r(xx,yy) - cz1l(xx,yy);
  //delzc1 no communications

  for(int64_t yy = cdelzc1.start(); yy<cdelzc1.height();yy++)
    {
      for(int64_t xx = cdelzc1.start();xx<cdelzc1.width(); xx++)
	{
	  cdelzc1(xx,yy) = cz1r(xx+1,yy) - cz1l(xx,yy);
	  cdelzc2(xx,yy) = cz2r(xx,yy) - cz2l(xx,yy+1);
	}
    }
  //delz2 no communications

  for(int64_t yy = cdelz2.start(); yy<cdelz2.height();yy++)
    for(int64_t xx = cdelz2.start();xx<cdelz2.width(); xx++)
      cdelz2(xx,yy) = cz2r(xx,yy) - cz2l(xx,yy);
  //-------------------------------------------------//
  bloc1(flux_left,flux_right,flux_bottom,flux_top);
  bloc22(hs,us,vs,qs1,qs2,hsa,qsa1,qsa2,h,q1,q2); //with Heun
  //----------------------------------------------//

  //=========================================================
  //=========================================================


    //time iteration's beginning
    Chronometer chrono;
    chrono.start();

    //while (T > tps) {
    while(tps<T){
      //std::cout<<"========= ITERATION "<<tps<<"==========="<<std::endl;
        // save the data in huv_movie.dat
      //if (n % nsave == 0) {
      //out->write(h,u,v,z,tps);
      //}// end if

        //boundary conditions
        boundary(h,u,v);

        //========== order1huv1
        //communications

	//unsigned long long a,b;
	//a=rdtsc();

	h.getDMatrix()->getBorders();
        u.getDMatrix()->getBorders();
        v.getDMatrix()->getBorders();
        z.getDMatrix()->getBorders();

	//b =rdtsc();
	//std::cout<<"Sync rdtsc = "<<b-a<<std::endl;

        parameters * par = parameters::getSingleton();
  rec_muscl * rec = rec_muscl::getSingleton(par->get_lim());

  //a=rdtsc();

  Controller<double,2,false> ch(h.getDMatrix());
  Controller<double,2,false> cu(u.getDMatrix());
  Controller<double,2,false> cv(v.getDMatrix());
  Controller<double,2,false> cz(z.getDMatrix());
  Controller<double,0,false> ch1l(h1l.getDMatrix());
  Controller<double,0,false> ch1r(h1r.getDMatrix());
  Controller<double,0,false> cu1l(u1l.getDMatrix());
  Controller<double,0,false> cu1r(u1r.getDMatrix());
  Controller<double,0,false> cv1l(v1l.getDMatrix());
  Controller<double,0,false> cv1r(v1r.getDMatrix());
  Controller<double,0,false> cz1l(z1l.getDMatrix());
  Controller<double,0,false> cz1r(z1r.getDMatrix());

  double dh1,dh2,du1,du2,dv1,dv2,dz1,dz2;
  double hl,ul,vl,zl,hr,ur,vr,zr;

  int64_t yy1 = ch1l.start();
  for(int64_t yy = ch.start(); yy<ch.height();yy++,yy1++)
  {
    int64_t x = ch.start();
    dh1 = ch(x-1,yy)-ch(x-2,yy);
    du1 = cu(x-1,yy)-cu(x-2,yy);
    dv1 = cv(x-1,yy)-cv(x-2,yy);
    dz1 = cz(x-1,yy)-cz(x-2,yy);
    dh2 = ch(x,yy)-ch(x-1,yy);
    du2 = cu(x,yy)-cu(x-1,yy);
    dv2 = cv(x,yy)-cv(x-1,yy);
    dz2 = cz(x,yy)-cz(x-1,yy);
    hl = ch(x-1,yy)+0.5*Minmodln(dh1,dh2);
    hr = ch(x-1,yy)+0.5*Minmodln(dh1,dh2);
    ur = cu(x-1,yy)+0.5*Minmodln(du1,du2)*hl/ch(x-1,yy);
    vr = cv(x-1,yy)+0.5*Minmodln(dv1,dv2)*hl/ch(x-1,yy);
    zr = cz(x-1,yy)+0.5*(Minmodln(dz1+dh1,dz2+dh2)-Minmodln(dh1,dh2));
    int64_t xx1 = ch1l.start();
    for(int64_t xx = ch.start()+1;xx<ch.width()-1; xx++,xx1++)
    {
      ch1l(xx1,yy1)=hr;
      cu1l(xx1,yy1)=ur;
      cv1l(xx1,yy1)=vr;
      cz1l(xx1,yy1)=zr;
      dh1 = dh2;
      du1 = du2;
      dv1 = dv2;
      dz1 = dz2;
      dh2 = ch(xx+1,yy)-ch(xx,yy);
      du2 = cu(xx+1,yy)-cu(xx,yy);
      dv2 = cv(xx+1,yy)-cv(xx,yy);
      dz2 = cz(xx+1,yy)-cz(xx,yy);
      hr = ch(xx,yy)+0.5*Minmodln(dh1,dh2);
      hl = ch(xx,yy)-0.5*Minmodln(dh1,dh2);
      ch1r(xx1,yy1)=hl;
      cu1r(xx1,yy1)=cu(xx,yy)-0.5*Minmodln(du1,du2)*hr/ch(xx,yy);
      cv1r(xx1,yy1)=cv(xx,yy)-0.5*Minmodln(dv1,dv2)*hr/ch(xx,yy);
      cz1r(xx1,yy1)=cz(xx,yy)-0.5*(Minmodln(dz1+dh1,dz2+dh2)-Minmodln(dh1,dh2));
      ur = cu(xx,yy)+0.5*Minmodln(du1,du2)*hl/ch(xx,yy);
      vr=cv(xx,yy)+0.5*Minmodln(dv1,dv2)*hl/ch(xx,yy);
      zr=cz(xx,yy)+0.5*(Minmodln(dz1+dh1,dz2+dh2)-Minmodln(dh1,dh2));
    }
    x = ch.width()-1;
    xx1 = ch1l.width()-1;
    ch1l(xx1,yy1)=hr;
    cu1l(xx1,yy1)=ur;
    cv1l(xx1,yy1)=vr;
    cz1l(xx1,yy1)=zr;
    dh1 = dh2;
    du1 = du2;
    dv1 = dv2;
    dz1 = dz2;
    dh2=ch(x+2,yy)-ch(x+1,yy);
    du2=ch(x+2,yy)-cu(x+1,yy);
    dv2=ch(x+2,yy)-cv(x+1,yy);
    dz2=ch(x+2,yy)-cz(x+1,yy);
    ch1r(xx1,yy1) = ch(x+1,yy)+0.5*Minmodln(dh1,dh2);
    cu1r(xx1,yy1) = cu(x+1,yy)+0.5*Minmodln(du1,du2)*hl/ch(x+1,yy);
    cv1r(xx1,yy1) = cv(x+1,yy)+0.5*Minmodln(dv1,dv2)*hl/ch(x+1,yy);
    cz1r(xx1,yy1) = cz(x+1,yy)+0.5*(Minmodln(dz1+dh1,dz2+dh2)-Minmodln(dh1,dh2));
  }

  //b =rdtsc();
  //std::cout<<"huvh1l rdtsc = "<<b-a<<std::endl;
  //========== order1huv1

    //========== order2huv2
  //a=rdtsc();
  Controller<double,0,false> ch2l(h2l.getDMatrix());
  Controller<double,0,false> ch2r(h2r.getDMatrix());
  Controller<double,0,false> cu2l(u2l.getDMatrix());
  Controller<double,0,false> cu2r(u2r.getDMatrix());
  Controller<double,0,false> cv2l(v2l.getDMatrix());
  Controller<double,0,false> cv2r(v2r.getDMatrix());
  Controller<double,0,false> cz2l(z2l.getDMatrix());
  Controller<double,0,false> cz2r(z2r.getDMatrix());

    //first line
    int64_t y = ch.start();
    int64_t yy2 = ch2r.start();
    int64_t xx2 = ch2r.start();
    for(int64_t xx = ch.start();xx<ch.width(); xx++,xx2++)
    {
      dh1 = ch(xx,y-1)-ch(xx,y);
      du1 = cu(xx,y-1)-cu(xx,y);
      dv1 = cv(xx,y-1)-cv(xx,y);
      dz1 = cz(xx,y-1)-cz(xx,y);
      dh2 = ch(xx,y-2)-ch(xx,y-1);
      du2 = cu(xx,y-2)-cu(xx,y-1);
      dv2 = cv(xx,y-2)-cv(xx,y-1);
      dz2 = cz(xx,y-2)-cz(xx,y-1);
      hl = ch(xx,y-1)-0.5*Minmodln(dh1,dh2);
      hr = ch(xx,y-1)+0.5*Minmodln(dh1,dh2);
      ch2r(xx2,yy2)=hl;
      cu2r(xx2,yy2)=cu(xx,y-1)-0.5*Minmodln(du1,du2)*hr/ch(xx,y-1);
      cv2r(xx2,yy2)=cv(xx,y-1)-0.5*Minmodln(dv1,dv2)*hl/ch(xx,y-1);
      cz2r(xx2,yy2)=cz(xx,y-1)-0.5*(Minmodln(dz1+dh1,dz2+dh2)-Minmodln(dh1,dh2));
    }

    //other lines
    int64_t yy2l = ch2l.start();
    int64_t yy2r = ch2r.start()+1;
    for(int64_t yy = ch.start()+1; yy<ch.height()-1;yy++,yy2l++,yy2r++)
    {
      int64_t xx2l = ch2l.start();
      int64_t xx2r = ch2r.start();
      for(int64_t xx = ch.start();xx<ch.width(); xx++,xx2l++,xx2r++)
      {
        dh1 = ch(xx,yy)-ch(xx,yy+1);
        du1 = cu(xx,yy)-cu(xx,yy+1);
        dv1 = cv(xx,yy)-cv(xx,yy+1);
        dz1 = cz(xx,yy)-cz(xx,yy+1);
        dh2 = ch(xx,yy-1)-ch(xx,yy);
        du2 = cu(xx,yy-1)-cu(xx,yy);
        dv2 = cv(xx,yy-1)-cv(xx,yy);
        dz2 = cz(xx,yy-1)-cz(xx,yy);
        hl = ch(xx,yy)-0.5*Minmodln(dh1,dh2);
        hr = ch(xx,yy)+0.5*Minmodln(dh1,dh2);
        ch2r(xx2r,yy2r)=hl;
        ch2l(xx2l,yy2l)=hr;
        cu2r(xx2r,yy2r)=cu(xx,yy)-0.5*Minmodln(du1,du2)*hr/ch(xx,yy);
        cu2l(xx2l,yy2l)=cu(xx,yy)+0.5*Minmodln(du1,du2)*hl/ch(xx,yy);
        cv2r(xx2r,yy2r)=cv(xx,yy)-0.5*Minmodln(dv1,dv2)*hr/ch(xx,yy);
        cv2l(xx2l,yy2l)=cv(xx,yy)+0.5*Minmodln(dv1,dv2)*hl/ch(xx,yy);
        cz2r(xx2r,yy2r)=cz(xx,yy)-0.5*(Minmodln(dz1+dh1,dz2+dh2)-Minmodln(dh1,dh2));
        cz2l(xx2l,yy2l)=cz(xx,yy)+0.5*(Minmodln(dz1+dh1,dz2+dh2)-Minmodln(dh1,dh2));
      }
    }

    //last line
    y = ch.height()-1;
    yy2 = ch2l.height()-1;
    xx2 = ch2l.start();
    for(int64_t xx = ch.start();xx<ch.width(); xx++,xx2++)
    {
      dh1 = ch(xx,y+1)-ch(xx,y+2);
      du1 = cu(xx,y+1)-cu(xx,y+2);
      dv1 = cv(xx,y+1)-cv(xx,y+2);
      dz1 = cz(xx,y+1)-cz(xx,y+2);
      dh2 = ch(xx,y)-ch(xx,y+1);
      du2 = cu(xx,y)-cu(xx,y+1);
      dv2 = cv(xx,y)-cv(xx,y+1);
      dz2 = cz(xx,y)-cz(xx,y+1);
      hl = ch(xx,y+1)-0.5*Minmodln(dh1,dh2);
      hr = ch(xx,y+1)+0.5*Minmodln(dh1,dh2);
      ch2l(xx2,yy2)=hr;
      cu2l(xx2,yy2)=cu(xx,y+1)+0.5*Minmodln(du1,du2)*hl/ch(xx,y+1);
      cv2l(xx2,yy2)=cv(xx,y+1)+0.5*Minmodln(dv1,dv2)*hl/ch(xx,y+1);
      cz2l(xx2,yy2)=cz(xx,y+1)+0.5*(Minmodln(dz1+dh1,dz2+dh2)-Minmodln(dh1,dh2));
    }
	
    //b =rdtsc();
    //std::cout<<"huvh2l rdtsc = "<<b-a<<std::endl;
        //------------delz1,delz2,delzc1,delzc2------------//
        //delz1 no communications
    //a=rdtsc();
          Controller<double,0,false> cdelz(delz1.getDMatrix());

          for(int64_t yy = cdelz.start(); yy<cdelz.height();yy++)
              for(int64_t xx = cdelz.start();xx<cdelz.width(); xx++)
                cdelz(xx,yy) = cz1r(xx,yy) - cz1l(xx,yy);

	  //b =rdtsc();
	  //std::cout<<"delzf rdtsc = "<<b-a<<std::endl;
        //delzc1 no communications
	  //a=rdtsc();
          Controller<double,0,false> cdelzc1(delzc1.getDMatrix());
          Controller<double,0,false> cdelzc2(delzc2.getDMatrix());

          for(int64_t yy = cdelzc1.start(); yy<cdelzc1.height();yy++)
          {
              for(int64_t xx = cdelzc1.start();xx<cdelzc1.width(); xx++)
              {
                cdelzc1(xx,yy) = cz1r(xx+1,yy) - cz1l(xx,yy);
                cdelzc2(xx,yy) = cz2r(xx,yy) - cz2l(xx,yy+1);
              }
          }

	  //b =rdtsc();
	  //std::cout<<"delzc1f rdtsc = "<<b-a<<std::endl;
        //delz2 no communications
	  //a=rdtsc();
          Controller<double,0,false> cdelz2(delz2.getDMatrix());

          for(int64_t yy = cdelz2.start(); yy<cdelz2.height();yy++)
              for(int64_t xx = cdelz2.start();xx<cdelz2.width(); xx++)
                cdelz2(xx,yy) = cz2r(xx,yy) - cz2l(xx,yy);
	  //b =rdtsc();
	  //std::cout<<"delzf rdtsc = "<<b-a<<std::endl;
        //-------------------------------------------------//

        bloc1(flux_left,flux_right,flux_bottom,flux_top);
        bloc2(h,u,v,q1,q2,hs,qs1,qs2,us,vs); //with check
        //check_ve_ca(hs,us,vs,qs1,qs2);
        //----------------------------------------------//

        //boundary conditions
        boundary(hs,us,vs);

        //========== order1huv1
        //communications
	//a=rdtsc();
	
        hs.getDMatrix()->getBorders();
        us.getDMatrix()->getBorders();
        vs.getDMatrix()->getBorders();
        z.getDMatrix()->getBorders();

	//b =rdtsc();
	//std::cout<<"Sync rdtsc = "<<b-a<<std::endl;

	//a=rdtsc();
  Controller<double,2,false> chs(hs.getDMatrix());
  Controller<double,2,false> cus(us.getDMatrix());
  Controller<double,2,false> cvs(vs.getDMatrix());

  yy1 = ch1l.start();
  for(int64_t yy = chs.start(); yy<chs.height();yy++,yy1++)
  {
    int64_t x = ch.start();
    dh1 = chs(x-1,yy)-chs(x-2,yy);
    du1 = cus(x-1,yy)-cus(x-2,yy);
    dv1 = cvs(x-1,yy)-cvs(x-2,yy);
    dz1 = cz(x-1,yy)-cz(x-2,yy);
    dh2 = chs(x,yy)-chs(x-1,yy);
    du2 = cus(x,yy)-cus(x-1,yy);
    dv2 = cvs(x,yy)-cvs(x-1,yy);
    dz2 = cz(x,yy)-cz(x-1,yy);
    hl = chs(x-1,yy)+0.5*Minmodln(dh1,dh2);
    hr = chs(x-1,yy)+0.5*Minmodln(dh1,dh2);
    ur = cus(x-1,yy)+0.5*Minmodln(du1,du2)*hl/chs(x-1,yy);
    vr = cvs(x-1,yy)+0.5*Minmodln(dv1,dv2)*hl/chs(x-1,yy);
    zr = cz(x-1,yy)+0.5*(Minmodln(dz1+dh1,dz2+dh2)-Minmodln(dh1,dh2));
    int64_t xx1 = ch1l.start();
    for(int64_t xx = chs.start()+1;xx<chs.width()-1; xx++,xx1++)
    {
      ch1l(xx1,yy1)=hr;
      cu1l(xx1,yy1)=ur;
      cv1l(xx1,yy1)=vr;
      cz1l(xx1,yy1)=zr;
      dh1 = dh2;
      du1 = du2;
      dv1 = dv2;
      dz1 = dz2;
      dh2 = chs(xx+1,yy)-chs(xx,yy);
      du2 = cus(xx+1,yy)-cus(xx,yy);
      dv2 = cvs(xx+1,yy)-cvs(xx,yy);
      dz2 = cz(xx+1,yy)-cz(xx,yy);
      hr = chs(xx,yy)+0.5*Minmodln(dh1,dh2);
      hl = chs(xx,yy)-0.5*Minmodln(dh1,dh2);
      ch1r(xx1,yy1)=hl;
      cu1r(xx1,yy1)=cus(xx,yy)-0.5*Minmodln(du1,du2)*hr/chs(xx,yy);
      cv1r(xx1,yy1)=cvs(xx,yy)-0.5*Minmodln(dv1,dv2)*hr/chs(xx,yy);
      cz1r(xx1,yy1)=cz(xx,yy)-0.5*(Minmodln(dz1+dh1,dz2+dh2)-Minmodln(dh1,dh2));
      ur = cus(xx,yy)+0.5*Minmodln(du1,du2)*hl/chs(xx,yy);
      vr=cvs(xx,yy)+0.5*Minmodln(dv1,dv2)*hl/chs(xx,yy);
      zr=cz(xx,yy)+0.5*(Minmodln(dz1+dh1,dz2+dh2)-Minmodln(dh1,dh2));
    }
    x = ch.width()-1;
    xx1 = ch1l.width()-1;
    ch1l(xx1,yy1)=hr;
    cu1l(xx1,yy1)=ur;
    cv1l(xx1,yy1)=vr;
    cz1l(xx1,yy1)=zr;
    dh1 = dh2;
    du1 = du2;
    dv1 = dv2;
    dz1 = dz2;
    dh2=chs(x+2,yy)-chs(x+1,yy);
    du2=chs(x+2,yy)-cus(x+1,yy);
    dv2=chs(x+2,yy)-cvs(x+1,yy);
    dz2=chs(x+2,yy)-cz(x+1,yy);
    ch1r(xx1,yy1) = chs(x+1,yy)+0.5*Minmodln(dh1,dh2);
    cu1r(xx1,yy1) = cus(x+1,yy)+0.5*Minmodln(du1,du2)*hl/chs(x+1,yy);
    cv1r(xx1,yy1) = cvs(x+1,yy)+0.5*Minmodln(dv1,dv2)*hl/chs(x+1,yy);
    cz1r(xx1,yy1) = cz(x+1,yy)+0.5*(Minmodln(dz1+dh1,dz2+dh2)-Minmodln(dh1,dh2));
  }

  //b =rdtsc();
  //std::cout<<"huvh1l rdtsc = "<<b-a<<std::endl;
  //========== order1huv1

        //========== order2huv2
    //no communications

  //a=rdtsc();

    //first line
    y = ch.start();
    yy2 = ch2r.start();
    xx2 = ch2r.start();
    for(int64_t xx = chs.start();xx<chs.width(); xx++,xx2++)
    {
      dh1 = chs(xx,y-1)-chs(xx,y);
      du1 = cus(xx,y-1)-cus(xx,y);
      dv1 = cvs(xx,y-1)-cvs(xx,y);
      dz1 = cz(xx,y-1)-cz(xx,y);
      dh2 = chs(xx,y-2)-chs(xx,y-1);
      du2 = cus(xx,y-2)-cus(xx,y-1);
      dv2 = cvs(xx,y-2)-cvs(xx,y-1);
      dz2 = cz(xx,y-2)-cz(xx,y-1);
      hl = chs(xx,y-1)-0.5*Minmodln(dh1,dh2);
      hr = chs(xx,y-1)+0.5*Minmodln(dh1,dh2);
      ch2r(xx2,yy2)=hl;
      cu2r(xx2,yy2)=cus(xx,y-1)-0.5*Minmodln(du1,du2)*hr/chs(xx,y-1);
      cv2r(xx2,yy2)=cvs(xx,y-1)-0.5*Minmodln(dv1,dv2)*hl/chs(xx,y-1);
      cz2r(xx2,yy2)=cz(xx,y-1)-0.5*(Minmodln(dz1+dh1,dz2+dh2)-Minmodln(dh1,dh2));
    }

    //other lines
    yy2l = ch2l.start();
    yy2r = ch2r.start()+1;
    for(int64_t yy = chs.start()+1; yy<chs.height()-1;yy++,yy2l++,yy2r++)
    {
      int64_t xx2l = ch2l.start();
      int64_t xx2r = ch2r.start();
      for(int64_t xx = chs.start();xx<chs.width(); xx++,xx2l++,xx2r++)
      {
        dh1 = chs(xx,yy)-chs(xx,yy+1);
        du1 = cus(xx,yy)-cus(xx,yy+1);
        dv1 = cvs(xx,yy)-cvs(xx,yy+1);
        dz1 = cz(xx,yy)-cz(xx,yy+1);
        dh2 = chs(xx,yy-1)-chs(xx,yy);
        du2 = cus(xx,yy-1)-cus(xx,yy);
        dv2 = cvs(xx,yy-1)-cvs(xx,yy);
        dz2 = cz(xx,yy-1)-cz(xx,yy);
        hl = chs(xx,yy)-0.5*Minmodln(dh1,dh2);
        hr = chs(xx,yy)+0.5*Minmodln(dh1,dh2);
        ch2r(xx2r,yy2r)=hl;
        ch2l(xx2l,yy2l)=hr;
        cu2r(xx2r,yy2r)=cus(xx,yy)-0.5*Minmodln(du1,du2)*hr/chs(xx,yy);
        cu2l(xx2l,yy2l)=cus(xx,yy)+0.5*Minmodln(du1,du2)*hl/chs(xx,yy);
        cv2r(xx2r,yy2r)=cvs(xx,yy)-0.5*Minmodln(dv1,dv2)*hr/chs(xx,yy);
        cv2l(xx2l,yy2l)=cvs(xx,yy)+0.5*Minmodln(dv1,dv2)*hl/chs(xx,yy);
        cz2r(xx2r,yy2r)=cz(xx,yy)-0.5*(Minmodln(dz1+dh1,dz2+dh2)-Minmodln(dh1,dh2));
        cz2l(xx2l,yy2l)=cz(xx,yy)+0.5*(Minmodln(dz1+dh1,dz2+dh2)-Minmodln(dh1,dh2));
      }
    }

    //last line
    y = chs.height()-1;
    yy2 = ch2l.height()-1;
    xx2 = ch2l.start();
    for(int64_t xx = chs.start();xx<chs.width(); xx++,xx2++)
    {
      dh1 = chs(xx,y+1)-chs(xx,y+2);
      du1 = cus(xx,y+1)-cus(xx,y+2);
      dv1 = cvs(xx,y+1)-cvs(xx,y+2);
      dz1 = cz(xx,y+1)-cz(xx,y+2);
      dh2 = chs(xx,y)-chs(xx,y+1);
      du2 = cus(xx,y)-cus(xx,y+1);
      dv2 = cvs(xx,y)-cvs(xx,y+1);
      dz2 = cz(xx,y)-cz(xx,y+1);
      hl = chs(xx,y+1)-0.5*Minmodln(dh1,dh2);
      hr = chs(xx,y+1)+0.5*Minmodln(dh1,dh2);
      ch2l(xx2,yy2)=hr;
      cu2l(xx2,yy2)=cus(xx,y+1)+0.5*Minmodln(du1,du2)*hl/chs(xx,y+1);
      cv2l(xx2,yy2)=cvs(xx,y+1)+0.5*Minmodln(dv1,dv2)*hl/chs(xx,y+1);
      cz2l(xx2,yy2)=cz(xx,y+1)+0.5*(Minmodln(dz1+dh1,dz2+dh2)-Minmodln(dh1,dh2));
    }

    //b =rdtsc();
    //std::cout<<"huvh2l rdtsc = "<<b-a<<std::endl;

        //------------delz1,delz2,delzc1,delzc2------------//
        //delz1 no communications
    //a=rdtsc();
          for(int64_t yy = cdelz.start(); yy<cdelz.height();yy++)
              for(int64_t xx = cdelz.start();xx<cdelz.width(); xx++)
                cdelz(xx,yy) = cz1r(xx,yy) - cz1l(xx,yy);
	  //b =rdtsc();
	  //std::cout<<"delzf rdtsc = "<<b-a<<std::endl;
        //delzc1 no communications
	  //a=rdtsc();
          for(int64_t yy = cdelzc1.start(); yy<cdelzc1.height();yy++)
          {
              for(int64_t xx = cdelzc1.start();xx<cdelzc1.width(); xx++)
              {
                cdelzc1(xx,yy) = cz1r(xx+1,yy) - cz1l(xx,yy);
                cdelzc2(xx,yy) = cz2r(xx,yy) - cz2l(xx,yy+1);
              }
          }
	  //b =rdtsc();
	  //std::cout<<"delzc1f rdtsc = "<<b-a<<std::endl;
        //delz2 no communications
	  //a=rdtsc();
          for(int64_t yy = cdelz2.start(); yy<cdelz2.height();yy++)
              for(int64_t xx = cdelz2.start();xx<cdelz2.width(); xx++)
                cdelz2(xx,yy) = cz2r(xx,yy) - cz2l(xx,yy);
	  //b =rdtsc();
	  //std::cout<<"delzf rdtsc = "<<b-a<<std::endl;
        //-------------------------------------------------//
        bloc1(flux_left,flux_right,flux_bottom,flux_top);
        bloc22(hs,us,vs,qs1,qs2,hsa,qsa1,qsa2,h,q1,q2); //with Heun
        //----------------------------------------------//
	   //Heun_hq(h,q1,q2,hsa,qsa1,qsa2);
        //check_ve_ca(h,u,v,q1,q2);

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
