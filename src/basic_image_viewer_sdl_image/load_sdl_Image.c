/*
By Andres Eufrasio Tinajero
27-01-26
basic CMD image loader for windows using SDL and SDL image 
- SDL version 2
*/

#include <stdio.h>
#include <SDL2/SDL.H>
#include <stdbool.h>
#include <SDL2/SDL_image.h>

// gcc Main.c -o Main.exe -I "C:\msys64\mingw64\include" -L "C:\msys64\mingw64\lib" -l mingw32 -l SDL2main -l SDL2 -l SDL2_image

// Have to use WinMain for windows
int WinMain(int argc, char ** argv){

    char buff[30] = "";

    // Is sort of like the window itself
    SDL_Event event;


    // create i/o subsystem in this case for video
    SDL_Init(SDL_INIT_VIDEO);

    printf("Please enter an image that you wish to load: ");
    
    scanf("%s",buff);
    // create the display
    SDL_Window * window = SDL_CreateWindow("SDL2 Displaying Image", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);   
    //create render function that allows the window to be rendered upon
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Error renderer creation");
        return 1;
    }

    // Raw pixel data into SDL format
    SDL_Surface* user_image = IMG_Load(buff);
    if (user_image == NULL){
        printf("There is no image called %s", buff);         
        return 2;
    }
    // use the renderer with the pixel data
    SDL_Texture* lettuce_tex = SDL_CreateTextureFromSurface(renderer, user_image);

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

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, lettuce_tex, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
    // deload everything from memory
    SDL_DestroyTexture(lettuce_tex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    // deinitalize sdl image
    IMG_Quit();
    // clean up my SDL video subsystem
    SDL_Quit();
    return 0;
}