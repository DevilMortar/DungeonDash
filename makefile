flags = -lm $(shell pkg-config --cflags --libs sdl2) -lSDL2_ttf -lSDL2_image -Og


main: main.o engine.o display.o
	gcc -o main main.o engine.o display.o $(flags)

main.o : main.c engine.o display.o
	gcc -g -Wall -c main.c $(flags)

display.o : header.h
	gcc -g -Wall -c display.c $(flags)

engine.o : header.h
	gcc -g -Wall -c engine.c $(flags)
	
deepclean :
	rm -f *.o
	rm -f main

