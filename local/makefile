flags = -lm $(shell pkg-config --cflags --libs sdl2) -lSDL2_ttf -lSDL2_image -lSDL2_mixer -Og
S = ./src
I = ./include
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

menu.o : $(S)/menu.c $(I)/menu.h $(I)/button.h $(I)/display.h $(I)/engine.h $(I)/file.h $(I)/sound.h
	gcc -g -Wall -c $(S)/menu.c $(flags)

buttons.o : $(S)/buttons.c $(I)/button.h $(I)/config.h $(I)/display.h $(I)/engine.h
	gcc -g -Wall -c $(S)/buttons.c $(flags)

game.o : $(S)/game.c $(I)/game.h $(I)/button.h $(I)/config.h $(I)/engine.h $(I)/sound.h
	gcc -g -Wall -c $(S)/game.c $(flags)

engine.o : $(S)/engine.c $(I)/config.h $(I)/display.h
	gcc -g -Wall -c $(S)/engine.c $(flags)

file.o : $(S)/file.c $(I)/file.h $(I)/engine.h
	gcc -g -Wall -c $(S)/file.c $(flags)

display.o : $(S)/display.c $(I)/display.h $(I)/config.h
	gcc -g -Wall -c $(S)/display.c $(flags)

sound.o : $(S)/sound.c $(I)/sound.h
	gcc -g -Wall -c $(S)/sound.c $(flags)
	
deepclean :
	rm -f *.o
	rm -f main
	rm -f $(B)/*
	rm -f $(B)/*.o

