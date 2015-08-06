#ifndef ORDER2_PARALLEL_H
#define ORDER2_PARALLEL_H

#include "order2.hpp"

//INIT DELZ
//-------------------------------------------------------------------------------
BEGINApplyBinaryBlock(order2_delz_f,zl,SCALAR,zr,SCALAR,0,delz,SCALAR,0)
//-------------------------------------------------------------------------------
{
  iTAB0 it = delz.begin();
  iTAB0 it_End = delz.end();

  SCALAR delztemp;

  for(; it<=it_End; ++it) {
    delz[it] = zr[it] - zl[it];
  }
}
END(order2_delz_f);

//INIT DELZC1
//-------------------------------------------------------------------------------
BEGINApplyBinaryBlock(order2_delzc1_f,zl,SCALAR,zr,SCALAR,0,delzc1,SCALAR,0)
//-------------------------------------------------------------------------------
{
  iTAB0L itc_line = delzc1.begin_line();
  iTAB0L itc_line_End = delzc1.end_line();
  iTAB0L itl_line = zl.begin_line();
  
  for(;itc_line<=itc_line_End;++itc_line,++itl_line)
    {
      iTAB0 itc = itc_line.begin();
      iTAB0 itcEnd = itc_line.end();
      iTAB0 itl = itl_line.begin();
      iTAB0 itr = itl_line.begin();
      ++itr;

      for(;itc<=itcEnd;++itc,++itl,++itr)
	delzc1[itc] = zr[itr]-zl[itl];
    }
}
END(order2_delzc1_f);
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
BEGINApplyBinaryBlock(order2_delzc2_f,zl,SCALAR,zr,SCALAR,0,delzc2,SCALAR,0)
//-------------------------------------------------------------------------------
{
  iTAB0L itc_line = delzc2.begin_line();
  iTAB0L itc_line_End = delzc2.end_line();
  iTAB0L itu_line = zr.begin_line();
  iTAB0L itd_line = zl.begin_line();
  ++itd_line;

  for(;itc_line<=itc_line_End;++itc_line,++itu_line,++itd_line)
    {
      iTAB0 itc = itc_line.begin();
      iTAB0 itcEnd = itc_line.end();
      iTAB0 itu = itu_line.begin();
      iTAB0 itd = itd_line.begin();

      for(;itc<=itcEnd;++itc,++itu,++itd)
	delzc2[itc] = zr[itu]-zl[itd];
    }
}
END(order2_delzc2_f);
//-------------------------------------------------------------------------------
//CACUL huv1
//-------------------------------------------------------------------------------
BEGINApplyListBlock(order2_huv1_f,inputs,SCALAR,2,outputs,SCALAR,0)
//-------------------------------------------------------------------------------
{
  TAB h(inputs[0]);
  TAB u(inputs[1]);
  TAB v(inputs[2]);
  TAB z(inputs[3]);
  TAB0 h1l(outputs[0]);
  TAB0 h1r(outputs[1]);
  TAB0 u1l(outputs[2]);
  TAB0 u1r(outputs[3]);
  TAB0 v1l(outputs[4]);
  TAB0 v1r(outputs[5]);
  TAB0 z1l(outputs[6]);
  TAB0 z1r(outputs[7]);

  parameters * par = parameters::getSingleton();
  rec_muscl * rec = rec_muscl::getSingleton(par->get_lim());

  SCALAR dh1,dh2,du1,du2,dv1,dv2,dz1,dz2;
  SCALAR hl,ul,vl,zl,hr,ur,vr,zr;

  SCALAR * hf = new SCALAR[4];
  SCALAR * uf = new SCALAR[4];
  SCALAR * vf = new SCALAR[4];
  SCALAR * zf = new SCALAR[4];

  iTABL it_2_line = h.begin_line();
  iTABL it_2_line_End = h.end_line();
  iTAB0L it_0_line = h1l.begin_line();

  for(;it_2_line<=it_2_line_End;++it_2_line,++it_0_line)
    {
      iTAB it_2 = it_2_line.begin();
      iTAB it_2_End = it_2_line.end();
      iTAB0 it_0 = it_0_line.begin();
      iTAB0 it_0_End = it_0_line.end();

      h.getAllX(it_2,hf);
      u.getAllX(it_2,uf);
      v.getAllX(it_2,vf);
      z.getAllX(it_2,zf);
      //attention ordre: 0--1--it--2--3

      // a la cellule fictive
      dh1 = hf[1]-hf[0];
      du1 = uf[1]-uf[0];
      dv1 = vf[1]-vf[0];
      dz1 = zf[1]-zf[0];
      dh2 = h[it_2]-hf[1];
      du2 = u[it_2]-uf[1];
      dv2 = v[it_2]-vf[1];
      dz2 = z[it_2]-zf[1];
      rec->calcul(hf[1],uf[1],vf[1],zf[1],dh1,dh2,du1,du2,dv1,dv2,dz1,dz2);
      hr = rec->get_hr();
      ur = rec->get_ur();
      vr = rec->get_vr();
      zr = rec->get_zr();

      for(;it_2<=it_2_End;++it_2,++it_0)
	{
	  //--------------------------------//
	  // a gauche de l'interface
	  h1l[it_0]=hr;
	  u1l[it_0]=ur;
	  v1l[it_0]=vr;
	  z1l[it_0]=zr;
	  // calculer la pente
	  dh1 = dh2;
	  du1 = du2;
	  dv1 = dv2;
	  dz1 = dz2;
	  dh2 = h.getRight(it_2) - h[it_2];
	  du2 = u.getRight(it_2) - u[it_2];
	  dv2 = v.getRight(it_2) - v[it_2];
	  dz2 = z.getRight(it_2) - z[it_2];
	  rec->calcul(h[it_2],u[it_2],v[it_2],z[it_2],dh1,dh2,du1,du2,dv1,dv2,dz1,dz2);
	  h1r[it_0] = rec->get_hl();
	  u1r[it_0] = rec->get_ul();
	  v1r[it_0] = rec->get_vl();
	  z1r[it_0] = rec->get_zl();
	  // pour l'interface suivante
	  hr = rec->get_hr();
	  ur = rec->get_ur();
	  vr = rec->get_vr();
	  zr = rec->get_zr();
	}
      //derniere element de la ligne calcule differemment
      // a gauche de l'interface
      h1l[it_0_End]=hr;
      u1l[it_0_End]=ur;
      v1l[it_0_End]=vr;
      z1l[it_0_End]=zr;

      dh1 = dh2;
      du1 = du2;
      dv1 = dv2;
      dz1 = dz2;

      // valeur sur les cellules fictives
      h.getAllX(it_2_End,hf);
      u.getAllX(it_2_End,uf);
      v.getAllX(it_2_End,vf);
      z.getAllX(it_2_End,zf);
      //attention ordre: 0--1--it--2--3
      // a la cellule fictive
      dh2 = hf[3]-hf[2];
      du2 = uf[3]-uf[2];
      dv2 = vf[3]-vf[2];
      dz2 = zf[3]-zf[2];
      rec->calcul(hf[2],uf[2],vf[2],zf[2],dh1,dh2,du1,du2,dv1,dv2,dz1,dz2);
      hl = rec->get_hl();
      ul = rec->get_ul();
      vl = rec->get_vl();
      zl = rec->get_zl();
      h1r[it_0_End] = hl;
      u1r[it_0_End] = ul;
      v1r[it_0_End] = vl;
      z1r[it_0_End] = zl; 
    }
  
  delete [] hf;
  delete [] uf;
  delete [] vf;
  delete [] zf;
}
END(order2_huv1_f);
//-------------------------------------------------------------------------------

