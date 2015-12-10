#ifndef ORDER1_PARALLEL_H
#define ORDER1_PARALLEL_H

#include "order1.hpp"

//INIT DELZ1
//-------------------------------------------------------------------------------
BEGINApplyUnaryBlock(order1_delz1_f,z,SCALAR,2,delz1,SCALAR,0)
//-------------------------------------------------------------------------------
{
  // iTABL it_z_line = z.begin_line();
  // iTABL it_z_End = z.end_line();
  // iTAB0L it_del_line = delz1.begin_line();

  // for(;it_z_line<=it_z_End;++it_z_line,++it_del_line)
  //   {
  //     iTAB it_z = it_z_line.begin();
  //     iTAB it_end = it_z_End.end();
  //     iTAB0 it_del = it_del_line.begin();

  //     for(;it_z<=it_end;++it_z,++it_del)
  //       delz1[it_del] = z[it_z] - z.getLeft(it_z);

  //     delz1[it_del] = z.getRight(it_end) - z[it_end];
  //   }
}
END(order1_delz1_f);
//-------------------------------------------------------------------------------

//INIT DELZ2
//-------------------------------------------------------------------------------
BEGINApplyUnaryBlock(order1_delz2_f,z,SCALAR,2,delz2,SCALAR,0)
//-------------------------------------------------------------------------------
{
  // iTAB it_z = z.begin();
  // iTAB0 it_del = delz2.begin();
  // iTAB it_zEnd = z.end();

  // for(; it_z<=it_zEnd; ++it_del,++it_z)
  //   delz2[it_del] = z.getUp(it_z) - z[it_z];

  // iTAB0 it_delEnd = delz2.end();

  // for(; it_delEnd>=it_del; --it_delEnd,--it_zEnd)
  //   delz2[it_delEnd] = z[it_zEnd] - z.getDown(it_zEnd);
}
END(order1_delz2_f);
//-------------------------------------------------------------------------------

//CACUL huv1
//-------------------------------------------------------------------------------
BEGINApplyListBlock(order1_huv1_f,inputs,SCALAR,2,outputs,SCALAR,0)
//-------------------------------------------------------------------------------
{
 //  TAB h(inputs[0]);
 //  TAB u(inputs[1]);
 //  TAB v(inputs[2]);
 //  TAB0 h1l(outputs[0]);
 //  TAB0 h1r(outputs[1]);
 //  TAB0 u1l(outputs[2]);
 //  TAB0 u1r(outputs[3]);
 //  TAB0 v1l(outputs[4]);
 //  TAB0 v1r(outputs[5]);

 //  iTABL it_2_line = h.begin_line();
 //  iTABL it_2_lineEnd = h.end_line();
 //  iTAB0L it_0_line = h1l.begin_line();

 //  for(;it_2_line<=it_2_lineEnd;++it_2_line,++it_0_line)
 //    {
 //      iTAB it_2 = it_2_line.begin();
 //      iTAB it_end = it_2_line.end();
 //      iTAB0 it_0 = it_0_line.begin();

 //      for(;it_2<=it_end;++it_2,++it_0)
	// {
	//   h1r[it_0] = h[it_2];
	//   u1r[it_0] = u[it_2];
	//   v1r[it_0] = v[it_2];

	//   h1l[it_0] = h.getLeft(it_2);
	//   u1l[it_0] = u.getLeft(it_2);
	//   v1l[it_0] = v.getLeft(it_2);
	// }

 //      h1r[it_0] = h.getRight(it_end);
 //      u1r[it_0] = u.getRight(it_end);
 //      v1r[it_0] = v.getRight(it_end);
 //      h1l[it_0] = h[it_end];
 //      u1l[it_0] = u[it_end];
 //      v1l[it_0] = v[it_end];
 //    }
}
END(order1_huv1_f);
//-------------------------------------------------------------------------------

//CACUL huv2
//-------------------------------------------------------------------------------
BEGINApplyListBlock(order1_huv2_f,inputs,SCALAR,2,outputs,SCALAR,0)
//-------------------------------------------------------------------------------
{
  // TAB h(inputs[0]);
  // TAB u(inputs[1]);
  // TAB v(inputs[2]);
  // TAB0 h2l(outputs[0]);
  // TAB0 h2r(outputs[1]);
  // TAB0 u2l(outputs[2]);
  // TAB0 u2r(outputs[3]);
  // TAB0 v2l(outputs[4]);
  // TAB0 v2r(outputs[5]);

  // iTAB it_2 = h.begin();
  // iTAB0 it_1 = h2l.begin();
  // iTAB it_hEnd = h.end();

  // for(; it_2<=it_hEnd; ++it_2,++it_1) {
  //   h2l[it_1] = h[it_2];
  //   u2l[it_1] = u[it_2];
  //   v2l[it_1] = v[it_2];

  //   h2r[it_1] = h.getUp(it_2);
  //   u2r[it_1] = u.getUp(it_2);
  //   v2r[it_1] = v.getUp(it_2);
  // }

  // iTAB0 it_1End = h2l.end();

  // for(; it_1End>=it_1; --it_1End,--it_hEnd) {
  //   h2r[it_1End] = h[it_hEnd];
  //   u2r[it_1End] = u[it_hEnd];
  //   v2r[it_1End] = v[it_hEnd];

  //   h2l[it_1End] = h.getDown(it_hEnd);
  //   u2l[it_1End] = u.getDown(it_hEnd);
  //   v2l[it_1End] = v.getDown(it_hEnd);
  // }
}
END(order1_huv2_f);
//-------------------------------------------------------------------------------

#endif
