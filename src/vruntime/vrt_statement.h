#ifndef __VRT_STATEMENT_H_
#define __VRT_STATEMENT_H_

namespace vds {
  class vrt_source_file;
  class vrt_type;
  
  class vrt_statement
  {
  public:
    vrt_statement(
      const vrt_source_file * file,
      int line,
      int column
    );

    virtual ~vrt_statement();

    const vrt_source_file * file() const
    {
      return this->file_;
    }

    int line() const
    {
      return this->line_;
    }

    int column() const
    {
      return this->column_;
    }

  private:
    const vrt_source_file * file_;
    int line_;
    int column_;
  };
 
  class vrt_pop_statement : public vrt_statement
  {
  public:
    vrt_pop_statement(
      const vrt_source_file * file,
      int line,
      int column);
    
  };
  
  class vrt_assign_var_statement : public vrt_statement
  {
  public:
    vrt_assign_var_statement(
      const vrt_source_file * file,
      int line,
      int column,
      size_t variable_index);
    
  private:
    size_t variable_index_;
  };

  class vrt_return_statement : public vrt_statement
  {
  public:
    vrt_return_statement(
      const vrt_source_file * file,
      int line,
      int column,
      const vrt_type * type);

  private:
    const vrt_type * type_;
  };
}

#endif // !__VRT_STATEMENT_H_
