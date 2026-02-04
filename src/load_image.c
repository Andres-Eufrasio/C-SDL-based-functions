#include "image_reader.c"
#include <SDL2/SDL.H>
#define rgba 4
#define rgb 3




/*Image loader
by Andres Eufrasio Tinajero
created 29/01/26

TODO:
allow cmd line as input
implement zooming 
add a fixed size implementation flag

*/

/*load a image file and present using sdl2.
returns 0 if failed or 1 if sucessful
*/
int main(int argc, char * argv[]){
    char fileName[260] = "";
    raw_image_data image_data;
    
    SDL_Event event;


    // create i/o subsystem in this case for video
    SDL_Init(SDL_INIT_VIDEO);

    printf("Please enter an image that you wish to load: ");
    scanf("%s",fileName);

     
    if (strcmp(fileName + strlen(fileName) - 3, "jpg") == 0 | strcmp(fileName + strlen(fileName) - 4, "jpeg") == 0) {
        
        image_data = read_jpeg_file(fileName);
        if (image_data.pixel_buff == NULL){
            printf("Image data not found");
            return 0;
        }
    }
    else{
        image_data = read_png_file(fileName);
        
        if (image_data.pixel_buff == NULL){
            printf("Image data not found");
            return 0;
        }
    }
    
    int width = image_data.width;
    int height = image_data.height;
    unsigned char * pixel_data = image_data.pixel_buff;
    
    // create the display
    SDL_Window * window = SDL_CreateWindow(fileName,  SDL_WINDOWPOS_CENTERED,  SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE);   
    // create render function that allows the window to be rendered upon
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Error renderer creation");
        return 0;
    }

    SDL_Texture * image; 
    
  

    
    SDL_SetRenderTarget( renderer, image );
    
    //!!TODOGET RID OF MAGIC NUMBER 
    if(image_data.pixel_length == rgba ){
        image = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, width, height);
        if (image == NULL){
            printf("SDL texture creation error");
        }

        SDL_UpdateTexture(image, NULL, pixel_data, width*rgba);

    }
    
    if(image_data.pixel_length == rgb){
        image = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_TARGET, width, height);
        if (image == NULL){
            printf("SDL texture creation error");
        }

        SDL_UpdateTexture(image, NULL, pixel_data, width*rgb);
        
    };
    free(pixel_data);
    
    // render back onto window
    SDL_SetRenderTarget( renderer, NULL );

    

    // rendering loop
    while (1) {
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            // quit check
            if (e.type == SDL_QUIT) {
                break;
            }
            
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, image, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);

    }
    // clearn up
    SDL_DestroyTexture(image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
}