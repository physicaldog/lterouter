CC = arm-linux-gnueabihf-gcc

ifdef DB
CFLAGS = -c -DDEBUG
else
CFLAGS = -c
endif

LDFLAGS  = -lpthread -lubox -luci

EXE = lteRouter
OBJ = module_common.o system_common.o
SRC = module_common.c system_common.c 

all:$(OBJ)
	$(CC) $(LDFLAGS) -o module_test $^

OBJ:$(SRC)
	$(CC) $(CFLAGS) -o $(OBJ) $^


clean:
	-rm *.o $(initEXE) button
