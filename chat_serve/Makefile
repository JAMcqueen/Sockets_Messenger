#
#	Joshua McQueen 
#	mcqueejo@onid.oregonstate.edu
#	CS375
#	project 1
# 

CC=g++
DEBUG=-g
FLAGS1= -lm -lrt -Wall -Wshadow -Wredundant-decls -Wmissing-declarations

CFLAGS=$(DEBUG) $(FLAGS1)
#CFLAGS=$(DEBUG) $(FLAGS1) $(FLAGS2) $(FLAGS3)
PROGS= chatserve 

all: $(PROGS)

chatserve: chatserve.o chat_helper.o
	$(CC) $(CFLAGS) -o chatserve chatserve.o chat_helper.o

chatserve.o: chatserve.cpp
	$(CC) $(CFLAGS) -c chatserve.cpp

chat_helper: chat_helper.o
	$(CC) $(CFLAGS) -o chat_helper chat_helper.o

chat_helper.o: chat_helper.cpp chat_helper.hpp
	$(CC) $(CFLAGS) -c chat_helper.cpp



clean:
	rm -f *.o $(PROGS)
