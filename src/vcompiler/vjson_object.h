#ifndef __VJSON_OBJECT_H_
#define __VJSON_OBJECT_H_

namespace vds {
  
  class vjson_value
  {
  public:
    vjson_value(int line, int column);
    virtual ~vjson_value();

    int line() const {
      return this->line_;
    }
    
    int column() const {
      return this->column_;
    }
    
  private:
    int line_;
    int column_;
  };
  
  class vjson_primitive : public vjson_value
  {
  public:
    vjson_primitive(
      int line, int column,
      const std::string & value);
    
    const std::string & value() const {
      return this->value_;
    }
    
  private:
    std::string value_;
  };
  
  struct vjson_property
  {
    int line;
    int column;
    std::string name;
    std::unique_ptr<vjson_value> value;
  };
  
  class vjson_object : public vjson_value
  {
  public:
    vjson_object(int line, int column);

    void visit(const std::function<void(const vjson_property &)> & visitor) const;
    
  private:
    friend class vjson_file_parser;
    std::list<vjson_property> properties_;
  };
  
  class vjson_array : public vjson_value
  {
  public:
    vjson_array(
      int line,
      int column);

    size_t size() const {
      return this->items_.size();
    }
    
    const vjson_value * get(int index) const {
      return this->items_[index].get();
    }
    
  private:
    friend class vjson_file_parser;
    std::vector<std::unique_ptr<vjson_value>> items_;
  };
  
}

#endif // __VJSON_OBJECT_H_
