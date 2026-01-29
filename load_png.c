#include <stdio.h>
#include <SDL2/SDL.H>
#include <stdbool.h>
#include "png_reader.c"

/*load a png file and print using sdl2
by Andres Eufrasio Tinajero
created 29/01/26

TODO:
create loop for pixel retrivel from png data
implement render loop for contiunal image viewing

*/

int WinMain(int argc, char ** argv){

    char buff[30] = "";

    // Is sort of like the window itself
    SDL_Event event;


    // create i/o subsystem in this case for video
    SDL_Init(SDL_INIT_VIDEO);

    printf("Please enter an image that you wish to load: ");
    scanf("%s",buff);
    png_data image_data = read_png_file(buff);


    // create the display
    SDL_Window * window = SDL_CreateWindow("SDL2 Displaying Image", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, image_data.width, image_data.height, 0);   
    //create render function that allows the window to be rendered upon
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Error renderer creation");
        return 1;
    }
    // Raw pixel data into SDL format
    SDL_Surface* user_image;

    int x = 0;
    int y = 0;

    SDL_Rect pixel;
    pixel.x = x;
    pixel.y = y;
    pixel.w = 1;
    pixel.h = 1;

    SDL_RenderDrawRect(renderer, &pixel);
    SDL_SetRenderDrawColor(m_window_renderer, 0, 0, 0, 255);
    
    // use the renderer with the pixel data
    

    // unload the user_image from SDL_Surface
    SDL_FreeSurface(user_image);   

    // rendering loop
    while (true) {
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            // check for user quitting is so end
            if (e.type == SDL_QUIT) {
                break;
            }
        }
        SDL_RenderPresent(renderer);
    }
    // deload everything from memory
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    // deinitalize sdl image
    IMG_Quit();
    // clean up my SDL video subsystem
    SDL_Quit();
    return 0;
}