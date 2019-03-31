#include <diag/diag.h>
#include <igris/util/stub.h>

#include <stdlib.h>

struct diag_ops diag_stub = {
	.putc = (diag_putc_t) do_nothing,
	.getc = (diag_getc_t) do_nothing,
	.write = (diag_write_t) do_nothing,
	.read = (diag_read_t) do_nothing,
};

struct diag_ops* current_diag_ops = &diag_stub;
void* current_diag_priv = NULL;

void diag_setup(struct diag_ops* ndiag, void* priv) {
	current_diag_ops = ndiag;
	current_diag_priv = priv;
}

int diag_putc(char c) {
	return current_diag_ops->putc(current_diag_priv, c);
}

int diag_getc() {
	return current_diag_ops->getc(current_diag_priv);
}

int diag_write(const char* buf, int len) {
	return current_diag_ops->write(current_diag_priv, buf, len);
}

int diag_read(char* buf, int len) {
	return current_diag_ops->read(current_diag_priv, buf, len);
}

int diag_write_stub(void* priv, const char* buf, int len) {
	const char *ptr = buf;
	for (;len;len--) diag_putc((int)*ptr++);
	return 0;
}

int diag_read_stub(void* priv, char* buf, int len) {
	char *ptr = buf;
	for (;len;len--) *ptr++ = diag_getc();
	return 0;
}