#include "stdafx.h"
#include "cpp_generator.h"
#include "vfile_syntax.h"

#ifdef _WIN32
#define test_out "D:\\projects\\vlang.test\\vlang.test\\"
#else
#define test_out "/home/vadim/projects/vlang/tests/vcompiled/"
#endif

vds::cpp_generator::cpp_generator()
: public_header_(test_out "public.h"),
  private_header_(test_out "private.h"),
  implementation_(test_out "implementation.cpp")
{
}

vds::cpp_generator::~cpp_generator()
{
}

void vds::cpp_generator::generate(vsyntax & root)
{
  this->public_header_ << "#ifndef __PUBLIC_H_\n";
  this->public_header_ << "#define __PUBLIC_H_\n";

  this->private_header_ << "#ifndef __PRIVATE_H_\n";
  this->private_header_ << "#define __PRIVATE_H_\n";

  this->public_header_ << "#include \"vruntime.h\"\n";

  this->implementation_ << "#include \"public.h\"\n";
  this->implementation_ << "#include \"private.h\"\n";

  
  for(auto & file : root.files()){

    this->public_header_ << "//Package " << file->file_path() << "\n";
    this->private_header_ << "//Package " << file->file_path() << "\n";
    this->implementation_ << "//Package " << file->file_path() << "\n";
    for (auto & ns : file->namespaces()) {
      for (auto &cls : ns->classes()) {
        this->public_header_ << "  struct " << this->struct_name(ns, cls) << ";//Forward\n";
        this->public_header_ << "  "
                             << this->struct_name(ns, cls)
                             << " * "
                             << this->construct_name(ns, cls)
                             << "();\n";
      }
    }

    for (auto & ns : file->namespaces()) {
      for (auto &cls : ns->classes()) {
        for(auto& prop : cls->properties()) {
          this->public_header_ << "    "
                               << prop->result_type()->name()
                               << " * "
                               << this->struct_name(ns, cls)
                               << "_get_"
                               << prop->name()
                               << "("
                               << this->struct_name(ns, cls)
                               << " * pthis);\n";

          this->public_header_ << "    void "
                               << this->struct_name(ns, cls)
                               << "_set_"
                               << prop->name()
                               << "(const "
                               << this->struct_name(ns, cls)
                               << " * pthis, "
                               << prop->result_type()->name()
                               << " new_value);\n";
        }
      }
    }

    for (auto & ns : file->namespaces()) {
      for (auto & cls : ns->classes()) {
        this->public_header_ << "  struct " << this->struct_name(ns, cls) << "{\n";
        for(auto& prop : cls->properties()){
          this->public_header_ << "    " << prop->result_type()->name() << " " << prop->name() << "_;\n";
        }
        this->public_header_ << "  };\n";

        for(auto& prop : cls->properties()){
          this->implementation_
              << "    "
              << prop->result_type()->name()
              << " * "
              << this->struct_name(ns, cls)
              << "_get_"
              << prop->name()
              << "(const "
              << this->struct_name(ns, cls)
              << " * pthis){\n"
              << "          return pthis->" << prop->name() << "_;}\n";

          this->implementation_
              << "    void "
              << this->struct_name(ns, cls)
              << "_set_"
              << prop->name()
              << "(const "
              << this->struct_name(ns, cls)
              << " * pthis, "
              << prop->result_type()->name()
              << " new_value) {\n"
              << " pthis->" << prop->name() << "_ = new_value; } \n";
        }

        for (auto& method : cls->methods()) {
          this->generate(cls.get(), method.get());
        }
      }
    }
  }

  this->public_header_ << "#endif//__PUBLIC_H_\n";
  this->private_header_ << "#endif//__PRIVATE_H_\n";
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

void vds::cpp_generator::generate(const vclass * cls, const vmethod * m)
{
  if (m->is_static()) {
    this->public_header_ << "static ";
    this->private_header_ << "static ";
  }

  bool is_counstruct = (cls->name() == m->name());

  if (is_counstruct) {
    this->public_header_ << "void __construct(";
    this->private_header_ << "void __construct(";
    if (!m->is_external()) {
      this->implementation_ << "void " << cls->name() << "::__construct(";
    }
  }
  else {
    this->public_header_ << this->print_type(m->result_type()) << " " << m->name() << "(";
    this->private_header_ << this->print_type(m->result_type()) << " " << m->name() << "(";
    if (!m->is_external()) {
      this->implementation_ << this->print_type(m->result_type()) << " " << cls->name() << "::" << m->name() << "(";
    }
  }
  bool is_first = true;
  for (auto & arg : m->parameters()) {
    if (is_first) {
      is_first = false;
    }
    else {
      this->public_header_ << ", ";
      this->private_header_ << ", ";
      if (!m->is_external()) {
        this->implementation_ << ", ";
      }
    }

    this->public_header_ << this->print_type(arg->type()) << " " << arg->name();
    this->private_header_ << this->print_type(arg->type()) << " " << arg->name();
    if (!m->is_external()) {
      this->implementation_ << this->print_type(arg->type()) << " " << arg->name();
    }
  }

  this->public_header_ << ");\n";
  this->private_header_ << ");\n";
  if (!m->is_external()) {
    this->implementation_ << ")\n{\n";

    this->generate(m->body());

    this->implementation_ << "}\n";
  }
}

std::string vds::cpp_generator::print_type(const vtype * t)
{
  if (nullptr == t) {
    return "void";
  }
  return t->name();
}

void vds::cpp_generator::generate(const vblock_statement * body)
{
  for (auto & st : body->statements()) {
    this->generate(st.get());
  }
}

void vds::cpp_generator::generate(const vstatement * st)
{
  auto exp = dynamic_cast<const vexpression_statement *>(st);
  if (nullptr != exp) {
    this->generate(exp->expression());
    this->implementation_ << ";\n";
    return;
  }

  auto vst = dynamic_cast<const var_statement *>(st);
  if (nullptr != vst) {
    this->implementation_ << "auto " << vst->name() << "=";
    this->generate(vst->init_value());
    this->implementation_ << ";\n";
    return;
  }

  auto ifst = dynamic_cast<const vif_statement *>(st);
  if (nullptr != ifst) {
    this->implementation_ << "if(";
    this->generate(ifst->condition());
    this->implementation_ << "){\n";
    this->generate(ifst->body());
    this->implementation_ << "}\n";

    if (nullptr != ifst->else_body()) {
      this->implementation_ << "else {\n";
      this->generate(ifst->else_body());
      this->implementation_ << "}\n";
    }
    return;
  }

  auto return_st = dynamic_cast<const vreturn_statement *>(st);
  if (nullptr != return_st) {
    this->implementation_ << "return ";
    this->generate(return_st->body());
    this->implementation_ << ";";
    return;
  }

  auto forst = dynamic_cast<const vfor_statement *>(st);
  if (nullptr != forst) {
    this->implementation_ << "for(";
    this->generate(forst->init());
    this->generate(forst->condition());
    this->generate(forst->step());
    this->implementation_ << "){\n";
    this->generate(forst->body());
    this->implementation_ << "}\n";
    return;
  }

  auto whilest = dynamic_cast<const vwhile_statement *>(st);
  if (nullptr != whilest) {
    this->implementation_ << "while(";
    this->generate(whilest->condition());
    this->implementation_ << "){\n";
    this->generate(whilest->body());
    this->implementation_ << "}\n";
    return;
  }

  throw std::runtime_error("Unexpected statement");
}

void vds::cpp_generator::generate(const vexpression * exp)
{
  if (nullptr == exp) {
    return;
  }

  auto bexp = dynamic_cast<const vbinary_exptession *>(exp);
  if (nullptr != bexp) {
    if ("=" == bexp->op()) {
      this->generate_left(bexp->left(), [this, bexp]() {
        this->generate(bexp->right());
      });
      return;
    }
    if ("||" == bexp->op()) {
      this->generate(bexp->left());
      this->implementation_ << ".__logical_or(";
      this->generate(bexp->right());
      this->implementation_ << ")";
      return;
    }
    if ("==" == bexp->op()) {
      this->generate(bexp->left());
      this->implementation_ << ".__is_equal(";
      this->generate(bexp->right());
      this->implementation_ << ")";
      return;
    }
    if ("+=" == bexp->op()) {
      this->generate_left(bexp->left(), [this, bexp]() {
        this->generate(bexp->left());
        this->implementation_ << ".__add(";
        this->generate(bexp->right());
        this->implementation_ << ")";
      });
      return;
    }
  }

  auto pexp = dynamic_cast<const vproperty_expression *>(exp);
  if (nullptr != pexp) {
    this->generate(pexp->object());
    this->implementation_ << ".get_" << pexp->name() << "()";
    return;
  }

  auto nexp = dynamic_cast<const vname_expression *>(exp);
  if (nullptr != nexp) {
    if ("this" == nexp->name()) {
      this->implementation_ << "(*this)";
    }
    else {
      this->implementation_ << nexp->name();
    }

    return;
  }

  auto prexp = dynamic_cast<const vprefix_exptession *>(exp);
  if (nullptr != prexp) {
    if ("!" == prexp->op()) {
      this->generate(prexp->right());
      this->implementation_ << ".__not()";
      return;
    }
  }

  auto strexp = dynamic_cast<const vstring_expression *>(exp);
  if (nullptr != strexp) {
    this->implementation_ << "String::__create(\"" << strexp->value() << "\")";
    return;
  }

  auto newexp = dynamic_cast<const vnew_object_expression *>(exp);
  if (nullptr != newexp) {
    this->implementation_ << newexp->type()->name() << "::__create(";
    bool is_first = true;
    for (auto & arg : newexp->arguments()) {
      if (is_first) {
        is_first = false;
      }
      else {
        this->implementation_ << ", ";
      }

      this->generate(arg->value());
    }
    this->implementation_ << ")";
    return;
  }
  
  auto invoke_exp = dynamic_cast<const vinvoke_expression *>(exp);
  if(nullptr != invoke_exp){
    this->generate(invoke_exp->object());
    this->implementation_ << "->" << invoke_exp->name() << "(";
    
    bool is_first = true;
    for (auto & arg : invoke_exp->arguments()) {
      if (is_first) {
        is_first = false;
      }
      else {
        this->implementation_ << ", ";
      }

      this->generate(arg->value());
    }
    this->implementation_ << ")";
    return;
  }
  
  std::string type = typeid(exp).name();

  throw std::runtime_error("Unexpected expression " + type);
}

void vds::cpp_generator::generate_left(const vexpression * exp, const std::function<void(void)>& right_callback)
{
  auto pexp = dynamic_cast<const vproperty_expression *>(exp);
  if (nullptr != pexp) {
    this->generate(pexp->object());
    this->implementation_ << ".set_" << pexp->name() << "(";
    right_callback();
    this->implementation_ << ")";
    return;
  }

  auto nexp = dynamic_cast<const vname_expression *>(exp);
  if (nullptr != nexp) {
    this->implementation_ << nexp->name();
    return;
  }

  throw std::runtime_error("Unexpected expression");
}

std::string vds::cpp_generator::struct_name(const std::unique_ptr<vds::vnamespace> &ns, const std::unique_ptr<vds::vclass> &cls) {
  return ns->name() + "_" + cls->name();
}

std::string vds::cpp_generator::construct_name(const std::unique_ptr<vds::vnamespace> &ns,
                                               const std::unique_ptr<vds::vclass> &cls) {
  return ns->name() + "_" + cls->name() + "_construct";
}
