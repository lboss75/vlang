#include "stdafx.h"
#include "parse_error.h"

vds::parse_error::parse_error(const std::string& filename, int line, int column, const std::string& message)
{
  std::stringstream s;
  s 
  << filename
  << "("
  << line
  << ","
  << column
  << "): "
  << message;
  
  this->message_ = s.str();
}

const char* vds::parse_error::what() const _GLIBCXX_USE_NOEXCEPT
{
    return this->message_.c_str();
}
