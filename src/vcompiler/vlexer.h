#ifndef VLEXER_H
#define VLEXER_H

#include "vlex.h"
#include "vtoken.h"

namespace vds {
  class vtokenizer;
  
  class vlexer
  {
  public:
    vlexer(
      vtokenizer & tokenizer,
      const char ** keywords);
       
    const vlex & current() const
    {
      return this->current_;
    }
    
    const vlex & next();
    
    const std::string & filename() const;
   
  private:
    vtokenizer & tokenizer_;
    vlex current_;
    vtoken next_token_;
    const char ** keywords_;
    
    void after_next();
  };
}


#endif // VLEXER_H
