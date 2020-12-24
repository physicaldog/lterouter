CC = arm-linux-gnueabihf-gcc
ifdef DB
CFLAGS = -c -DDEBUG
else
CFLAGS = -c
endif

LDFLAGS  = -lpthread -lubox -luci

EXE = lteDtu
OBJ = suyi_dtu.o tcpServer.o tcpClient.o udpServer.o udpClient.o serial.o log.o
SRC = suyi_dtu.c
SRC += tcpServer.c
SRC += tcpClient.c
SRC += udpServer.c
SRC += udpClient.c
SRC += serial.c
SRC += log.c

EXE:$(OBJ)
	$(CC) $(LDFLAGS) -o $(dtuEXE) $^

OBJ:$(SRC)
	$(CC) $(CFLAGS) -o $(OBJ) $^

clean:
	-rm *.o $(dtuEXE)
