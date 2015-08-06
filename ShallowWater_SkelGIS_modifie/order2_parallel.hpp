#ifndef ORDER2_PARALLEL_H
#define ORDER2_PARALLEL_H

#include "util/macros.hpp"

#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)

using namespace skelgis;

double Minmod2(double a, double b)
{
    double rec;
    if(a>=0. && b>=0.)
      rec=min(a,b);
    else if(a<=0. && b<=0.)
      rec=max(a,b);
    else
      rec=0.;
    return rec;
}

//INIT DELZ
//-------------------------------------------------------------------------------
BEGINApplyBinaryBlock(order2_delz_f,zl,zr,delz)
//-------------------------------------------------------------------------------
{
  //std::cout<<"test"<<std::endl;
  Controller<double> cz1l(zl);
  Controller<double> cz1r(zr);
  Controller<double> cdelz(delz);

  for(int64_t yy = cdelz.start(); yy<cdelz.height();yy++)
      for(int64_t xx = cdelz.start();xx<cdelz.width(); xx++)
        cdelz(xx,yy) = cz1r(xx,yy) - cz1l(xx,yy);
  
  // iTAB0 it = delz.begin();
  // iTAB0 it_End = delz.end();

  // SCALAR delztemp;

  // for(; it<=it_End; ++it) {
  //   delz[it] = zr[it] - zl[it];
  // }
}
END(order2_delz_f);

