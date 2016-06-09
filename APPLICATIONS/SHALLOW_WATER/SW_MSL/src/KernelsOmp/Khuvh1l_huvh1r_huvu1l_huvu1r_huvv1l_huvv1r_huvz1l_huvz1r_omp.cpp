#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controller.hpp>
#include <kernelh.h>

#include <omp.h>
#define CHUNK 5

#include "libskelgis/skelgis.hpp"

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)

class Khuvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r_omp:virtual public Go
{
public:
  Datah* h;
  Datah* u;
  Datah* v;
  Datah* z;
  Datah* h1l;
  Datah* h1r;
  Datah* u1l;
  Datah* u1r;
  Datah* v1l;
  Datah* v1r;
  Datah* z1l;
  Datah* z1r;

  Khuvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r_omp(){}
  ~Khuvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r_omp(){}
  
  void go ()
  {
    //cout<<"====START Khuvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r===="<<endl;
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
    double hr,hl,ul,vl,zl,ur,vr,zr;

    int64_t yy1 = ch1l.start();

//dynamic also possible
#pragma omp parallel for shared(ch,cu,cv,cz,ch1l,ch1r,cu1l,cu1r,cv1l,cv1r,cz1l,cz1r) private(yy,xx,yy1,xx1,dh1,dh2,du1,du2,dv1,dv2,dz1,dz2,hr,hl,ul,vl,zl,ur,vr,zr) schedule (static, CHUNK)
    for(int64_t yy = ch.start(); yy<ch.height();yy++)
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
      hl = ch(x-1,yy)+0.5*Minmod(dh1,dh2);
      hr = ch(x-1,yy)+0.5*Minmod(dh1,dh2);
      ur = cu(x-1,yy)+0.5*Minmod(du1,du2)*hl/ch(x-1,yy);
      vr = cv(x-1,yy)+0.5*Minmod(dv1,dv2)*hl/ch(x-1,yy);
      zr = cz(x-1,yy)+0.5*(Minmod(dz1+dh1,dz2+dh2)-Minmod(dh1,dh2));

      int64_t xx1 = ch1l.start();
      for(int64_t xx = ch.start()+1;xx<ch.width()-1; xx++)
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
        hr = ch(xx,yy)+0.5*Minmod(dh1,dh2);
        hl = ch(xx,yy)-0.5*Minmod(dh1,dh2);
        ch1r(xx1,yy1)=hl;

        cu1r(xx1,yy1)=cu(xx,yy)-0.5*Minmod(du1,du2)*hr/ch(xx,yy);
        cv1r(xx1,yy1)=cv(xx,yy)-0.5*Minmod(dv1,dv2)*hr/ch(xx,yy);
        cz1r(xx1,yy1)=cz(xx,yy)-0.5*(Minmod(dz1+dh1,dz2+dh2)-Minmod(dh1,dh2));
        ur = cu(xx,yy)+0.5*Minmod(du1,du2)*hl/ch(xx,yy);
        vr=cv(xx,yy)+0.5*Minmod(dv1,dv2)*hl/ch(xx,yy);
        zr=cz(xx,yy)+0.5*(Minmod(dz1+dh1,dz2+dh2)-Minmod(dh1,dh2));

        xx1++;
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
      ch1r(xx1,yy1) = ch(x+1,yy)+0.5*Minmod(dh1,dh2);

      cu1r(xx1,yy1) = cu(x+1,yy)+0.5*Minmod(du1,du2)*hl/ch(x+1,yy);
      cv1r(xx1,yy1) = cv(x+1,yy)+0.5*Minmod(dv1,dv2)*hl/ch(x+1,yy);
      cz1r(xx1,yy1) = cz(x+1,yy)+0.5*(Minmod(dz1+dh1,dz2+dh2)-Minmod(dh1,dh2));

      yy1++;
    }
  }

private:
  double Minmod(double a, double b)
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

};

#include <KernelsOmp/Khuvh1l_huvh1r_huvu1l_huvu1r_huvv1l_huvv1r_huvz1l_huvz1r_omp.l2c>
