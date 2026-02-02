

#include "image_reader.c"
#include <SDL2/SDL.H>


/*load a png file and print using sdl2
by Andres Eufrasio Tinajero
created 29/01/26

TODO:
rplace use pngbyte 
fix memory leak
allow cmd line as input

implement zooming 
add a fixed size implementation 

*/


  

int main(int argc, char * argv[]){
    char fileName[260] = "";
    raw_image_data image_data;
    // Is sort of like the window itself
    SDL_Event event;


    // create i/o subsystem in this case for video
    SDL_Init(SDL_INIT_VIDEO);

    printf("Please enter an image that you wish to load: ");
    scanf("%s",fileName);

     
    if (strcmp(fileName + strlen(fileName) - 3, "jpg") == 0) {
        
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
    unsigned char ** pixel_data = image_data.pixel_buff;
    
    // create the display
    SDL_Window * window = SDL_CreateWindow(fileName,  SDL_WINDOWPOS_CENTERED,  SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE);   
    // create render function that allows the window to be rendered upon
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Error renderer creation");
        return 0;
    }
    /*
    make texture conditional to not only include SDL_PIXELFORMAT_RGBA8888
    */
    SDL_Texture * image = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);

    /*Use SDL_UpdateTexture(image, NULL, pixel_data, width*4);
    make data storage contigous as it requires pixel_data to be *
    
    */
    
    SDL_SetRenderTarget( renderer, image );
    int r;
    int g;
    int b;
    int a;
    
    if(image_data.pixel_length == 4 ){
    for (int y=0; y < height;y++){
        unsigned char *row = pixel_data[y];
        for (int x=0; x < width; x++){
                unsigned char * pixel = &row[x * 4];
                r = pixel[0];
                g = pixel[1];
                b = pixel[2];
                a = pixel[3];

                SDL_SetRenderDrawColor(renderer, r,g,b,a);
                SDL_RenderDrawPoint(renderer, x, y);                             

        }
        free(pixel_data[y]);
        
        }
        free(pixel_data);
    }
    
    if(image_data.pixel_length == 3){
        
        for (int y=0; y < height;y++){
            unsigned char *row = pixel_data[y];
            for (int x=0; x < width; x++){
                    unsigned char * pixel = &row[x * 3];
                    r = pixel[0];
                    g = pixel[1];
                    b = pixel[2];
                    

                    SDL_SetRenderDrawColor(renderer, r,g,b,255);
                    SDL_RenderDrawPoint(renderer, x, y);        
                      

            }
            free(pixel_data[y]);

            
        }
        free(pixel_data);
        
    };
    
    
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