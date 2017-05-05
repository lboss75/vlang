#include "stdafx.h"
#include "vjson_file_parser.h"
#include "compile_error.h"
#include "vjson_object.h"

vds::vjson_file_parser::vjson_file_parser(vds::vlexer& lexer)
: lexer_(lexer), next_lex_(lexer.current())
{

}

void vds::vjson_file_parser::next(bool throwIfEof)
{
  this->next_lex_ = this->lexer_.next();
  if (throwIfEof && this->next_lex_.type == LT_EOF) {
    throw new compile_error(
      this->lexer_.filename(),
      this->next_lex_.token.line,
      this->next_lex_.token.column,
      "Unexpected end of file");
  }
}

void vds::vjson_file_parser::require(
  const std::string & value,
  lex_type type,
  vlex * catched)
{
  if (type != this->next_lex_.type
    || value != this->next_lex_.token.value
  ) {
    throw new compile_error(
      this->lexer_.filename(),
      this->next_lex_.token.line,
      this->next_lex_.token.column,
      value + " expected");
  }
  
  if (nullptr != catched) {
    *catched = this->next_lex_;
  }
  
  this->next(false);
}

bool vds::vjson_file_parser::optional(
  const std::string & value,
  lex_type type,
  vds::vlex * catched)
{
  if (type != this->next_lex_.type
    || value != this->next_lex_.token.value
  ) {
    return false;
  }
  
  if (nullptr != catched) {
    *catched = this->next_lex_;
  }
  
  this->next(false);
  return true;
}

void vds::vjson_file_parser::unexpected()
{
  throw new compile_error(
    this->lexer_.filename(),
    this->next_lex_.token.line,
    this->next_lex_.token.column,
    "Unexpected token " + this->next_lex_.token.value);
}

std::unique_ptr<vds::vjson_object> vds::vjson_file_parser::parse_json_object()
{
  std::unique_ptr<vds::vjson_object> result(
    new vjson_object(
      this->next_lex_.token.line,
      this->next_lex_.token.column));
  while (!this->optional("}")) {

    if (LT_STRING == this->next_lex_.type) {
      auto name = this->next_lex_.token;
      this->next();
      
      this->require(":");

      result->properties_.push_back(
        vjson_property{
          name.line,
          name.column,
          name.value,
          this->parse_json_value()});

      if (this->optional(",")) {
        continue;
      }
      
      if (this->optional("}")) {
        break;
      }
    }
    
    this->unexpected();
  }

  return result;
}

std::unique_ptr<vds::vjson_array> vds::vjson_file_parser::parse_json_array()
{
  std::unique_ptr<vds::vjson_array> result(new vjson_array(this->next_lex_.token.line, this->next_lex_.token.column));
  if(!this->optional("]")) {
    for (;;) {
      result->items_.push_back(this->parse_json_value());

      if (this->optional("]")) {
        break;
      }

      this->require(",");
    }
  }

  return result;
}


std::unique_ptr<vds::vjson_value> vds::vjson_file_parser::parse_json_value()
{
  if (LT_STRING == this->next_lex_.type
    || LT_NUMBER == this->next_lex_.type
  ) {
    auto value = this->next_lex_.token;
    this->next();
    return std::unique_ptr<vds::vjson_value>(
      new vjson_primitive(
        value.line,
        value.column,
        value.value));
  }
  
  if (this->optional("{")) {
    return this->parse_json_object();
  }

  if (this->optional("[")) {
    return this->parse_json_array();
  }

  this->unexpected();
  return std::unique_ptr<vds::vjson_value>();
}
