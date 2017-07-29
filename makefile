
srf: srf.o \
	   Image.o \
		 gfx/gfx.o \
		 script/luaref.o \
		 script/event.o \
		 script/script.o \
		 script/gfx.o \
		 script/input.o \
		 script/timer.o \
		 script/font.o \
		 script/image.o
	
	g++ -Wno-unused -Wall -Wpedantic -Werror -g -o $@ $^ -lluajit-5.1 -lSDL2 -lGLEW -lGL -lGLU

%.o: %.c
	gcc -Wno-unused -Wall -Wpedantic -Werror -I. -g -c -o $@ $<

%.o: %.cpp
	g++ -Wno-unused -Wall -Wpedantic -Werror -I. -g -c -o $@ $<

