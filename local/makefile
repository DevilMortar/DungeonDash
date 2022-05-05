flags = -lm $(shell pkg-config --cflags --libs sdl2) -lSDL2_ttf -lSDL2_image -lSDL2_mixer -Og
S = ./src
B = ./build


main: main.o engine.o display.o
	gcc -o main main.o engine.o display.o game.o menu.o buttons.o file.o sound.o $(flags)
	mv display.o $(B)/
	mv engine.o $(B)/
	mv main.o $(B)/
	mv game.o $(B)/
	mv menu.o $(B)/
	mv buttons.o $(B)/
	mv file.o $(B)/
	mv sound.o $(B)/

main.o : engine.o engine.o display.o game.o menu.o buttons.o file.o sound.o
	gcc -g -Wall -c $(S)/main.c $(flags)

menu.o : buttons.o
	gcc -g -Wall -c $(S)/menu.c $(flags)

buttons.o : game.o
	gcc -g -Wall -c $(S)/buttons.c $(flags)

game.o : display.o
	gcc -g -Wall -c $(S)/game.c $(flags)

display.o : engine.o
	gcc -g -Wall -c $(S)/display.c $(flags)

engine.o : file.o
	gcc -g -Wall -c $(S)/engine.c $(flags)

file.o : sound.o
	gcc -g -Wall -c $(S)/file.c $(flags)

sound.o : 
	gcc -g -Wall -c $(S)/sound.c $(flags)
	
deepclean :
	rm -f *.o
	rm -f main
	rm -f $(B)/*
	rm -f $(B)/*
	rm -f $(B)/*.o
	rm -f $(B)/*.o
	rm -f $(B)/*.o
	rm -f $(B)/*.o

