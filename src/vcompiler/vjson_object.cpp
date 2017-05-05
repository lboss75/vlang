#include "stdafx.h"
#include "vjson_object.h"

vds::vjson_value::vjson_value(int line, int column)
: line_(line), column_(column)
{
}

vds::vjson_value::~vjson_value()
{
}

vds::vjson_primitive::vjson_primitive(
    int line,
    int column,
    const std::string & value)
: vjson_value(line, column), value_(value)
{
}


vds::vjson_object::vjson_object(
  int line, int column
) : vjson_value(line, column)
{
}

void vds::vjson_object::visit(const std::function<void(const vjson_property&)>& visitor) const
{
  for (const vjson_property & property : this->properties_) {
    visitor(property);
  }
}

vds::vjson_array::vjson_array(int line, int column)
: vjson_value(line, column)
{

}
