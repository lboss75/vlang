#ifndef __V2CPP_CPP_GENERATOR_H_
#define __V2CPP_CPP_GENERATOR_H_

namespace vds {
  class cpp_generator
  {
  public:
    cpp_generator();
    ~cpp_generator();

    void generate(vrt_package * package);

  private:
    std::ofstream public_header_;
    std::ofstream private_header_;
    std::ofstream implementation_;

    static std::list<std::string> parse_name(const std::string & name);
    static std::string type_name(const vrt_type * t);

    void generate_property(const vrt_property * prop);
    void generate_method(const vrt_callable * m);
  };
}

#endif//__V2CPP_CPP_GENERATOR_H_
