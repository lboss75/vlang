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
  class vnamespace;

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

    std::string method_name(const std::unique_ptr<vnamespace>& ns, const std::unique_ptr<vclass>& cls, const vmethod* vmethod);
    void generate(const std::unique_ptr<vds::vnamespace> &ns, const std::unique_ptr<vclass> &cls, const vmethod * m);

    std::string print_type(const vtype * t);

    void generate(const vblock_statement * body);

    void generate(const vstatement * st);
    void generate(const vexpression * exp);
    void generate_left(const vexpression * exp, const std::function<void(void)> & right_callback);

    std::string struct_name(const std::unique_ptr<vnamespace> &ns, const std::unique_ptr<vclass> &cls);
    std::string struct_name(const std::unique_ptr<vds::vnamespace> &ns, const vtype * t);
    std::string construct_name(const std::unique_ptr<vnamespace> &ns, const std::unique_ptr<vclass> &cls);
  };
}

#endif//__V2CPP_CPP_GENERATOR_H_
