#ifndef __PARSE_ERROR_H_
#define __PARSE_ERROR_H_

#include <exception>
#include <string>

#ifdef _WIN32
#define _GLIBCXX_USE_NOEXCEPT
#endif

namespace vds {
  class parse_error : public std::exception
  {
  public:
    parse_error(
      const std::string & filename,
      int line,
      int column,
      const std::string & message
    );
    
    const char* what() const _GLIBCXX_USE_NOEXCEPT override;
    
  private:
    std::string message_;
  };
}

#endif // __PARSE_ERROR_H_
