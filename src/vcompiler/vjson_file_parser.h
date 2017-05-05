#ifndef __VJSON_FILE_PARSER_H_
#define __VJSON_FILE_PARSER_H_

#include "lex_type.h"
#include "vlex.h"
#include "vlexer.h"

namespace vds {
  class vjson_object;
  class vjson_value;
  class vjson_array;
  
  class vjson_file_parser
  {
  public:
    
  protected:
    vjson_file_parser(vlexer & lexer);

    vlexer & lexer_;
    vlex next_lex_;

    void next(bool throwIfEof = true);

    std::unique_ptr<vjson_object> parse_json_object();
    std::unique_ptr<vjson_value> parse_json_value();
    std::unique_ptr<vjson_array> parse_json_array();
    
    void require(const std::string & value, lex_type type = LT_OPERATOR, vlex * catched = nullptr);
    bool optional(const std::string & value, lex_type type = LT_OPERATOR, vlex * catched = nullptr);
    void unexpected();
  };
}

#endif // __VJSON_FILE_PARSER_H_
