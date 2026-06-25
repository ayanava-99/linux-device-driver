obj-m+= mydriver.o


NEWDIR :=/lib/modules/$(shell uname -r)/build

all:
	make -C $(NEWDIR) M=$(PWD) modules

clean:
	make -C $(NEWDIR) M=$(PWD) clean
