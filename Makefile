CC := g++

main: main.o Communication.o

main.o Communication.o: Communication.h

clean:
	$(RM) *.o
	$(RM) main