TOOLDIR = /opt/iot-devkit/1.7.2/sysroots/x86_64-pokysdk-linux/usr/bin/i586-poky-linux

ifeq ($(TEST_TARGET), Galileo2)
	CC=$(TOOLDIR)/i586-poky-linux-gcc
else
	CC=gcc
endif


APP = main


obj-m:= readfile.o

all:
	$(CC) -Wall -o $(APP) readfile.c -pthread -lm 

clean:
	rm -f *.o
	rm -f $(APP) 
