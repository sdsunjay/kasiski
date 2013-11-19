CC = gcc

CFLAGS = -lm -W

all: kasiski ic ftable

kasiski: kasiskiProgram/kasiski.o
		$(CC) $(CFLAGS) -o kasiski kasiskiProgram/kasiski.o
ic: icProgram/ic.o
		$(CC) $(CFLAGS) -o ic icProgram/ic.o
ftable: ftableProgram/ftable.o
	 	$(CC) $(CFLAGS) -o ftable ftableProgram/ftable.o
clean:
		rm icProgram/*.o
	        rm ftableProgram/*.o
		rm kasiskiProgram/*.o
