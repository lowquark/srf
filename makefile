
srf: build/srf.o \
	   build/util/Image.o \
	   build/util/load_png.o \
	   build/util/AsyncContext2.o \
		 build/db/db.o \
		 build/gfx/gfx.o \
		 build/gfx/draw.o \
		 build/gfx/Tilemap.o \
		 build/gfx/gl/Texture.o \
		 build/gfx/gl/Program.o \
		 build/script/luaref.o \
		 build/script/event.o \
		 build/script/script.o \
		 build/script/gfx.o \
		 build/script/input.o \
		 build/script/timer.o \
		 build/script/db.o
	
	g++ -Wno-unused -Wall -Wpedantic -Werror -g -o $@ $^ -lluajit-5.1 -lSDL2 -lGLEW -lGL -lGLU -lpng -lz -lpthread -lgdbm

build/%.o: src/%.cpp
	@mkdir --parents ${@D}
	g++ -Wno-unused -Wall -Wpedantic -Werror -Isrc/ -g -c -o $@ $<

.PHONY: clean
clean:
	rm srf
	rm -r build/

