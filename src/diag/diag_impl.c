#include <diag/diag.h>
//#include <igris/util/stub.h>

#include <stdlib.h>

struct diag_ops* current_diag_ops = NULL;
void* current_diag_priv = NULL;

void diag_setup(struct diag_ops* ndiag, void* priv)
{
	current_diag_ops = ndiag;
	current_diag_priv = priv;
}

int diag_putc(char c)
{
	if (current_diag_ops == NULL) return 0;
	return current_diag_ops->putc(current_diag_priv, c);
}

int diag_getc()
{
	if (current_diag_ops == NULL) return 0;
	return current_diag_ops->getc(current_diag_priv);
}

int diag_write(const char* buf, int len)
{
	if (current_diag_ops == NULL) return 0;
	return current_diag_ops->write(current_diag_priv, buf, len);
}

int diag_read(char* buf, int len)
{
	if (current_diag_ops == NULL) return 0;
	return current_diag_ops->read(current_diag_priv, buf, len);
}

int diag_write_common(void* priv, const char* buf, int len)
{
	if (current_diag_ops == NULL) return 0;
	const char *ptr = buf;
	for (; len; len--) diag_putc((int)*ptr++);
	return 0;
}

int diag_read_common(void* priv, char* buf, int len)
{
	if (current_diag_ops == NULL) return 0;
	char *ptr = buf;
	for (; len; len--) *ptr++ = diag_getc();
	return 0;
}