#include <hal/board.h>
#include <hal/irq.h>
#include <drivers/gpio/gpio.h>
#include <drivers/gpio/pin.h>

#include <drivers/spi/avr_spi.h>

#include <gxx/debug/dprint.h>
#include <gxx/debug/delay.h>

#include <mvfs/mvfs.h>
#include <mvfs/fstype.h>
#include <mvfs/fsops.h>
#include <mvfs/fs/joke/joke.h>

#include <drivers/cdev/virtual/null.h>
#include <drivers/cdev/virtual/zero.h>
#include <drivers/cdev/virtual/debug.h>

#include <sched/sched.h>
#include <sched/sched.h>
#include <sched/timer.h>

#include <sched/api.h>

#include <gxx/util/iteration_counter.h>

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

//GPIO_PIN(dataflash_pin, GPIOB, 6);

int i;

void led_blink_timer(void* arg, struct ktimer * tim) {
	gpio_toggle(GPIOB, 1<<7);

	ktimer_swift(tim);
	ktimer_plan(tim);
}

void * aproc(void * arg, int * state) {
	//open("/dev/debug");

	//struct file * filp;
	/*int sts = vfs_open("/dev/debug", 0, &filp);
		if (sts) 
			dprf("error on open %s", strerror(sts));
	
	vfs_write(filp, "HelloWorld\r\n", 12);*/

	int fd = open("/dev/debug", O_WRONLY|O_CREAT);
		if (fd < 0) 
			perror("open");

	int fd2 = dup2(fd, 1);

	int ret = write(fd2, "HelloWorld\r\n", 12);
		if (ret < 0) 
			perror("write");

	printf("mirmik\r\n");
	perror("mirmik");

	schedee_exit();
	return 0;
}

int main() {
	int sts;

	__debug_delay_multiplier = 100;

	//char buf[128] = "\0\x29";

	board_init();
	schedee_manager_init();

	gpio_settings(GPIOB, 1<<7, GPIO_MODE_OUTPUT);

	//struct spi_device * spi = get_avr_spi_device();

	//spi_select(spi, &dataflash_pin, 1);
	
	//int sts = spi_exchange(spi, buf, buf, 2, NOSCHED);
	//if (sts) dprf("exchange status: %d", sts);

	vfs_register_fs(&joke_fstype);
	vfs_mount_first("joke", 0, NULL);

	vfs_mkdir("/dev");

	
	sts = link_debug_device("/dev");
		if (sts) 
			dprf("error on link %s", strerror(sts));
	
	sts = link_null_device("/dev");
		if (sts) 
			dprf("error on link %s", strerror(sts));
	
	sts = link_zero_device("/dev");
		if (sts) 
			dprf("error on link %s", strerror(sts));

	vfs_debug_tree(NULL);

	ktimer_create_for(led_blink_timer, NULL, 1000);

	schedee_run(create_autom_schedee(aproc, NULL));

	irqs_enable();
	__schedule__();
}

void __schedule__() {
	while(1) {
		timer_manager();
		schedee_manager();
	}
}