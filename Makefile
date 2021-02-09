CC := g++
LDLIBS := -lpigpiod_if2 -pthread

main: main.o Communication.o Pigpio.o SoundSystem.o

main.o Communication.o Pigpio.o SoundSystem.o: Communication.h Pigpio.h SoundSystem.h

clean:
	$(RM) *.o
	$(RM) main