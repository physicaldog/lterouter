CC = arm-linux-gnueabihf-gcc
CFLAGS = -c
LDFLAGS  = -lpthread

EXE = lteRouter
OBJ = init.o at.o ipConfig.o
SRC = init.c at.c 
SRC += ipConfig.c

EXE:$(OBJ)
	$(CC) $(LDFLAGS) -o $(initEXE) $^

OBJ:$(SRC)
	$(CC) $(CFLAGS) -o $(OBJ) $^

clean:
	-rm *.o $(initEXE)
