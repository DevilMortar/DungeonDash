flags = -lm $(shell pkg-config --cflags --libs sdl2) -lSDL2_ttf -lSDL2_image -Og
S_D = ./src/display
S_E = ./src/engine
S = ./src
B_D = ./build/display
B_E = ./build/engine
B = ./build


main: main.o engine.o display.o
	gcc -o main main.o engine.o display.o $(flags)
	mv display.o $(B_D)/
	mv engine.o $(B_E)/
	mv main.o $(B)/

main.o : $(S)/main.c engine.o display.o
	gcc -g -Wall -c $(S)/main.c $(flags)

display.o : $(S_D)/display.h engine.o
	gcc -g -Wall -c $(S_D)/display.c $(flags)

engine.o : $(S_E)/engine.h
	gcc -g -Wall -c $(S_E)/engine.c $(flags)
	
deepclean :
	rm -f *.o
	rm -f main
	rm -f $(B_D)/*
	rm -f $(B_E)/*
	rm -f $(B)/*.o

