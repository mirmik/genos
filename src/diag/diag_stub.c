#include <gxx/diag/diag.h>
#include <gxx/util/stub.h>

struct diag_ops diag_stub = {
	(diag_putc_t)  do_nothing,
	(diag_getc_t)  do_nothing,
	(diag_write_t) do_nothing,
	(diag_read_t)  do_nothing,
};

struct diag_ops* current_diag = &diag_stub;;

void diag_setup(struct diag_ops* ndiag){};

int diag_putc(char c){};
int diag_getc(){};
int diag_write(const char* buf, int len){};
int diag_read(char* buf, int len){};

int diag_write_stub(const char* buf, int len){};
int diag_read_stub(char* buf, int len){};