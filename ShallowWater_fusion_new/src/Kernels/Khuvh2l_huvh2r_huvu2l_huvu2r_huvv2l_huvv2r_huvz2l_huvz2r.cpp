#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controller.hpp>
#include <kernelh.h>

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)

class Khuvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r:virtual public Go
{
public:
  Datah* h;
  Datah* u;
  Datah* v;
  Datah* z;
  Datah* h2l;
  Datah* h2r;
  Datah* u2l;
  Datah* u2r;
  Datah* v2l;
  Datah* v2r;
  Datah* z2l;
  Datah* z2r;

  Khuvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r(){}
  ~Khuvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r(){}
  
  void go ()
  {
    //cout<<"====START Khuvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r===="<<endl;
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
    double hr,hl,ul,vl,zl,ur,vr,zr;

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
      hl = ch(xx,y-1)-0.5*Minmod(dh1,dh2);
      hr = ch(xx,y-1)+0.5*Minmod(dh1,dh2);
      ch2r(xx2,yy2)=hl;
      cu2r(xx2,yy2)=cu(xx,y-1)-0.5*Minmod(du1,du2)*hr/ch(xx,y-1);
      cv2r(xx2,yy2)=cv(xx,y-1)-0.5*Minmod(dv1,dv2)*hl/ch(xx,y-1);
      cz2r(xx2,yy2)=cz(xx,y-1)-0.5*(Minmod(dz1+dh1,dz2+dh2)-Minmod(dh1,dh2));
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
        hl = ch(xx,yy)-0.5*Minmod(dh1,dh2);
        hr = ch(xx,yy)+0.5*Minmod(dh1,dh2);
        ch2r(xx2r,yy2r)=hl;
        ch2l(xx2l,yy2l)=hr;

        cu2r(xx2r,yy2r)=cu(xx,yy)-0.5*Minmod(du1,du2)*hr/ch(xx,yy);
        cu2l(xx2l,yy2l)=cu(xx,yy)+0.5*Minmod(du1,du2)*hl/ch(xx,yy);
        cv2r(xx2r,yy2r)=cv(xx,yy)-0.5*Minmod(dv1,dv2)*hr/ch(xx,yy);
        cv2l(xx2l,yy2l)=cv(xx,yy)+0.5*Minmod(dv1,dv2)*hl/ch(xx,yy);
        cz2r(xx2r,yy2r)=cz(xx,yy)-0.5*(Minmod(dz1+dh1,dz2+dh2)-Minmod(dh1,dh2));
        cz2l(xx2l,yy2l)=cz(xx,yy)+0.5*(Minmod(dz1+dh1,dz2+dh2)-Minmod(dh1,dh2));
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
      hl = ch(xx,y+1)-0.5*Minmod(dh1,dh2);
      hr = ch(xx,y+1)+0.5*Minmod(dh1,dh2);
      ch2l(xx2,yy2)=hr;

      cu2l(xx2,yy2)=cu(xx,y+1)+0.5*Minmod(du1,du2)*hl/ch(xx,y+1);
      cv2l(xx2,yy2)=cv(xx,y+1)+0.5*Minmod(dv1,dv2)*hl/ch(xx,y+1);
      cz2l(xx2,yy2)=cz(xx,y+1)+0.5*(Minmod(dz1+dh1,dz2+dh2)-Minmod(dh1,dh2));
    }
    delete [] hf;
    delete [] uf;
    delete [] vf;
    delete [] zf;
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

#include <Kernels/Khuvh2l_huvh2r_huvu2l_huvu2r_huvv2l_huvv2r_huvz2l_huvz2r.l2c>
