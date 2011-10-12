EXTRA_CFLAGS = -g -O0
KVERSION = $(shell uname -r)

M=$(PWD)

obj-m := work_queue.o
work_queue-y += proc_info.o

all:
	@make -C /lib/modules/$(shell uname -r)/build M=$(M)

clean:
	@rm -f *.o *.ko *~ modules.order Module.symvers *.mod.c; \
	rm -fr \.[a-z]*