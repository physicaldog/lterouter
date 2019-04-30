CC = arm-linux-gnueabihf-gcc
CFLAGS = -c
LDFLAGS  = -lpthread -lm

OBJ = suyi_web.o
SRC = Account.c ftpUpdate.c index.c RunningStatus.c settingNetMode.c settingLTE.c suyi_common.h


OBJ:$(SRC)
	$(CC) $(CFLAGS) -o $(OBJ) $^

clean:
	-rm *.o 
