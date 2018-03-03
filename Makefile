APP = main


obj-m:= readfile.o

all:
	$(CC) -Wall -o $(APP) test.c -lm 

clean:
	rm -f *.o
	rm -f $(APP) 
