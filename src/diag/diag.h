#ifndef GENOS_DIAG_H
#define GENOS_DIAG_H

//Diag является отладочным интерфейсом ввода-вывода для таких систем, как debug_print, printk.
//Указанные системы используют функции, смонтированные в cdiag (current diag)
//Diag не регламинтирует, используется ли он для вывода в виде потока байт, 
//консольного форматированного вывода,
//или вывода в файл. То есть cdiag является просто точкой монтирования функций ввода-вывода,
//используемых отладочным вводом-выводом.

#include <sys/cdefs.h>

__BEGIN_DECLS

struct diag_ops {
	int (*putc)(void*, char);
	int (*getc)(void*);
	int (*write)(void*, const char*, int);
	int (*read)(void*, char*, int);
};

typedef int(*diag_putc_t)(void*, char);
typedef int(*diag_getc_t)(void*);
typedef int(*diag_write_t)(void*, const char*, int);
typedef int(*diag_read_t)(void*, char*, int);

extern struct diag_ops* current_diag_ops;

void diag_setup(struct diag_ops* ndiag, void* priv);
int diag_putc(char c);
int diag_getc();
int diag_write(const char* buf, int len);
int diag_read(char* buf, int len);

int diag_write_stub(void* priv, const char* buf, int len);
int diag_read_stub(void* priv, char* buf, int len);

__END_DECLS

#endif