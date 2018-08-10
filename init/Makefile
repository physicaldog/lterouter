CC = arm-linux-gnueabihf-gcc

ifdef DB
CFLAGS = -c -DDEBUG
else
CFLAGS = -c
endif

LDFLAGS  = -lpthread

EXE = lteRouter
OBJ = init.o at.o ipConfig.o log.o
SRC = init.c at.c 
SRC += ipConfig.c
SRC += log.c

EXE:$(OBJ)
	$(CC) $(LDFLAGS) -o $(initEXE) $^

OBJ:$(SRC)
	$(CC) $(CFLAGS) -o $(OBJ) $^

clean:
	-rm *.o $(initEXE)
