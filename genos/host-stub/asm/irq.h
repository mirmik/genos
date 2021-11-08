typedef int irqstate_t;

#define irqs_enable() ((void)0)
#define irqs_disable() ((void)0)
#define irqs_save() (0)
#define irqs_restore(a) ((void)a)
