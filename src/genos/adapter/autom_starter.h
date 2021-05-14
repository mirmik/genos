#ifndef GENOS_AUTOM_SCHEDEE_STARTER_H
#define GENOS_AUTOM_SCHEDEE_STARTER_H

#include <igris/compiler.h>

typedef void (automfunc_t)(int argc, char ** argv, int * state);

__BEGIN_DECLS

void autom_schedee_emulator(int argc, char ** argv, automfunc_t automfunc);

__END_DECLS

#define AUTOMATE_SCHEDEE_MAIN(func) \
int main(int argc, char ** argv) { autom_schedee_emulator(argc, argv, func); }

#endif