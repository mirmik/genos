#ifndef _CONFIGURE
#define _CONFIGURE

#include <stdint.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

void project_configure();

void tim5_compare_enable();
void tim5_compare_disable();
void tim2_compare_enable();
void tim2_compare_disable();

void one_pulse_generator_start();

void tim10_configure_polarity(int reverse_polarity);
void tim10_configure_duration(int duration);

__END_DECLS

#endif