//INIT DELZC1
//-------------------------------------------------------------------------------
BEGINApplyBinaryBlock(order2_delzc1_f,zl,zr,delzc1)
//-------------------------------------------------------------------------------
{
  Controller<double> cz1l(zl);
  Controller<double> cz1r(zr);
  Controller<double> cdelzc1(delzc1);

  for(int64_t yy = cdelzc1.start(); yy<cdelzc1.height();yy++)
      for(int64_t xx = cdelzc1.start();xx<cdelzc1.width(); xx++)
        cdelzc1(xx,yy) = cz1r(xx+1,yy) - cz1l(xx,yy);     
//  iTAB0L itc_line = delzc1.begin_line();
 //  iTAB0L itc_line_End = delzc1.end_line();
 //  iTAB0L itl_line = zl.begin_line();
  
 //  for(;itc_line<=itc_line_End;++itc_line,++itl_line)
 //    {
 //      iTAB0 itc = itc_line.begin();
 //      iTAB0 itcEnd = itc_line.end();
 //      iTAB0 itl = itl_line.begin();
 //      iTAB0 itr = itl_line.begin();
 //      ++itr;

 //      for(;itc<=itcEnd;++itc,++itl,++itr)
	// delzc1[itc] = zr[itr]-zl[itl];
 //    }
}
END(order2_delzc1_f);
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
BEGINApplyBinaryBlock(order2_delzc2_f,zl,zr,delzc2)
//-------------------------------------------------------------------------------
{  
  Controller<double> cz2l(zl);
  Controller<double> cz2r(zr);
  Controller<double> cdelzc2(delzc2);

  for(int64_t yy = cdelzc2.start(); yy<cdelzc2.height();yy++)
    for(int64_t xx = cdelzc2.start();xx<cdelzc2.width(); xx++)
        cdelzc2(xx,yy) = cz2r(xx,yy) - cz2l(xx,yy+1);
 
//  iTAB0L itc_line = delzc2.begin_line();
 //  iTAB0L itc_line_End = delzc2.end_line();
 //  iTAB0L itu_line = zr.begin_line();
 //  iTAB0L itd_line = zl.begin_line();
 //  ++itd_line;

 //  for(;itc_line<=itc_line_End;++itc_line,++itu_line,++itd_line)
 //    {
 //      iTAB0 itc = itc_line.begin();
 //      iTAB0 itcEnd = itc_line.end();
 //      iTAB0 itu = itu_line.begin();
 //      iTAB0 itd = itd_line.begin();

 //      for(;itc<=itcEnd;++itc,++itu,++itd)
	// delzc2[itc] = zr[itu]-zl[itd];
 //    }
}
END(order2_delzc2_f);
//-------------------------------------------------------------------------------
//CACUL huv1
//-------------------------------------------------------------------------------
BEGINApplyListBlock(order2_huv1_f,inputs,outputs)
//-------------------------------------------------------------------------------
{
  Data * h = inputs[0];
  Data * u = inputs[1];
  Data * v = inputs[2];
  Data * z = inputs[3];
  Data * h1l = outputs[0];
  Data *h1r = outputs[1];
  Data * u1l = outputs[2];
  Data * u1r = outputs[3];
  Data *v1l=outputs[4];
  Data *v1r=outputs[5];
  Data *z1l=outputs[6];
  Data *z1r=outputs[7];

  // parameters * par = parameters::getSingleton();
  // rec_muscl * rec = rec_muscl::getSingleton(par->get_lim());

  Controller<double> ch(h);
  Controller<double> cu(u);
  Controller<double> cv(v);
  Controller<double> cz(z);
  Controller<double> ch1l(h1l);
  Controller<double> ch1r(h1r);
  Controller<double> cu1l(u1l);
  Controller<double> cu1r(u1r);
  Controller<double> cv1l(v1l);
  Controller<double> cv1r(v1r);
  Controller<double> cz1l(z1l);
  Controller<double> cz1r(z1r);

  double dh1,dh2,du1,du2,dv1,dv2,dz1,dz2;
  double hl,ul,vl,zl,hr,ur,vr,zr;

  double * hf = new double[4];
  double * uf = new double[4];
  double * vf = new double[4];
  double * zf = new double[4];

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
    hl = ch(x-1,yy)+0.5*Minmod2(dh1,dh2);
    hr = ch(x-1,yy)+0.5*Minmod2(dh1,dh2);
    ur = cu(x-1,yy)+0.5*Minmod2(du1,du2)*hl/ch(x-1,yy);
    vr = cv(x-1,yy)+0.5*Minmod2(dv1,dv2)*hl/ch(x-1,yy);
    zr = cz(x-1,yy)+0.5*(Minmod2(dz1+dh1,dz2+dh2)-Minmod2(dh1,dh2));
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
      hr = ch(xx,yy)+0.5*Minmod2(dh1,dh2);
      hl = ch(xx,yy)-0.5*Minmod2(dh1,dh2);
      ch1r(xx1,yy1)=hl;
      cu1r(xx1,yy1)=cu(xx,yy)-0.5*Minmod2(du1,du2)*hr/ch(xx,yy);
      cv1r(xx1,yy1)=cv(xx,yy)-0.5*Minmod2(dv1,dv2)*hr/ch(xx,yy);
      cz1r(xx1,yy1)=cz(xx,yy)-0.5*(Minmod2(dz1+dh1,dz2+dh2)-Minmod2(dh1,dh2));
      ur = cu(xx,yy)+0.5*Minmod2(du1,du2)*hl/ch(xx,yy);
      vr=cv(xx,yy)+0.5*Minmod2(dv1,dv2)*hl/ch(xx,yy);
      zr=cz(xx,yy)+0.5*(Minmod2(dz1+dh1,dz2+dh2)-Minmod2(dh1,dh2));
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
    ch1r(xx1,yy1) = ch(x+1,yy)+0.5*Minmod2(dh1,dh2);
    cu1r(xx1,yy1) = cu(x+1,yy)+0.5*Minmod2(du1,du2)*hl/ch(x+1,yy);
    cv1r(xx1,yy1) = cv(x+1,yy)+0.5*Minmod2(dv1,dv2)*hl/ch(x+1,yy);
    cz1r(xx1,yy1) = cz(x+1,yy)+0.5*(Minmod2(dz1+dh1,dz2+dh2)-Minmod2(dh1,dh2));
  }
  delete [] hf;
  delete [] uf;
  delete [] vf;
  delete [] zf;
  
 //  SCALAR dh1,dh2,du1,du2,dv1,dv2,dz1,dz2;
 //  SCALAR hl,ul,vl,zl,hr,ur,vr,zr;

 //  SCALAR * hf = new SCALAR[4];
 //  SCALAR * uf = new SCALAR[4];
 //  SCALAR * vf = new SCALAR[4];
 //  SCALAR * zf = new SCALAR[4];

 //  iTABL it_2_line = h.begin_line();
 //  iTABL it_2_line_End = h.end_line();
 //  iTAB0L it_0_line = h1l.begin_line();

 //  for(;it_2_line<=it_2_line_End;++it_2_line,++it_0_line)
 //    {
 //      iTAB it_2 = it_2_line.begin();
 //      iTAB it_2_End = it_2_line.end();
 //      iTAB0 it_0 = it_0_line.begin();
 //      iTAB0 it_0_End = it_0_line.end();

 //      h.getAllX(it_2,hf);
 //      u.getAllX(it_2,uf);
 //      v.getAllX(it_2,vf);
 //      z.getAllX(it_2,zf);
 //      //attention ordre: 0--1--it--2--3

 //      // a la cellule fictive
 //      dh1 = hf[1]-hf[0];
 //      du1 = uf[1]-uf[0];
 //      dv1 = vf[1]-vf[0];
 //      dz1 = zf[1]-zf[0];
 //      dh2 = h[it_2]-hf[1];
 //      du2 = u[it_2]-uf[1];
 //      dv2 = v[it_2]-vf[1];
 //      dz2 = z[it_2]-zf[1];
 //      rec->calcul(hf[1],uf[1],vf[1],zf[1],dh1,dh2,du1,du2,dv1,dv2,dz1,dz2);
 //      hr = rec->get_hr();
 //      ur = rec->get_ur();
 //      vr = rec->get_vr();
 //      zr = rec->get_zr();

 //      for(;it_2<=it_2_End;++it_2,++it_0)
	// {
	//   //--------------------------------//
	//   // a gauche de l'interface
	//   h1l[it_0]=hr;
	//   u1l[it_0]=ur;
	//   v1l[it_0]=vr;
	//   z1l[it_0]=zr;
	//   // calculer la pente
	//   dh1 = dh2;
	//   du1 = du2;
	//   dv1 = dv2;
	//   dz1 = dz2;
	//   dh2 = h.getRight(it_2) - h[it_2];
	//   du2 = u.getRight(it_2) - u[it_2];
	//   dv2 = v.getRight(it_2) - v[it_2];
	//   dz2 = z.getRight(it_2) - z[it_2];
	//   rec->calcul(h[it_2],u[it_2],v[it_2],z[it_2],dh1,dh2,du1,du2,dv1,dv2,dz1,dz2);
	//   h1r[it_0] = rec->get_hl();
	//   u1r[it_0] = rec->get_ul();
	//   v1r[it_0] = rec->get_vl();
	//   z1r[it_0] = rec->get_zl();
	//   // pour l'interface suivante
	//   hr = rec->get_hr();
	//   ur = rec->get_ur();
	//   vr = rec->get_vr();
	//   zr = rec->get_zr();
	// }
 //      //derniere element de la ligne calcule differemment
 //      // a gauche de l'interface
 //      h1l[it_0_End]=hr;
 //      u1l[it_0_End]=ur;
 //      v1l[it_0_End]=vr;
 //      z1l[it_0_End]=zr;

 //      dh1 = dh2;
 //      du1 = du2;
 //      dv1 = dv2;
 //      dz1 = dz2;

 //      // valeur sur les cellules fictives
 //      h.getAllX(it_2_End,hf);
 //      u.getAllX(it_2_End,uf);
 //      v.getAllX(it_2_End,vf);
 //      z.getAllX(it_2_End,zf);
 //      //attention ordre: 0--1--it--2--3
 //      // a la cellule fictive
 //      dh2 = hf[3]-hf[2];
 //      du2 = uf[3]-uf[2];
 //      dv2 = vf[3]-vf[2];
 //      dz2 = zf[3]-zf[2];
 //      rec->calcul(hf[2],uf[2],vf[2],zf[2],dh1,dh2,du1,du2,dv1,dv2,dz1,dz2);
 //      hl = rec->get_hl();
 //      ul = rec->get_ul();
 //      vl = rec->get_vl();
 //      zl = rec->get_zl();
 //      h1r[it_0_End] = hl;
 //      u1r[it_0_End] = ul;
 //      v1r[it_0_End] = vl;
 //      z1r[it_0_End] = zl; 
 //    }
  
 //  delete [] hf;
 //  delete [] uf;
 //  delete [] vf;
 //  delete [] zf;
}
END(order2_huv1_f);
//-------------------------------------------------------------------------------

