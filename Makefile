obj-m += qns1Driver.o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	$(CC) qns1Driver.c -o qns1Driver
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
	rm qns1Driver