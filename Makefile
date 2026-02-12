CC = gcc
CFLAGS = -I./lib/SDL2/include -I./lib/libpng/include -I./lib/jpeglib/include
LDFLAGS = -L./lib/SDL2/lib -L./lib/libpng/lib -L./lib/jpeglib/lib
LIBS = -lmingw32 -lSDL2main -lSDL2 -lpng -ljpeg -lz -lm

load_image: ./src/image_viewer/image_viewer.c
	$(CC) $(CFLAGS) ./src/image_viewer/image_viewer.c -o image_viewer $(LDFLAGS) $(LIBS) -w

clean:
	rm -f load_image image_viewer.exe image_viewer.o