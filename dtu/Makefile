CC = arm-linux-gnueabihf-gcc
CFLAGS = -c
LDFLAGS  = -lpthread

EXE = lteDtu
OBJ = suyi_dtu.o tcpServer.o tcpClient.o udpServer.o udpClient.o serial.o
SRC = suyi_dtu.c
SRC += tcpServer.c
SRC += tcpClient.c
SRC += udpServer.c
SRC += udpClient.c
SRC += serial.c

EXE:$(OBJ)
	$(CC) $(LDFLAGS) -o $(dtuEXE) $^

OBJ:$(SRC)
	$(CC) $(CFLAGS) -o $(OBJ) $^

clean:
	-rm *.o $(dtuEXE)
