#include "skelgis.hpp"
#include "order2_parallel.hpp"
#include "scheme_parallel.hpp"

using namespace skelgis;

int Mpi_::level_provided=0;
int Mpi_::mpi_rank=0;
int Mpi_::mpi_nb=0;

//-------------------------------------------------------------------------------
int main(int argc, char** argv)
//-------------------------------------------------------------------------------
{
  INITSKELGIS;

  int64_t gW = 10000;
  int64_t gH = 10000;

  //mesh
  Structure * str = new Structure(gW,gH);

  //data
  Data * h = new Data(2,1,1.0,str);
  Data * u = new Data(2,1,1.0,str);
  Data * v = new Data(2,1,1.0,str);
  Data * z = new Data(2,1,1.0,str);
  Data * hs = new Data(2,1,1.0,str);
  Data * us = new Data(2,1,1.0,str);
  Data * vs = new Data(2,1,1.0,str);
  Data * hsa = new Data(2,1,1.0,str);
  Data * usa = new Data(2,1,1.0,str);
  Data * vsa = new Data(2,1,1.0,str);
  Data * q1 = new Data(0,1,1.0,str);
  Data * q2 = new Data(0,1,1.0,str);
  Data * qs1 = new Data(0,1,1.0,str);
  Data * qs2 = new Data(0,1,1.0,str);
  Data * f1 = new Data(0,2,0.0,str);
  Data * f2 = new Data(0,2,0.0,str);
  Data * f3 = new Data(0,2,0.0,str);
  Data * g1 = new Data(0,3,0.0,str);
  Data * g2 = new Data(0,3,0.0,str);
  Data * g3 = new Data(0,3,0.0,str);
  Data * h1d = new Data(0,2,0.0,str);
  Data * h1g = new Data(0,2,0.0,str);
  Data * h1l = new Data(0,2,0.0,str);
  Data * h1r = new Data(0,2,0.0,str);
  Data * h2d = new Data(0,3,0.0,str);
  Data * h2g = new Data(0,3,0.0,str);
  Data * h2l = new Data(0,3,0.0,str);
  Data * h2r = new Data(0,3,0.0,str);
  Data * delz1 = new Data(0,2,0.0,str);
  Data * delz2 = new Data(0,3,0.0,str);
  Data * delzc1 = new Data(0,1,0.0,str);
  Data * delzc2 = new Data(0,1,0.0,str);
  Data * qsa1 = new Data(0,1,0.0,str);
  Data * qsa2 = new Data(0,1,0.0,str);
  Data * u1l = new Data(0,2,0.0,str);
  Data * u1r = new Data(0,2,0.0,str);
  Data * u2l = new Data(0,3,0.0,str);
  Data * u2r = new Data(0,3,0.0,str);
  Data * v1l = new Data(0,2,0.0,str);
  Data * v1r = new Data(0,2,0.0,str);
  Data * v2l = new Data(0,3,0.0,str);
  Data * v2r = new Data(0,3,0.0,str);
  Data * z1l = new Data(0,2,0.0,str);
  Data * z1r = new Data(0,2,0.0,str);
  Data * z2l = new Data(0,3,0.0,str);
  Data * z2r = new Data(0,3,0.0,str);
  Data * Rain = new Data(0,1,0.0,str);
  Data * rainm = new Data(0,1,0.0,str);
  Data * mcf = new Data(0,1,0.0,str);

  //-----Time calculation
  Chronometer chrono;
  chrono.start();
  //-----

  for(int i=0;i<500;i++)
  {
    std::vector<Data *> inputs;
    std::vector<Data *> outputs;
    outputs.push_back(h);
    outputs.push_back(v);
    outputs.push_back(u);

    ApplyList::apply(boundary_f,inputs,outputs);

    inputs.clear();
    outputs.clear();

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

      ApplyList::apply(order2_huv1_f,inputs,outputs);
      
      outputs.clear();
      outputs.push_back(h2l);
      outputs.push_back(h2r);
      outputs.push_back(u2l);
      outputs.push_back(u2r);
      outputs.push_back(v2l);
      outputs.push_back(v2r);
      outputs.push_back(z2l);
      outputs.push_back(z2r);

      ApplyList::apply(order2_huv2_f,inputs,outputs);

      inputs.clear();
      outputs.clear();
      
      //------------delz1,delz2,delzc1,delzc2------------//
      //delz1
      ApplyBinary::apply(order2_delz_f,z1l,z1r,delz1);
      //delzc1
      ApplyBinary::apply(order2_delzc1_f,z1l,z1r,delzc1);
      //delz2
      ApplyBinary::apply(order2_delz_f,z2l,z2r,delz2);
      //delz1
      ApplyBinary::apply(order2_delzc2_f,z2l,z2r,delzc2);
      //-------------------------------------------------//

      inputs.clear();
      outputs.clear();
      
      inputs.push_back(h1l);
      inputs.push_back(h1r);
      inputs.push_back(u1l);
      inputs.push_back(u1r);
      inputs.push_back(v1l);
      inputs.push_back(v1r);
      inputs.push_back(delz1);
      outputs.push_back(h1g);
      outputs.push_back(h1d);
      outputs.push_back(f1);
      outputs.push_back(f2);
      outputs.push_back(f3);
      ApplyList::apply(bloc1x_f,inputs,outputs);

      inputs.clear();
      outputs.clear();
      inputs.push_back(h2l);
      inputs.push_back(h2r);
      inputs.push_back(u2l);
      inputs.push_back(u2r);
      inputs.push_back(v2l);
      inputs.push_back(v2r);
      inputs.push_back(delz2);
      outputs.push_back(h2g);
      outputs.push_back(h2d);
      outputs.push_back(g1);
      outputs.push_back(g2);
      outputs.push_back(g3);
      ApplyList::apply(bloc1y_f,inputs,outputs);

      inputs.clear();
      outputs.clear();

      inputs.push_back(h);
      inputs.push_back(u);
      inputs.push_back(v);
      inputs.push_back(hs);

      outputs.push_back(q1);
      outputs.push_back(q2);
      outputs.push_back(h1l);
      outputs.push_back(h1r);
      outputs.push_back(h1g);
      outputs.push_back(h1d);
      outputs.push_back(h2l);
      outputs.push_back(h2r);
      outputs.push_back(h2g);
      outputs.push_back(h2d);
      outputs.push_back(delzc1);
      outputs.push_back(delzc2);
      outputs.push_back(f1);
      outputs.push_back(f2);
      outputs.push_back(f3);
      outputs.push_back(g1);
      outputs.push_back(g2);
      outputs.push_back(g3);

      outputs.push_back(qs1);
      outputs.push_back(qs2);

      ApplyList::apply(bloc2_f,inputs,outputs);

      inputs.clear();
      inputs.push_back(qs1);
      inputs.push_back(qs2);
      outputs.clear();
      outputs.push_back(hs);
      outputs.push_back(us);
      outputs.push_back(vs);
      
      ApplyList::apply(ve_ca_f,inputs,outputs);
      //----------------------------------------------//

      //boundary conditions
      inputs.clear();
      outputs.clear();
      outputs.push_back(hs);
      outputs.push_back(us);
      outputs.push_back(vs);

      ApplyList::apply(boundary_f,inputs,outputs);

      inputs.clear();
      outputs.clear();
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
      
      ApplyList::apply(order2_huv1_f,inputs,outputs);

      outputs.clear();
      outputs.push_back(h2l);
      outputs.push_back(h2r);
      outputs.push_back(u2l);
      outputs.push_back(u2r);
      outputs.push_back(v2l);
      outputs.push_back(v2r);
      outputs.push_back(z2l);
      outputs.push_back(z2r);

      ApplyList::apply(order2_huv2_f,inputs,outputs);

      inputs.clear();
      outputs.clear();

      //------------delz1,delz2,delzc1,delzc2------------//
      //delz1
      ApplyBinary::apply(order2_delz_f,z1l,z1r,delz1);
      //delzc1
      ApplyBinary::apply(order2_delzc1_f,z1l,z1r,delzc1);
      //delz2
      ApplyBinary::apply(order2_delz_f,z2l,z2r,delz2);
      //delzc1
      ApplyBinary::apply(order2_delzc2_f,z2l,z2r,delzc2);
      //-------------------------------------------------//

      inputs.clear();
      outputs.clear();
      inputs.push_back(h1l);
      inputs.push_back(h1r);
      inputs.push_back(u1l);
      inputs.push_back(u1r);
      inputs.push_back(v1l);
      inputs.push_back(v1r);
      inputs.push_back(delz1);
      outputs.push_back(h1g);
      outputs.push_back(h1d);
      outputs.push_back(f1);
      outputs.push_back(f2);
      outputs.push_back(f3);
      ApplyList::apply(bloc1x_f,inputs,outputs);

      inputs.clear();
      outputs.clear();
      inputs.push_back(h2l);
      inputs.push_back(h2r);
      inputs.push_back(u2l);
      inputs.push_back(u2r);
      inputs.push_back(v2l);
      inputs.push_back(v2r);
      inputs.push_back(delz2);
      outputs.push_back(h2g);
      outputs.push_back(h2d);
      outputs.push_back(g1);
      outputs.push_back(g2);
      outputs.push_back(g3);
      ApplyList::apply(bloc1y_f,inputs,outputs);

      inputs.clear();
      outputs.clear();
      inputs.push_back(hs);
      inputs.push_back(us);
      inputs.push_back(vs);
      inputs.push_back(hsa);

      //R=0 pas de pb
      outputs.push_back(qs1);
      outputs.push_back(qs2);
      outputs.push_back(h1l);
      outputs.push_back(h1r);
      outputs.push_back(h1g);
      outputs.push_back(h1d);
      outputs.push_back(h2l);
      outputs.push_back(h2r);
      outputs.push_back(h2g);
      outputs.push_back(h2d);
      outputs.push_back(delzc1);
      outputs.push_back(delzc2);
      outputs.push_back(f1);
      outputs.push_back(f2);
      outputs.push_back(f3);
      outputs.push_back(g1);
      outputs.push_back(g2);
      outputs.push_back(g3);

      outputs.push_back(qsa1);
      outputs.push_back(qsa2);

      ApplyList::apply(bloc2_f,inputs,outputs);

      //Heun
      inputs.clear();
      outputs.clear();
      outputs.push_back(h);
      inputs.push_back(q1);
      inputs.push_back(q2);
      outputs.push_back(hsa);
      inputs.push_back(qsa1);
      inputs.push_back(qsa2);

      ApplyList::apply(Heun_hq_f,inputs,outputs);

      //check
      inputs.clear();
      inputs.push_back(qs1);
      inputs.push_back(qs2);
      outputs.clear();
      outputs.push_back(hs);
      outputs.push_back(us);
      outputs.push_back(vs);
      
      ApplyList::apply(ve_ca_f,inputs,outputs);
         
  }

  //-----Time calculation
  chrono.stop();
  std::stringstream st;
  st<<" Time TOTAL CALCULATION : "<<chrono.dureeCalcule()<<" ";
  Mpi_::printScreen(st.str());
  //-----

  delete h;
  delete u;
  delete v;
  delete z;
  delete hs;
  delete us;
  delete vs;
  delete hsa;
  delete usa;
  delete vsa;
  delete q1;
  delete q2;
  delete qs1;
  delete qs2;
  delete f1;
  delete f2;
  delete f3;
  delete g1;
  delete g2;
  delete g3;
  delete h1d;
  delete h1g;
  delete h1l;
  delete h1r;
  delete h2d;
  delete h2g;
  delete h2l;
  delete h2r;
  delete delz1;
  delete delz2;
  delete delzc1;
  delete delzc2;
  delete qsa1;
  delete qsa2;
  delete u1l;
  delete u1r;
  delete u2l;
  delete u2r;
  delete v1l;
  delete v1r;
  delete v2l;
  delete v2r;
  delete z1l;
  delete z1r;
  delete z2l;
  delete z2r;
  delete Rain;
  delete rainm;
  delete mcf;

  delete str;

  ENDSKELGIS;
}
