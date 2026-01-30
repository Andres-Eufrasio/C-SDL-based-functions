#include <stdio.h>
#include <SDL2/SDL.H>
#include "image_reader.c"


/*load a png file and print using sdl2
by Andres Eufrasio Tinajero
created 29/01/26

TODO:
rplace use pngbyte 
fix memory leak
allow cmd line as input

implement zooming 
add a fixed size implementation 
replace raw_data with name pixel_buffer

*/


  

int WinMain(int argc, char ** argv){
    char fileName[260] = "";
    char datatype[10];
    raw_image_data image_data;
    // Is sort of like the window itself
    SDL_Event event;


    // create i/o subsystem in this case for video
    SDL_Init(SDL_INIT_VIDEO);

    printf("Please enter an image that you wish to load: ");
    scanf("%s",fileName);


    if (strcmp(fileName + strlen(fileName) - 4, "jpeg") == 0) {
        
        raw_image_data image_data = read_jpeg_file(fileName);
        if (image_data.pixel_buff == NULL){
            printf("Image data not found");
            return 0;
        }
        strcpy(datatype, "jpeg");
    }
    else{
        raw_image_data image_data = read_png_file(fileName);
        if (image_data.pixel_buff == NULL){
            printf("Image data not found");
            return 0;
        }
        strcpy(datatype, "png");
    }
    
    
    const int width = image_data.width;
    const int height = image_data.height;

    // create the display
    SDL_Window * window = SDL_CreateWindow("SDL2 Displaying Image",  SDL_WINDOWPOS_CENTERED,  SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE);   
    // create render function that allows the window to be rendered upon
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Error renderer creation");
        return 0;
    }
    // render onto Texture
    SDL_Texture * image = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
    SDL_SetRenderTarget( renderer, image );
    png_byte r;
    png_byte g;
    png_byte b;
    png_byte a;
    int px_count = 0;
    for (int y=0; y < image_data.height;y++){
        png_bytep row = image_data.pixel_buff[y];
        for (int x=0; x < image_data.width; x++){
                png_bytep pixel = &row[x * 4];
                r = pixel[0];
                g = pixel[1];
                b = pixel[2];
                a = pixel[3];

                SDL_SetRenderDrawColor(renderer, r,g,b,a);
                SDL_RenderDrawPoint(renderer, x, y);              
                

        }
        
        
    }
        
    // render back onto window
    SDL_SetRenderTarget( renderer, NULL );
    // rendering loop
    while (1) {
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            // check for user quitting is so end
            if (e.type == SDL_QUIT) {
                break;
            }
            
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, image, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);

    }
    // deload everything from memory
    SDL_DestroyTexture(image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    // clean up my SDL video subsystem
    SDL_Quit();
    return 0;
}