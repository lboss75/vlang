#ifndef __V2CPP_CPP_GENERATOR_H_
#define __V2CPP_CPP_GENERATOR_H_

#include <fstream>
#include <functional>

#include "vsyntax.h"


namespace vds {
  class vclass;
  class vmethod;
  class vtype;
  class vstatement;
  class vblock_statement;
  class vexpression;

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

    void generate(const vclass * clss, const vmethod * m);

    std::string print_type(const vtype * t);

    void generate(const vblock_statement * body);

    void generate(const vstatement * st);
    void generate(const vexpression * exp);
    void generate_left(const vexpression * exp, const std::function<void(void)> & right_callback);

  };
}

#endif//__V2CPP_CPP_GENERATOR_H_
