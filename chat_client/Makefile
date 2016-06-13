#
#	Joshua McQueen 
#	mcqueejo@onid.oregonstate.edu
#	CS375
#	project 1
# 

CC=gcc
DEBUG=-g
FLAGS1= -lm -lrt -Wall -Wshadow -Wredundant-decls -Wmissing-declarations

CFLAGS=$(DEBUG) $(FLAGS1)
PROGS= chatclient 

all: $(PROGS)

chatclient: chatclient.o chat_helper.o
	$(CC) $(CFLAGS) -o chatclient chatclient.o chat_helper.o

chatclient.o: chatclient.c
	$(CC) $(CFLAGS) -c chatclient.c

chat_helper: chat_helper.o
	$(CC) $(CFLAGS) -o chat_helper chat_helper.o

chat_helper.o: chat_helper.c chat_helper.h
	$(CC) $(CFLAGS) -c chat_helper.c



clean:
	rm -f *.o $(PROGS)
