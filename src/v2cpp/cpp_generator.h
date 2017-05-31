#ifndef __V2CPP_CPP_GENERATOR_H_
#define __V2CPP_CPP_GENERATOR_H_

#include <fstream>

#include "vsyntax.h"


namespace vds {
  class cpp_generator
  {
  public:
    cpp_generator();
    ~cpp_generator();

    void generate(vsyntax & root);

  private:
    std::ofstream public_header_;
    std::ofstream private_header_;
    std::ofstream implementation_;

    static std::list<std::string> parse_name(const std::string & name);

  };
}

#endif//__V2CPP_CPP_GENERATOR_H_
