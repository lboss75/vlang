#ifndef __VTOKEN_H_
#define __VTOKEN_H_

#include <string>
#include "text_token.h"

namespace vds {
  enum vtoken_type
  {
    TT_EOF,
    TT_WORD,
    TT_NUMBER,
    TT_STRING,
    TT_OPERATOR
  };

  struct vtoken : public text_token<vtoken_type>
  {
  };
}

#endif // __VTOKEN_H_
