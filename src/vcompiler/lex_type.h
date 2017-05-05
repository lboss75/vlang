#ifndef __LEX_TYPE_H_
#define __LEX_TYPE_H_

namespace vds {
  enum lex_type
  {
    LT_EOF,
    LT_KEYWORD,
    LT_NAME,
    LT_OPERATOR,
    LT_STRING,
    LT_NUMBER
  };
}

#endif // LEX_TYPE_H
