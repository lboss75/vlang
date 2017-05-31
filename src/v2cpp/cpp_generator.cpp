#include "stdafx.h"
#include "cpp_generator.h"
#include "vfile_syntax.h"

vds::cpp_generator::cpp_generator()
: public_header_("public.h"),
  private_header_("private.h"),
  implementation_("implementation.cpp")
{
}

vds::cpp_generator::~cpp_generator()
{
}

void vds::cpp_generator::generate(vsyntax & root)
{
  this->public_header_ << "#include <memory>\n";
  
  for(auto & file : root.files()){

    this->public_header_ << "//Package " << file->file_path() << "\n";
    this->private_header_ << "//Package " << file->file_path() << "\n";
    this->implementation_ << "//Package " << file->file_path() << "\n";

    for (auto & ns : file->namespaces()) {
      this->public_header_ << "namespace " << ns->name() << "{\n";
      this->private_header_ << "namespace " << ns->name() << "{\n";
      this->implementation_ << "namespace " << ns->name() << "{\n";
      for (auto & cls : ns->classes()) {
        this->public_header_ << "  class " << cls->name() << "\n";
        this->public_header_ << "  {\n";
        this->public_header_ << "  public:\n";
        this->public_header_ << "    " << cls->name() << "(){}\n";
        
        this->private_header_ << "  class _" << cls->name() << "\n";
        this->private_header_ << "  : public std::enable_shared_from_this<_" << cls->name() << ">\n";
        this->private_header_ << "  {\n";
        this->private_header_ << "  public:\n";

        this->implementation_ << cls->name() << "::" << cls->name() << "()\n";
        this->implementation_ << "{\n";
        this->implementation_ << "}\n";
        

        for(auto& prop : cls->properties()){
          this->public_header_ << "    " << prop->result_type()->name() << " get_" << prop->name() << "() const;\n";
          this->public_header_ << "    void set_" << prop->name() << "(" << prop->result_type()->name() << " new_value);\n";

          this->private_header_ << "    " << prop->result_type()->name()
            << " get_" << prop->name() << "() const { return this->" << prop->name() << "_;}\n";
          this->private_header_ << "    void set_" << prop->name() << "(" << prop->result_type()->name() << " new_value) {"
            << " this->" << prop->name() << "_ = new_value; } \n";
          
          this->implementation_ << prop->result_type()->name() << " " << cls->name() << "::get_" << prop->name() << "() const\n";
          this->implementation_ << "{\n";
          this->implementation_ << "  return this->impl_->get_" << prop->name() << ";\n";
          this->implementation_ << "}\n";
        }



        
        this->public_header_ << "  private:\n";
        this->private_header_ << "  private:\n";
        
        this->public_header_ << "    std::shared_ptr<_" << cls->name() << "> impl_;\n";
        
        for(auto& prop : cls->properties()){
          this->private_header_ << "    " << prop->result_type()->name() << " " << prop->name() << "_;\n";
        }

        
        this->public_header_ << "  };\n";
        this->private_header_ << "  };\n";
      }
      this->public_header_ << "}\n";
      this->private_header_ << "}\n";
      this->implementation_ << "}\n";
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

