#include <ircc/ircc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    size_t size;
    const char *hello = ircc_c_string("another_key", &size);
    int sts = strcmp(hello, "HelloUnderWorld");
    int test_failed = sts != 0 || size != 15;
    if (test_failed)
        printf("Test test failed.\n");
    else
        printf("Test test passed.\n");
    return test_failed;
}