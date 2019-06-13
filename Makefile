obj-m := booga.o
TARGET_MODULE:=booga

all:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean 

test:test-booga.o
	gcc -o test-booga test-booga.o

test-booga.o:test-booga.c
	gcc -c test-booga.c

load:
	insmod ./$(TARGET_MODULE).ko

unload:
	rmmod ./$(TARGET_MODULE).ko
