CC = arm-linux-gnueabihf-gcc
CFLAGS = -c
LDFLAGS  = -lpthread

OBJ = suyi_web.o
SRC = Account.c ftpUpdate.c index.c RunningStatus.c settingLTE.c suyi_common.h


OBJ:$(SRC)
	$(CC) $(CFLAGS) -o $(OBJ) $^

clean:
	-rm *.o 
