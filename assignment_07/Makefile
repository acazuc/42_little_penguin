obj-m += main.o
main-objs := main.o file_id.o file_jiffies.o file_foo.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
