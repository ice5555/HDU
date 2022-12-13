
#VPATH=src
obj-m := assignment1.o
CPLUS_INCLUDE_PATH=/usr/lib
OBJC_INCLUDE_PATH=/usr/include
all:
	make -I/include -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

