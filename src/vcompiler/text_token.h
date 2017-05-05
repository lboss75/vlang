#ifndef __TEXT_TOKEN_H_
#define __TEXT_TOKEN_H_

#include <string>

namespace vds {
  template<typename tokentype>
  struct text_token
  {
    typedef tokentype token_type;
    token_type type;
    int line;
    int column;
    std::string value;    
  };
}

#endif // __TEXT_TOKEN_H_