//CACUL huv2
//-------------------------------------------------------------------------------
BEGINApplyListBlock(order2_huv2_f,inputs,outputs)
//-------------------------------------------------------------------------------
{
  Data * h = inputs[0];
  Data * u = inputs[1];
  Data * v = inputs[2];
  Data * z=inputs[3];
  Data * h2l=outputs[0];
  Data * h2r=outputs[1];
  Data * u2l=outputs[2];
  Data * u2r=outputs[3];
  Data * v2l=outputs[4];
  Data * v2r=outputs[5];
  Data * z2l=outputs[6];
  Data * z2r=outputs[7];

  // parameters * par = parameters::getSingleton();
  // rec_muscl * rec = rec_muscl::getSingleton(par->get_lim());

  Controller<double> ch(h);
    Controller<double> cu(u);
    Controller<double> cv(v);
    Controller<double> cz(z);
    Controller<double> ch2l(h2l);
    Controller<double> ch2r(h2r);
    Controller<double> cu2l(u2l);
    Controller<double> cu2r(u2r);
    Controller<double> cv2l(v2l);
    Controller<double> cv2r(v2r);
    Controller<double> cz2l(z2l);
    Controller<double> cz2r(z2r);

    double dh1,dh2,du1,du2,dv1,dv2,dz1,dz2;
    double hl,ul,vl,zl,hr,ur,vr,zr;

    double * hf = new double[4];
    double * uf = new double[4];
    double * vf = new double[4];
    double * zf = new double[4];

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
      hl = ch(xx,y-1)-0.5*Minmod2(dh1,dh2);
      hr = ch(xx,y-1)+0.5*Minmod2(dh1,dh2);
      ch2r(xx2,yy2)=hl;
      cu2r(xx2,yy2)=cu(xx,y-1)-0.5*Minmod2(du1,du2)*hr/ch(xx,y-1);
      cv2r(xx2,yy2)=cv(xx,y-1)-0.5*Minmod2(dv1,dv2)*hl/ch(xx,y-1);
      cz2r(xx2,yy2)=cz(xx,y-1)-0.5*(Minmod2(dz1+dh1,dz2+dh2)-Minmod2(dh1,dh2));
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
        hl = ch(xx,yy)-0.5*Minmod2(dh1,dh2);
        hr = ch(xx,yy)+0.5*Minmod2(dh1,dh2);
        ch2r(xx2r,yy2r)=hl;
        ch2l(xx2l,yy2l)=hr;
        cu2r(xx2r,yy2r)=cu(xx,yy)-0.5*Minmod2(du1,du2)*hr/ch(xx,yy);
        cu2l(xx2l,yy2l)=cu(xx,yy)+0.5*Minmod2(du1,du2)*hl/ch(xx,yy);
        cv2r(xx2r,yy2r)=cv(xx,yy)-0.5*Minmod2(dv1,dv2)*hr/ch(xx,yy);
        cv2l(xx2l,yy2l)=cv(xx,yy)+0.5*Minmod2(dv1,dv2)*hl/ch(xx,yy);
        cz2r(xx2r,yy2r)=cz(xx,yy)-0.5*(Minmod2(dz1+dh1,dz2+dh2)-Minmod2(dh1,dh2));
        cz2l(xx2l,yy2l)=cz(xx,yy)+0.5*(Minmod2(dz1+dh1,dz2+dh2)-Minmod2(dh1,dh2));
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
      hl = ch(xx,y+1)-0.5*Minmod2(dh1,dh2);
      hr = ch(xx,y+1)+0.5*Minmod2(dh1,dh2);
      ch2l(xx2,yy2)=hr;
      cu2l(xx2,yy2)=cu(xx,y+1)+0.5*Minmod2(du1,du2)*hl/ch(xx,y+1);
      cv2l(xx2,yy2)=cv(xx,y+1)+0.5*Minmod2(dv1,dv2)*hl/ch(xx,y+1);
      cz2l(xx2,yy2)=cz(xx,y+1)+0.5*(Minmod2(dz1+dh1,dz2+dh2)-Minmod2(dh1,dh2));
    }
    delete [] hf;
    delete [] uf;
    delete [] vf;
    delete [] zf;
 
  // SCALAR dh1,dh2,du1,du2,dv1,dv2,dz1,dz2;
  // SCALAR hl,ul,vl,zl,hr,ur,vr,zr;

  // SCALAR * hf= new SCALAR[4];
  // SCALAR * uf= new SCALAR[4];
  // SCALAR * vf= new SCALAR[4];
  // SCALAR * zf= new SCALAR[4];

  // iTABL it_2_line = h.begin_line();
  // iTAB0L it_0r_line = h2l.begin_line();

  // //first line
  // iTAB it_2 = it_2_line.begin();
  // iTAB it_2_End = it_2_line.end();
  // iTAB0 it_0r = it_0r_line.begin();
  // for(;it_2<=it_2_End;++it_2,++it_0r)
  //   {
  //     // valeur sur les cellules fictives
  //     h.getAllY(it_2,hf);
  //     u.getAllY(it_2,uf);
  //     v.getAllY(it_2,vf);
  //     z.getAllY(it_2,zf);
  //     //                 0
  //     //                 1
  //     //attention ordre: it
  //     //                 2
  //     //                 3
  //     // a la cellule fictive
  //     dh1 = hf[1]-h[it_2];
  //     du1 = uf[1]-u[it_2];
  //     dv1 = vf[1]-v[it_2];
  //     dz1 = zf[1]-z[it_2];
  //     dh2 = hf[0]-hf[1];
  //     du2 = uf[0]-uf[1];
  //     dv2 = vf[0]-vf[1];
  //     dz2 = zf[0]-zf[1];

  //     rec->calcul(hf[1],uf[1],vf[1],zf[1],dh1,dh2,du1,du2,dv1,dv2,dz1,dz2);
  //     hl = rec->get_hl();
  //     ul = rec->get_ul();
  //     vl = rec->get_vl();
  //     zl = rec->get_zl();
  //     h2r[it_0r]=hl;
  //     u2r[it_0r]=ul;
  //     v2r[it_0r]=vl;
  //     z2r[it_0r]=zl;
  //   }
  // //other lines of h
  // ++it_0r_line;
  // it_2 = h.begin();
  // it_0r = it_0r_line.begin();
  // iTAB it_hEnd = h.end();
  // iTAB0 it_0l = h2l.begin();

  // for(; it_2<=it_hEnd; ++it_2,++it_0l,++it_0r) {
  //   dh1 = h[it_2]-h.getDown(it_2);
  //   du1 = u[it_2]-u.getDown(it_2);
  //   dv1 = v[it_2]-v.getDown(it_2);
  //   dz1 = z[it_2]-z.getDown(it_2);
  //   dh2 = h.getUp(it_2)-h[it_2];
  //   du2 = u.getUp(it_2)-u[it_2];
  //   dv2 = v.getUp(it_2)-v[it_2];
  //   dz2 = z.getUp(it_2)-z[it_2];
  //   rec->calcul(h[it_2],u[it_2],v[it_2],z[it_2],dh1,dh2,du1,du2,dv1,dv2,dz1,dz2);
  //   hl = rec->get_hl();
  //   ul = rec->get_ul();
  //   vl = rec->get_vl();
  //   zl = rec->get_zl();
  //   // a droite de l'interface
  //   h2r[it_0r]=hl;
  //   u2r[it_0r]=ul;
  //   v2r[it_0r]=vl;
  //   z2r[it_0r]=zl;
  //   // pour l'interface suivante
  //   h2l[it_0l] = rec->get_hr();
  //   u2l[it_0l] = rec->get_ur();
  //   v2l[it_0l] = rec->get_vr();
  //   z2l[it_0l] = rec->get_zr();
  // }

  // iTAB0 it_0End = h2l.end();
  
  // for(; it_0End>=it_0l; --it_0End,--it_hEnd) {
  //   // valeur sur les cellules fictives
  //   h.getAllY(it_hEnd,hf);
  //   u.getAllY(it_hEnd,uf);
  //   v.getAllY(it_hEnd,vf);
  //   z.getAllY(it_hEnd,zf);
  //   //                 0
  //   //                 1
  //   //attention ordre: it
  //   //                 2
  //   //                 3
  //   // a la cellule fictive
  //   dh1 = hf[2]-hf[3];
  //   du1 = uf[2]-uf[3];
  //   dv1 = vf[2]-vf[3];
  //   dz1 = zf[2]-zf[3];
  //   dh2 = h[it_hEnd]-hf[2];
  //   du2 = u[it_hEnd]-uf[2];
  //   dv2 = v[it_hEnd]-vf[2];
  //   dz2 = z[it_hEnd]-zf[2];

  //   rec->calcul(hf[2],uf[2],vf[2],zf[2],dh1,dh2,du1,du2,dv1,dv2,dz1,dz2);
  //   h2l[it_0End] = rec->get_hr();
  //   u2l[it_0End] = rec->get_ur();
  //   v2l[it_0End] = rec->get_vr();
  //   z2l[it_0End] = rec->get_zr();
  // }
  // delete [] hf;
  // delete [] uf;
  // delete [] vf;
  // delete [] zf;
}
END(order2_huv2_f);
//-------------------------------------------------------------------------------

