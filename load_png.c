#include <stdio.h>
#include <SDL2/SDL.H>
#include <stdbool.h>
#include "png_reader.c"


/*load a png file and print using sdl2
by Andres Eufrasio Tinajero
created 29/01/26

TODO:
add to sdl surface so that image is reloaded.
fix memory leak
allow cmd line as input
add jpeg compatiblity
implement zooming 

*/




int WinMain(int argc, char ** argv){
    char buff[260] = "";

    // Is sort of like the window itself
    SDL_Event event;


    // create i/o subsystem in this case for video
    SDL_Init(SDL_INIT_VIDEO);

    printf("Please enter an image that you wish to load: ");
    scanf("%s",buff);
    png_data image_data = read_png_file(buff);
    if (image_data.raw_data == NULL){
        printf("Image data not found");
        return 0;
    }

    // create the display
    SDL_Window * window = SDL_CreateWindow("SDL2 Displaying Image",  SDL_WINDOWPOS_CENTERED,  SDL_WINDOWPOS_CENTERED, image_data.width, image_data.height, SDL_WINDOW_RESIZABLE);   
    //create render function that allows the window to be rendered upon
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Error renderer creation");
        return 0;
    }

    png_byte r;
    png_byte g;
    png_byte b;
    png_byte a;
    int px_count = 0;
    for (int y=0; y < image_data.height;y++){
        png_bytep row = image_data.raw_data[y];
        for (int x=0; x < image_data.width; x++){
                png_bytep pixel = &row[x * 4];
                r = pixel[0];
                g = pixel[1];
                b = pixel[2];
                a = pixel[3];
                
                //printf("%02x %02x %02x %02x",r,g,b,a);
                SDL_SetRenderDrawColor(renderer, r,g,b,a);
                SDL_RenderDrawPoint(renderer, x, y);              
                //draw it one at a time for fun

        }
        
    }
    SDL_RenderPresent(renderer);
    // rendering loop
    while (true) {
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            // check for user quitting is so end
            if (e.type == SDL_QUIT) {
                break;
            }
            
        }

    }
    // deload everything from memory
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    // clean up my SDL video subsystem
    SDL_Quit();
    return 0;
}