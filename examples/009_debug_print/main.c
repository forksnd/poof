#include <stdio.h>

#define poof(...)

//
// Buckle up.
//
// Here we implement a poof function that generates code to dump any datatype
// in our program to the console.
//
// As you can see by the number of NOTE and TODO comments, this type of code is
// still not completely stable.  That said, it's pretty flippin' sweet what
// `poof` can do despite the current level of jank-osity.
//



// Hand-written code that thunks DebugPrint through to printf() for primitives
#include "print_primitives.h"




struct nested_struct
{
  int NestedInt;
};

struct foo_struct
{
  struct nested_struct *FooNested;
};

struct bar_struct
{
  int BarInt;
};

struct baz_struct
{
  int BazInt;
};


//
// NOTE(Jesse): Automatically generating generic code for d_unions is a WIP.
//
// We do a small amount of manual work later on that `poof` should be smart
// enough to do for us, but I haven't quite gotten that far yet.
//
// see @manually_generate_d_union_switch
//
poof(
  d_union my_discriminated_union
  {
    foo_struct
    bar_struct
    baz_struct

    flazz       enum_only
  }
)
#include <generated/d_union_my_discriminated_union.h>


//
// TODO(Jesse): Document this more thoroughly
//
// for_datatypes() takes a named_list of datatypes to iterate over, or the
// symbolic constant `all` if you'd like it to iterate over every type in your
// program.
//
// You can exclude types using the .exclude() operator and passing one or multiple named_lists
//



//
// First we declare all the functions we're about to generate
//
// Notice we can surround TStruct.type with parens () to weld it into another
// identifier.  This will generate function declarations of the form:
//
// void DebugPrint_the_struct_type(struct the_struct_type *Struct);
//
// and
//
// void DebugPrint_the_enum_type(enum the_enum_type Enum);
//
poof(
  for_datatypes(all)
    func (TStruct)
    {
      void DebugPrint_(TStruct.name)(struct TStruct.name *Struct);
    }
    func (TEnum)
    {
      void DebugPrint_(TEnum.name)(enum TEnum.name Enum);
    }
)
#include <generated/for_datatypes_all_Hm28xSLw.h>

//
// Time to party.  Generate implementations for DebugPrint for every datatype
// in our program.
//
poof(
  for_datatypes(all) // TODO(Jesse): Probably wrap the callbacks in {} .. ?  The syntax here looks pretty weird 

    func (TStruct)
    {
      void DebugPrint_(TStruct.name)(struct TStruct.name *Struct)
      {
        DebugPrint_str("{ ");

        TStruct.map_members(StructMember)
        {
          StructMember.is_union?
          {
            // Print d_union members.  See comment at @manually_generate_d_union_switch
            switch (Struct->Type)
            {
              StructMember.map_members (TUnionMember)
              {
                 case type_(TUnionMember.name):
                 {
                   DebugPrint_str("\n    TUnionMember.name = ");
                   DebugPrint_(TUnionMember.name)(&Struct->(TUnionMember.name) );
                 } break;
              }

              default: { /* Invalid type passed to DebugPrint .. handling the error might be nice */ } break;
            }
          }
          // Print regular members
          {
            DebugPrint_str("StructMember.type StructMember.name = ");
            DebugPrint_(StructMember.type)( Struct->(StructMember.name) );
            DebugPrint_str(";");
          }
        }
        DebugPrint_str(" }");
      }
    }

    func (TEnum)
    {

      // Print an enum value as a string.
      //
      // In 'real' code this might be a thunk to ToString(enum) that's already generated.
      //
      void DebugPrint_(TEnum.name)( enum TEnum.name EnumValue)
      {
        switch (EnumValue)
        {
          TEnum.map_values (TEnumValue)
          {
            case TEnumValue.name:
            {
              DebugPrint_str("TEnumValue.name");
            } break;
          }
        }
      }

    }
)
#include <generated/for_datatypes_all_F8GpeIx4.h>


//
// Now let's put our handiwork to good use!!
//
int main()
{
  struct bar_struct BarInstance =
  {
    .BarInt = 42,
  };

  // TODO(Jesse): Generate constructor functions for this operation
  //
  struct my_discriminated_union BarDUnionInstance =
  {
    .Type = type_bar_struct,
    .bar_struct = BarInstance,
  };

  DebugPrint_str("Bar Instance : \n  ");
  DebugPrint_bar_struct(&BarInstance); // < Meta generated function
  DebugPrint_str("\n\n");

  DebugPrint_str("Bar DUnion Instance : \n  ");
  DebugPrint_my_discriminated_union(&BarDUnionInstance); // < Meta generated function
  DebugPrint_str("\n\n");


  struct nested_struct NestedInstance =
  {
    .NestedInt = 420,
  };

  struct foo_struct FooInstance =
  {
    .FooNested = &NestedInstance,
  };

  // TODO(Jesse): Generate constructor functions for this operation
  //
  struct my_discriminated_union FooDUnionInstance =
  {
    .Type = type_foo_struct,
    .foo_struct = FooInstance,
  };

  DebugPrint_str("Nested Instance : \n  ");
  DebugPrint_nested_struct(&NestedInstance); // < Meta generated function
  DebugPrint_str("\n\n");

  DebugPrint_str("Foo Instance : \n  ");
  DebugPrint_foo_struct(&FooInstance); // < Meta generated function
  DebugPrint_str("\n\n");

  DebugPrint_str("Foo DUnion Instance : \n  ");
  DebugPrint_my_discriminated_union(&FooDUnionInstance); // < Meta generated function
  DebugPrint_str("\n");

}




//
// NOTES ON IMPLEMENTATION
//
// This example is meant to illustrate how you can implement generic code using `poof`.
//
// It is *not* meant to represent the canonical implementation of DebugPrint,
// in `poof`.  In fact, it has several glaring deficiencies.  Notably, it fails
// spectacularly when you declare a struct with a union member that doesn't
// strictly comply with the structure of d_unions.  ie:
//

#if 0
struct this_completely_breaks_our_DebugPrint_code
{
  int foo;

  union
  {
    int bar;
  };
}
#endif

//
// For an implementation of DebugPrint that handles arbitrary code, see the
// implementation in [bonsai_stdlib](https://github.com/scallyw4g/bonsai_stdlib/blob/master/headers/debug_print.h)
//
// The bonsai_stdlib code relies on a C++ compiler, so we don't have to do all
// the gymnastics to generate functions with unique names, which makes
// everything quite a bit more readable.
//


