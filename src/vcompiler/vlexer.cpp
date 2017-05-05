#include "stdafx.h"
#include "vlexer.h"
#include "vtokenizer.h"
#include "vtoken.h"
#include "compile_error.h"

vds::vlexer::vlexer(
  vds::vtokenizer& tokenizer,
  const char ** keywords)
: tokenizer_(tokenizer), keywords_(keywords)
{
  this->next_token_ = tokenizer.current();
  this->after_next();
}

const vds::vlex& vds::vlexer::next()
{
  this->next_token_ = this->tokenizer_.next();
  this->after_next();
  return this->current_;
}

void vds::vlexer::after_next()
{
  if (TT_WORD == this->next_token_.type) {
    for(const char ** kw = this->keywords_; nullptr != *kw; ++kw) {
      if(*kw == this->next_token_.value) {
        this->current_.type = LT_KEYWORD;
        this->current_.token = this->next_token_;
        return;
      }
    }
    
    this->current_.type = LT_NAME;
    this->current_.token = this->next_token_;
    return;
  }
  
  if(this->next_token_.type == vds::TT_OPERATOR) {
    this->current_.type = LT_OPERATOR;
    this->current_.token = this->next_token_;
    return;
  }
  
  if (this->next_token_.type == vds::TT_STRING) {
    this->current_.type = LT_STRING;
    this->current_.token = this->next_token_;
    return;
  }

  if (this->next_token_.type == vds::TT_NUMBER) {
    this->current_.type = LT_NUMBER;
    this->current_.token = this->next_token_;
    return;
  }
  
  if (this->next_token_.type == vds::TT_EOF) {
    this->current_.type = LT_EOF;
    this->current_.token = this->next_token_;
    return;
  }
  
  throw new compile_error(
    this->tokenizer_.filename(),
    this->next_token_.line,
    this->next_token_.column,
    "Invalid");
}

const std::string& vds::vlexer::filename() const
{
  return this->tokenizer_.filename();
}
