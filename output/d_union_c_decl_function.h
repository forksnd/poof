enum c_decl_function_type
{
  type_c_decl_function_noop,
  type_c_decl_function_normal,
  type_c_decl_function_operator,
  type_c_decl_function_constructor,
  type_c_decl_function_destructor,
};

struct c_decl_function
{
  c_decl_function_type Type;

  union
  {
  };
};

