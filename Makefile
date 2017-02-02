CC=gcc
CFLAGS= -fdiagnostics-color=always -Wall -pedantic -Wextra 
LDFLAGS=-lpthread -lrt
EXEC=orchestrator msgclient msgserver


all:$(EXEC)

orchestrator: lib_uzzi.o orchestrator.o
	$(CC) -o $@ $^ $(LDFLAGS)
orchestrator.o: ./src/orchestrator.c ./include/lib_uzzi.h
	$(CC) $(CFLAGS) -c $^
msgclient: lib_uzzi.o msgclient.o
	$(CC) -o $@ $^  $(LDFLAGS)
msgclient.o: ./src/msgclient.c ./include/lib_uzzi.h
	$(CC) $(CFLAGS) -c $<
msgserver: lib_uzzi.o msgserver.o
	$(CC) -o $@ $^ $(LDFLAGS)
msgserver.o: ./src/msgserver.c ./include/lib_uzzi.h 
	$(CC) $(CFLAGS) -c $<
lib_uzzi:lib_uzzi.o 
	$(CC) -o $@ $^ $(LDFLAGS)
lib_uzzi.o:./src/lib_uzzi.c ./include/lib_uzzi.h 
	$(CC) $(CFLAGS) -c $^ $(LDFLAGS)
clean:
	rm -f *~ *.o
