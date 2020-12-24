EXEC = ./bin/self_test
OBJS = self_test.o
SRC  = self_test.c

CC = arm-linux-gnueabihf-gcc
CFLAGS += -O2 -Wall
LDFLAGS += 

all: clean $(EXEC)

$(EXEC):$(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -vf $(EXEC) *.o *~
