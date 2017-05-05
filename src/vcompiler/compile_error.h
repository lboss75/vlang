#ifndef COMPILE_ERROR_H
#define COMPILE_ERROR_H

#include <exception>

#ifdef _WIN32
#define _GLIBCXX_USE_NOEXCEPT
#endif

namespace vds {
  class compile_error : public std::exception
  {
  public:
    compile_error(
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

#endif // COMPILE_ERROR_H
