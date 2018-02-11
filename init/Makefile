CC = arm-linux-gnueabihf-gcc
CFLAGS = -c
LDFLAGS  = -lpthread

EXE = lteRouter
OBJ = init.o at.o getip.o
SRC = init.c at.c 
SRC += getip.c

EXE:$(OBJ)
	$(CC) $(LDFLAGS) -o $(initEXE) $^

OBJ:$(SRC)
	$(CC) $(CFLAGS) -o $(OBJ) $^

clean:
	-rm *.o $(initEXE)
