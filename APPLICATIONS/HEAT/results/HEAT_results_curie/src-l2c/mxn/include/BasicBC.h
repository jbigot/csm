#ifndef _ITF_BASICBC_H
#define _ITF_BASICBC_H

namespace mxn
{

namespace impl
{

  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  
  class BasicBC
  {
  public:
	 // Data description API
	 virtual void setEltSize(unsigned long size)=0;
	 virtual void setBlocSize(unsigned long bsz)=0; // bloc-> bc(0), cyclic -> bc(1)
	 virtual void setTotalNbElt(unsigned long elt_nb)=0;

	 virtual unsigned long getBlocSize()=0;
	 virtual unsigned long getLocalNbElt()=0;
	 virtual unsigned long getTotalNbElt()=0;

	 virtual unsigned long getFirst()=0;
	 virtual unsigned long getLast()=0;
	 virtual unsigned long getStep()=0;
  };
  
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

} // namespace

}

#endif
