#ifndef RAIN_PARALLEL_H
#define RAIN_PARALLEL_H

#include "misc.hpp"

extern SCALAR rain_intensity;

//RAIN
//-------------------------------------------------------------------------------
BEGINApplyOneMat(rain_f,input,SCALAR,output,SCALAR)
//-------------------------------------------------------------------------------
{
  DMatrix<SCALAR>::iterator itBeg=input->begin();
  DMatrix<SCALAR>::iterator itEnd=input->end();

  for(;itBeg<=itEnd;++itBeg)
  {
    output->setValue(rain_intensity,itBeg);
  }
}
END(rain_f);
//-------------------------------------------------------------------------------

#endif
