#ifndef __TOKENIZER_H_
#define __TOKENIZER_H_

#include "text_token.h"
#include "text_symbol.h"
#include "text_stream.h"
#include "parse_error.h"

namespace vds {
  template <typename token>
  class tokenizer
  {
  public:
    tokenizer(text_stream & s);
    
    const token & current() const
    {
      return this->current_;
    }
   
    const std::string & filename() const;

  protected:
    const text_symbol & next_symbol() const {
      return this->next_symbol_;
    }

    void set_current(typename token::token_type type, int line, int column, const std::string & value);
    std::string & set_current(typename token::token_type type, int line, int column);
    std::string & set_current(typename token::token_type type);

    bool read_next();

    void compile_error(const std::string & message);

    bool is_eof() const {
      return this->s_.is_eof();
    }

  private:
    text_stream & s_;
    token current_;
    text_symbol next_symbol_;
  };

  template<typename token>
  inline tokenizer<token>::tokenizer(text_stream & s)
    : s_(s)
  {
    if (s.is_eof()) {
      this->current_.type = token::token_type::TT_EOF;
    }
    else {
      this->next_symbol_ = s.read();
    }
  }

  template<typename token>
  inline const std::string & tokenizer<token>::filename() const
  {
    return this->s_.filename();
  }

  template<typename token>
  inline bool tokenizer<token>::read_next()
  {
    if (this->s_.is_eof()) {
      return false;
    }

    this->next_symbol_ = this->s_.read();
    return true;
  }

  template<typename token>
  inline void tokenizer<token>::set_current(typename token::token_type type, int line, int column, const std::string & value)
  {
    this->current_.type = type;
    this->current_.line = line;
    this->current_.column = column;
    this->current_.value = value;
  }

  template<typename token>
  inline std::string & tokenizer<token>::set_current(typename token::token_type type, int line, int column)
  {
    this->current_.type = type;
    this->current_.line = line;
    this->current_.column = column;
    this->current_.value.clear();

    return this->current_.value;
  }

  template<typename token>
  inline std::string & tokenizer<token>::set_current(typename token::token_type type)
  {
    this->current_.type = type;
    this->current_.line = this->next_symbol_.line;
    this->current_.column = this->next_symbol_.column;
    this->current_.value.clear();

    return this->current_.value;
  }

  template<typename token>
  inline void tokenizer<token>::compile_error(const std::string & message)
  {
    throw new parse_error(
      this->s_.filename(),
      this->current_.line,
      this->current_.column,
      message);
  }
}

#endif // __TOKENIZER_H_
