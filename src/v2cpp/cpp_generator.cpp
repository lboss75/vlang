#include "stdafx.h"
#include "cpp_generator.h"

vds::cpp_generator::cpp_generator()
: public_header_("public.h"),
  private_header_("private.h"),
  implementation_("implementation.cpp")
{
}

vds::cpp_generator::~cpp_generator()
{
}

void vds::cpp_generator::generate(vrt_package * package)
{
  this->public_header_ << "//Package " << package->name() << "\n";
  this->private_header_ << "//Package " << package->name() << "\n";
  this->implementation_ << "//Package " << package->name() << "\n";

  for (auto & class_info : package->classes()) {
    auto names = parse_name(class_info->name());

    for (auto p = names.begin(); p != names.end(); ) {
      auto current = *p;
      ++p;

      if (p == names.end()) {
        this->public_header_ << "class " << current << "\n";
        this->public_header_ << "{\n";
        this->public_header_ << "public:\n";
        this->public_header_ << "  " << current << "(){}\n";


        this->private_header_ << "class _" << current << "\n";
        this->private_header_ << "{\n";

        this->implementation_ << current << "::" << current << "()\n";
        this->implementation_ << "{\n";
        this->implementation_ << "}\n";
      }
      else {
        this->public_header_ << "namespace " << current << "\n";
        this->public_header_ << "{\n";


        this->private_header_ << "namespace " << current << "\n";
        this->private_header_ << "{\n";

        this->implementation_ << current << "::";
      }
    }

    for (auto & prop : class_info->properties()) {
      this->generate_property(prop.get());
    }

    for (auto & m : class_info->methods()) {
      this->generate_method(m.get());
    }

    for (auto p : names) {
      this->public_header_ << "};\n";
      this->private_header_ << "};\n";
    }
  }
}

std::list<std::string> vds::cpp_generator::parse_name(const std::string & name)
{
  std::list<std::string> result;

  std::size_t start = 0;
  for (;;) {
    auto pos = name.find('.', start);
    if (std::string::npos == pos) {
      result.push_back(name.substr(start));
      break;
    }

    result.push_back(name.substr(start, pos - start));
    start = pos + 1;
  }

  return result;
}

std::string vds::cpp_generator::type_name(const vrt_type * t)
{
  auto names = parse_name(t->name());
  bool is_first = true;
  std::string result;
  for (auto n : names) {
    if (is_first) {
      is_first = false;
    }
    else {
      result += "::";
    }

    result += n;
  }

  return result;

}

void vds::cpp_generator::generate_property(const vrt_property * prop)
{
  this->public_header_ << "private: " << type_name(prop->get_property_type()) << " " << prop->name() << "_;\n";
}

void vds::cpp_generator::generate_method(const vrt_callable * m)
{
  this->public_header_ << "public: ";

  auto result_type = m->get_result_type();

  if (nullptr == result_type) {
    this->public_header_ << "void ";
  }
  else {
    this->public_header_ << "std::shared_ptr<" << type_name(result_type) << "> ";
  }

  this->public_header_ << m->name();

  this->public_header_ << "(const vds::service_provider & sp";

  for (auto & p : m->parameters()) {
    this->public_header_ << ", " << type_name(p->type()) << " " << p->name();
  }

  this->public_header_ << ");\n";
}
