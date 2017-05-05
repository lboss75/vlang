#ifndef VLEX_H
#define VLEX_H

#include "vtoken.h"
#include "lex_type.h"

namespace vds {
 
  struct vlex
  {
    lex_type type;
    vtoken token;    
  };
}

#endif // VLEX_H
