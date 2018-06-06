#ifndef CORE_TYPES_H
#define CORE_TYPES_H

class v_int
{
};

class v_string
{
public:
  v_string * __add(v_string * value);

  static v_string * create(const char * value);
};

class v_char
{
};



#endif // CORE_TYPES_H
