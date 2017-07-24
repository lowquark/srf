
srf: srf.c \
	   queue.c \
		 gfx/gfx.c \
		 script/luaref.c \
		 script/event.c \
		 script/script.c \
		 script/gfx.c \
		 script/input.c \
		 script/timer.c
	
	gcc -Wno-unused -Wall -Wpedantic -Werror -g -o $@ $^ -lluajit-5.1 -lSDL2 -lGLEW -lGL -lGLU -I.

