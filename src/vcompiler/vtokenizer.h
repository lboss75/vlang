#ifndef vtokenizer_H
#define vtokenizer_H

#include "vtoken.h"
#include "text_symbol.h"
#include "tokenizer.h"

namespace vds {
  class text_stream;
  
  class vtokenizer : public tokenizer<vtoken>
  {
  public:
    vtokenizer(
      text_stream & s,
      const char ** operators);
    
    const vtoken & next();
    
  private:
    const char ** operators_;
  };
}

#endif // vtokenizer_H
