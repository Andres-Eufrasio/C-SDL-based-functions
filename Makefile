load_image: ./src/load_image.c
	gcc load_image -o load_image -lmingw32 -l SDL2main -l SDL2 -lpng -lz -ljpeg