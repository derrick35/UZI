CC=gcc
CFLAGS= -fdiagnostics-color=always -Wall -pedantic -Wextra -Wconversion 
LDFLAGS=-lpthread -lrt -g -lseccomp
EXEC=orchestrator msgclient blacklist


all:$(EXEC)

orchestrator: lib_uzzi.o orchestrator.o
	$(CC) -o $@ $^ $(LDFLAGS)
orchestrator.o: ./src/orchestrator.c ./include/lib_uzzi.h
	$(CC) $(CFLAGS) -c $^
msgclient: lib_uzzi.o msgclient.o
	$(CC) -o $@ $^  $(LDFLAGS)
msgclient.o: ./src/msgclient.c ./include/lib_uzzi.h
	$(CC) $(CFLAGS) -c $<
blacklist: lib_uzzi.o blacklist.o
	$(CC) -o $@ $^ $(LDFLAGS)
blacklist.o: ./src/blacklist.c ./include/lib_uzzi.h 
	$(CC) $(CFLAGS) -c $<
lib_uzzi:lib_uzzi.o 
	$(CC) -o $@ $^ $(LDFLAGS)
lib_uzzi.o:./src/lib_uzzi.c ./include/lib_uzzi.h 
	$(CC) $(CFLAGS) -c $^ $(LDFLAGS)
clean:
	rm -f *~ *.o
