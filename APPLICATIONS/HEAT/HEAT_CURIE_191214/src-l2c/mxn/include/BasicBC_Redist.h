#ifndef _ITF_BASICBC_REDIST_H
#define _ITF_BASICBC_REDIST_H

#include <vector>

namespace mxn {

  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  
  // Not exposed to user but to Redist
  struct BasicBCDescr
  {
	 typedef struct id_info_s { 
		unsigned gstart;
		unsigned long len;
	 } id_info_t;
	 typedef std::vector<id_info_t> ids_t;

    // Total data size (in element)
    unsigned long glen;
    // UnitSize -> size of an element (in octect)
    // IMPOSSIBLE: 32bit vs 64bit: can vary !!! unsigned long usz;
    // BlockSize -> distribution type: number of element in one bloc (in element)
    unsigned long bsz;

    // Sequence of gstart for each block element; (in element)
    ids_t ids;
  };
  
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

} // namespace

#endif
