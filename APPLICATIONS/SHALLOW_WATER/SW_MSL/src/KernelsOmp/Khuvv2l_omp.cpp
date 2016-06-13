#include <iostream>
#include <llcmcpp/llcmcpp3.h>
#include <llcmcpp/go.h>

#include <datah.h>
#include <controller.hpp>
#include <kernelh.h>

#include <omp.h>
#define CHUNK 5

typedef llcmcpp::Go Go;

using ::std::cout;
using ::std::endl;

#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)

class Khuvv2l_omp:virtual public Go
{
public:
  Datah* h;
  Datah* u;
  Datah* v;
  Datah* z;
  Datah* v2l;

  Khuvv2l_omp(){}
  ~Khuvv2l_omp(){}
  
  void go ()
  {
    //cout<<"====START Khuvv2l===="<<endl;
    Controller<double> ch(h);
    Controller<double> cv(v);
    Controller<double> cv2l(v2l);

    double dh1,dh2,dv1,dv2;
    double hl;

    //other lines
    int64_t yy2l = cv2l.start();
    int64_t xx,yy,xx2l;

//dynamic also possible
#pragma omp parallel for shared(ch,cv,cv2l) private(yy,xx,xx2l,dh1,dh2,dv1,dv2,hl) firstprivate(yy2l) schedule (static, CHUNK)
    for(yy = ch.start()+1; yy<ch.height()-1;yy++)
    {
      xx2l = cv2l.start();
      for(xx = ch.start();xx<ch.width(); xx++)
      {
        dh1 = ch(xx,yy)-ch(xx,yy+1);
        dv1 = cv(xx,yy)-cv(xx,yy+1);
        dh2 = ch(xx,yy-1)-ch(xx,yy);
        dv2 = cv(xx,yy-1)-cv(xx,yy);
        hl = ch(xx,yy)-0.5*Minmod(dh1,dh2);
        cv2l(xx2l,yy2l)=cv(xx,yy)+0.5*Minmod(dv1,dv2)*hl/ch(xx,yy);

        xx2l++;
      }
      yy2l++;
    }

    //last line
    int64_t y = ch.height()-1;
    int64_t yy2 = cv2l.height()-1;
    int64_t xx2 = cv2l.start();

//dynamic also possible
#pragma omp parallel for shared(ch,cv,cv2l) private(yy,xx,dh1,dh2,dv1,dv2,hl) firstprivate(y,yy2,xx2) schedule (static, CHUNK)
    for(xx = ch.start();xx<ch.width(); xx++)
    {
      dh1 = ch(xx,y+1)-ch(xx,y+2);
      dv1 = cv(xx,y+1)-cv(xx,y+2);
      dh2 = ch(xx,y)-ch(xx,y+1);
      dv2 = cv(xx,y)-cv(xx,y+1);
      hl = ch(xx,y+1)-0.5*Minmod(dh1,dh2);
      cv2l(xx2,yy2)=cv(xx,y+1)+0.5*Minmod(dv1,dv2)*hl/ch(xx,y+1);

      xx2++;
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

#include <KernelsOmp/Khuvv2l_omp.l2c>
