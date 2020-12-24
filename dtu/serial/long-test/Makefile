EXEC = ./bin/long_test
OBJS = long_test.o
SRC  = long_test.c

CC = arm-linux-gnueabihf-gcc   
CFLAGS += -O2 -Wall
LDFLAGS += 

all: clean $(EXEC)

$(EXEC):$(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) -pthread

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@ -pthread

clean:
	@rm -vf $(EXEC) *.o *~
