CC := g++
LDLIBS := -lpigpiod_if2 -pthread

main: main.o Communication.o Pigpio.o

main.o Communication.o Pigpio.o: Communication.h Pigpio.h

clean:
	$(RM) *.o
	$(RM) main