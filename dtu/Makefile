CC = arm-linux-gnueabihf-gcc
CFLAGS = -c
LDFLAGS  = -lpthread

DTU = suyi_Dtu
EXE = lteDtu
OBJ = suyi_dtu.o tcpServer.o serial.o
SRC = suyi_dtu.c
SRC += tcpServer.c
SRC += serial.c

EXE:$(OBJ)
	$(CC) $(LDFLAGS) -o $(DTU) $^

OBJ:$(SRC)
	$(CC) $(CFLAGS) -o $(OBJ) $^

clean:
	-rm *.o $(DTU)