//Heun_HQ
//-------------------------------------------------------------------------------
BEGINApplyListBlock(Heun_hq_f,inputs,outputs)
//-------------------------------------------------------------------------------
{
  Data * h=outputs[0];
  Data * hs=outputs[1];

  Data * q1=inputs[0];
  Data * q2=inputs[1];
  Data * qs1=inputs[2];
  Data * qs2=inputs[3];

  Controller<double> chsa(hs);
    Controller<double> cqsa1(qs1);
    Controller<double> cqsa2(qs2);
    Controller<double> ch(h);
    Controller<double> cq1(q1);
    Controller<double> cq2(q2);

    int64_t yyq=cq1.start();
    for(int64_t yy = ch.start(); yy<ch.height();yy++,yyq++)
    {
      int64_t xxq=cq1.start();
      for(int64_t xx = ch.start();xx<ch.width(); xx++,xxq++)
      {
        ch(xx,yy)=0.5*(ch(xx,yy)+chsa(xx,yy));
        cq1(xxq,yyq)=0.5*(cq1(xxq,yyq)+cqsa1(xxq,yyq));
        cq2(xxq,yyq)=0.5*(cq2(xxq,yyq)+cqsa2(xxq,yyq));
      }
    }
  // iTAB it_2 = h.begin();
  // iTAB0 it_0 = q1.begin();
  // iTAB it_hEnd = h.end();
  // for(; it_2<=it_hEnd; it_2++,++it_0) {
  //   h[it_2] = 0.5*(h[it_2]+hs[it_2]);
  //   q1[it_0] = 0.5*(q1[it_0]+qs1[it_0]);
  //   q2[it_0] = 0.5*(q2[it_0]+qs2[it_0]);
  // }
}
END(Heun_hq_f);

#endif
