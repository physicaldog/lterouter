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
	$(CC) $(CFLAGS) -o button.o button.c
	$(CC) $(LDFLAGS) -o button log.o button.o

OBJ:$(SRC)
	$(CC) $(CFLAGS) -o $(OBJ) $^


clean:
	-rm *.o $(initEXE) button