//CACUL huv2
//-------------------------------------------------------------------------------
BEGINApplyListBlock(order2_huv2_f,inputs,SCALAR,2,outputs,SCALAR,0)
//-------------------------------------------------------------------------------
{
  TAB h(inputs[0]);
  TAB u(inputs[1]);
  TAB v(inputs[2]);
  TAB z(inputs[3]);
  TAB0 h2l(outputs[0]);
  TAB0 h2r(outputs[1]);
  TAB0 u2l(outputs[2]);
  TAB0 u2r(outputs[3]);
  TAB0 v2l(outputs[4]);
  TAB0 v2r(outputs[5]);
  TAB0 z2l(outputs[6]);
  TAB0 z2r(outputs[7]);

  parameters * par = parameters::getSingleton();
  rec_muscl * rec = rec_muscl::getSingleton(par->get_lim());

  SCALAR dh1,dh2,du1,du2,dv1,dv2,dz1,dz2;
  SCALAR hl,ul,vl,zl,hr,ur,vr,zr;

  SCALAR * hf= new SCALAR[4];
  SCALAR * uf= new SCALAR[4];
  SCALAR * vf= new SCALAR[4];
  SCALAR * zf= new SCALAR[4];

  iTABL it_2_line = h.begin_line();
  iTAB0L it_0r_line = h2l.begin_line();

  //first line
  iTAB it_2 = it_2_line.begin();
  iTAB it_2_End = it_2_line.end();
  iTAB0 it_0r = it_0r_line.begin();
  for(;it_2<=it_2_End;++it_2,++it_0r)
    {
      // valeur sur les cellules fictives
      h.getAllY(it_2,hf);
      u.getAllY(it_2,uf);
      v.getAllY(it_2,vf);
      z.getAllY(it_2,zf);
      //                 0
      //                 1
      //attention ordre: it
      //                 2
      //                 3
      // a la cellule fictive
      dh1 = hf[1]-h[it_2];
      du1 = uf[1]-u[it_2];
      dv1 = vf[1]-v[it_2];
      dz1 = zf[1]-z[it_2];
      dh2 = hf[0]-hf[1];
      du2 = uf[0]-uf[1];
      dv2 = vf[0]-vf[1];
      dz2 = zf[0]-zf[1];

      rec->calcul(hf[1],uf[1],vf[1],zf[1],dh1,dh2,du1,du2,dv1,dv2,dz1,dz2);
      hl = rec->get_hl();
      ul = rec->get_ul();
      vl = rec->get_vl();
      zl = rec->get_zl();
      h2r[it_0r]=hl;
      u2r[it_0r]=ul;
      v2r[it_0r]=vl;
      z2r[it_0r]=zl;
    }
  //other lines of h
  ++it_0r_line;
  it_2 = h.begin();
  it_0r = it_0r_line.begin();
  iTAB it_hEnd = h.end();
  iTAB0 it_0l = h2l.begin();

  for(; it_2<=it_hEnd; ++it_2,++it_0l,++it_0r) {
    dh1 = h[it_2]-h.getDown(it_2);
    du1 = u[it_2]-u.getDown(it_2);
    dv1 = v[it_2]-v.getDown(it_2);
    dz1 = z[it_2]-z.getDown(it_2);
    dh2 = h.getUp(it_2)-h[it_2];
    du2 = u.getUp(it_2)-u[it_2];
    dv2 = v.getUp(it_2)-v[it_2];
    dz2 = z.getUp(it_2)-z[it_2];
    rec->calcul(h[it_2],u[it_2],v[it_2],z[it_2],dh1,dh2,du1,du2,dv1,dv2,dz1,dz2);
    hl = rec->get_hl();
    ul = rec->get_ul();
    vl = rec->get_vl();
    zl = rec->get_zl();
    // a droite de l'interface
    h2r[it_0r]=hl;
    u2r[it_0r]=ul;
    v2r[it_0r]=vl;
    z2r[it_0r]=zl;
    // pour l'interface suivante
    h2l[it_0l] = rec->get_hr();
    u2l[it_0l] = rec->get_ur();
    v2l[it_0l] = rec->get_vr();
    z2l[it_0l] = rec->get_zr();
  }

  iTAB0 it_0End = h2l.end();
  
  for(; it_0End>=it_0l; --it_0End,--it_hEnd) {
    // valeur sur les cellules fictives
    h.getAllY(it_hEnd,hf);
    u.getAllY(it_hEnd,uf);
    v.getAllY(it_hEnd,vf);
    z.getAllY(it_hEnd,zf);
    //                 0
    //                 1
    //attention ordre: it
    //                 2
    //                 3
    // a la cellule fictive
    dh1 = hf[2]-hf[3];
    du1 = uf[2]-uf[3];
    dv1 = vf[2]-vf[3];
    dz1 = zf[2]-zf[3];
    dh2 = h[it_hEnd]-hf[2];
    du2 = u[it_hEnd]-uf[2];
    dv2 = v[it_hEnd]-vf[2];
    dz2 = z[it_hEnd]-zf[2];

    rec->calcul(hf[2],uf[2],vf[2],zf[2],dh1,dh2,du1,du2,dv1,dv2,dz1,dz2);
    h2l[it_0End] = rec->get_hr();
    u2l[it_0End] = rec->get_ur();
    v2l[it_0End] = rec->get_vr();
    z2l[it_0End] = rec->get_zr();
  }
  delete [] hf;
  delete [] uf;
  delete [] vf;
  delete [] zf;
}
END(order2_huv2_f);
//-------------------------------------------------------------------------------

//Heun_HQ
//-------------------------------------------------------------------------------
BEGINApplyListBlock(Heun_hq_f,inputs,SCALAR,0,outputs,SCALAR,2)
//-------------------------------------------------------------------------------
{
  TAB h(outputs[0]);
  TAB hs(outputs[1]);

  TAB0 q1(inputs[0]);
  TAB0 q2(inputs[1]);
  TAB0 qs1(inputs[2]);
  TAB0 qs2(inputs[3]);

  iTAB it_2 = h.begin();
  iTAB0 it_0 = q1.begin();
  iTAB it_hEnd = h.end();
  for(; it_2<=it_hEnd; it_2++,++it_0) {
    h[it_2] = 0.5*(h[it_2]+hs[it_2]);
    q1[it_0] = 0.5*(q1[it_0]+qs1[it_0]);
    q2[it_0] = 0.5*(q2[it_0]+qs2[it_0]);
  }
}
END(Heun_hq_f);

#endif
