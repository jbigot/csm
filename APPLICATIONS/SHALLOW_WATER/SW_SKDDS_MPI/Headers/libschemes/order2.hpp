#ifndef Class_scheme
#include "scheme.hpp"
#endif

#ifndef Class_rec_muscl
#include "rec_muscl.hpp"
#endif

#ifndef Class_order2
#define Class_order2

class order2: public scheme {
private:

  TAB hsa,usa,vsa;
  TAB0 qsa1,qsa2,z1l,z1r,z2l,z2r;

  rec_muscl * rec;

public:
  /*!
   * \brief Constructor
   */
  order2(parameters * &);

  void Heun_hq(TAB &, TAB0 &, TAB0 &, TAB &, TAB0 &, TAB0 &);

  /*!
   * \brief Destructor
   */
  virtual ~order2();

  void calcul();
};
#endif
