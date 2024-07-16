#ifndef NOS_UTIL_HORIBLE_CAST_H
#define NOS_UTIL_HORIBLE_CAST_H

/**
  @file

   Черная магия 4-ой ступени. Кастует что угодно во что угодно.

   horrible_cast< >
   This is truly evil. It completely subverts C++'s type system, allowing you
   to cast from any class to any other class. Technically, using a union
   to perform the cast is undefined behaviour (even in C). But we can see if
   it is OK by checking that the union is the same size as each of its members.
   horrible_cast<> should only be used for compiler-specific workarounds.
   Usage is identical to reinterpret_cast<>.

   This union is declared outside the horrible_cast because BCC 5.5.1
   can't inline a function with a nested class, and gives a warning.
*/

namespace nos {
  template <class OutputClass, class InputClass> union horrible_union
  {
      OutputClass out;
      InputClass in;
      horrible_union(){};
  };
  
  template <class OutputClass, class InputClass>
  inline OutputClass horrible_cast(const InputClass input)
  {
      horrible_union<OutputClass, InputClass> u;
  
      // Cause a compile-time error if in, out and u are not the same size.
      // If the compile fails here, it means the compiler has peculiar
      // unions which would prevent the cast from working.
      typedef int ERROR_CantUseHorrible_cast[sizeof(InputClass) == sizeof(u) &&
                                                     sizeof(InputClass) ==
                                                         sizeof(OutputClass)
                                                 ? 1
                                                 : -1];
  
      // warn supress:
      ERROR_CantUseHorrible_cast _;
      (void)_;
  
      u.in = input;
      return u.out;
  }
}

#endif
