flags = -lm $(shell pkg-config --cflags --libs sdl2) -lSDL2_ttf -lSDL2_image -lSDL2_mixer -Og
S_D = ./src/display
S_E = ./src/engine
S_K = ./src/kernel
S_M = ./src/menu
S = ./src
B_D = ./build/display
B_E = ./build/engine
B_M = ./build/menu
B = ./build


main: main.o engine.o display.o
	gcc -o main main.o engine.o display.o game.o menu.o buttons.o $(flags)
	mv display.o $(B_D)/
	mv engine.o $(B_E)/
	mv main.o $(B)/
	mv game.o $(B)/
	mv menu.o $(B_M)/
	mv buttons.o $(B_M)/

main.o : engine.o engine.o display.o game.o menu.o buttons.o
	gcc -g -Wall -c $(S)/main.c $(flags)

menu.o : buttons.o
	gcc -g -Wall -c $(S_M)/menu.c $(flags)

buttons.o : game.o
	gcc -g -Wall -c $(S_M)/buttons.c $(flags)

game.o : display.o
	gcc -g -Wall -c $(S_K)/game.c $(flags)

display.o : engine.o
	gcc -g -Wall -c $(S_D)/display.c $(flags)

engine.o :
	gcc -g -Wall -c $(S_E)/engine.c $(flags)
	
deepclean :
	rm -f *.o
	rm -f main
	rm -f $(B_D)/*
	rm -f $(B_E)/*
	rm -f $(B)/*.o
	rm -f $(B)/*.o
	rm -f $(B_M)/*.o

