
srf: build/srf.o \
	   build/Image.o \
		 build/gfx/gfx.o \
		 build/gfx/draw.o \
		 build/gfx/TileMap.o \
		 build/gfx/gl/Texture.o \
		 build/gfx/gl/Program.o \
		 build/gfx/gl/Buffer.o \
		 build/script/luaref.o \
		 build/script/event.o \
		 build/script/script.o \
		 build/script/gfx.o \
		 build/script/input.o \
		 build/script/timer.o \
		 build/script/font.o \
		 build/script/image.o
	
	g++ -Wno-unused -Wall -Wpedantic -Werror -g -o $@ $^ -lluajit-5.1 -lSDL2 -lGLEW -lGL -lGLU

build/%.o: src/%.cpp
	@mkdir --parents ${@D}
	g++ -Wno-unused -Wall -Wpedantic -Werror -Isrc/ -g -c -o $@ $<

.PHONY: clean
clean:
	rm srf
	rm -r build/

