#include <diag/diag.h>

void diag_setup(struct diag_ops *ndiag, void *priv){};

int diag_putc(char c){};
int diag_getc(){};
int diag_write(const char *buf, int len){};
int diag_read(char *buf, int len){};

int diag_write_stub(const char *buf, int len){};
int diag_read_stub(char *buf, int len){};