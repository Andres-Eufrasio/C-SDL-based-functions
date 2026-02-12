#include "file_image_reader.c"
#include <SDL2/SDL.h>
#define rgba 4
#define rgb 3




/*Image loader
by Andres Eufrasio Tinajero
created 29/01/26

TODO:

finish implement zooming 
add --help
add .ico
add .bmp
add a fixed size implementation flag
*/







void keep_texure_on_render(){

}

/*load a image file and present using sdl2.
use argument filename to select a file
returns 0 if failed or 1 if sucessful
*/
int main(int argc, char * argv[]){
    
    char fileName[260] = "";
    raw_image_data image_data;
    SDL_Window * window;
    SDL_Renderer * renderer;
    SDL_Texture * texture; 


    SDL_Init(SDL_INIT_VIDEO);
    
    if (argc == 1){
    printf("Please enter an image that you wish to load: ");
    scanf("%s",fileName);
    if (fileName[0] == '\0'){
        printf("No file name found");
        return 0;
    }   
    }
    else if (argc == 2){
        strncpy(fileName, argv[1], 259);
        SDL_Delay(2000);
    }


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
    if (width<100 || height <100){
    window = SDL_CreateWindow(fileName,  SDL_WINDOWPOS_CENTERED,  SDL_WINDOWPOS_CENTERED, 200, 100, SDL_WINDOW_RESIZABLE);   
    }
    else if (width>1920 || height >1080){
        window = SDL_CreateWindow(fileName,  SDL_WINDOWPOS_CENTERED,  SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_RESIZABLE);   
    }
    else{
        window = SDL_CreateWindow(fileName,  SDL_WINDOWPOS_CENTERED,  SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE);   
    }

    raw_image_data icon = read_png_file("./resources/icon.png");
    if (icon.pixel_buff == NULL){
        goto skipIcon;
    }
    SDL_Surface * window_icon = SDL_CreateRGBSurfaceFrom(icon.pixel_buff,icon.width,icon.height,32,icon.width*rgba,0x000000ff,0x0000ff00,0x00ff0000,0xff000000);
    
    if (window_icon == NULL){
        printf("icon creation failed");
    }
    SDL_SetWindowIcon(window, window_icon);
    
    skipIcon: 
    free(icon.pixel_buff);
    
    // create render function that allows the window to be rendered upon
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Error renderer creation");
        return 0;
    }
    

    
    

    

    SDL_RenderSetLogicalSize(renderer, width, height);
    
    
    printf("\n");
    if(image_data.pixel_length == rgba ){
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, width, height);
        if (texture == NULL){
        }

        SDL_UpdateTexture(texture, NULL, pixel_data, width*rgba);

    }
    
    if(image_data.pixel_length == rgb){
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STATIC, width, height);
        if (texture == NULL){
        }

        SDL_UpdateTexture(texture, NULL, pixel_data, width*rgb);
        
    };
    free(pixel_data);
    

    
    
    float zoom_amount = 1;
    // rendering loop
    
    SDL_Rect rect_t;
    const SDL_Rect * dstrect = & rect_t;
    rect_t.h = height;
    rect_t.w = width;
    rect_t.x=0;
    rect_t.y=0;
    int old_w=0;
    int old_h=0;
    int buttonDown = 0;
    while (1) {
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            // quit check
            if (e.type == SDL_QUIT) {
                break;
            }
            if (e.type == SDL_MOUSEWHEEL){
                old_w = rect_t.w;
                old_h = rect_t.h;
                // zoom in 
                if (e.wheel.preciseY > 0){
                    zoom_amount+= 0.10;


                    rect_t.h  = height*zoom_amount;
                    rect_t.w = width*zoom_amount;  

                    rect_t.x = rect_t.x - (rect_t.w-old_w)/2;
                    rect_t.y = rect_t.y - (rect_t.h-old_h)/2;
                }
                // zoom out
                else{
                    
                    if (zoom_amount > 1){
                        zoom_amount -=0.10;                        
                    }
                    else{
                        zoom_amount = 1;
                        rect_t.x = 0;
                        rect_t.y = 0;

                    }


                    
                    rect_t.h  = height*zoom_amount;
                    rect_t.w = width*zoom_amount;
                    rect_t.x = rect_t.x + (old_w-rect_t.w)/2;
                    rect_t.y = rect_t.y + (old_h-rect_t.h)/2;



                }

                if (rect_t.x>0){
                    rect_t.x = 0;
                }
                if (rect_t.y>0){
                    rect_t.y = 0;
                }
                if (rect_t.y+rect_t.h<height){
                     rect_t.y = height-rect_t.h;
                }
                if (rect_t.x+rect_t.w<width){
                     rect_t.x = width-rect_t.w;
                }         

            }
            if (e.type == SDL_MOUSEBUTTONDOWN){
                buttonDown = 1;
            }
            if (e.type == SDL_MOUSEBUTTONUP){
                buttonDown = 0;
            }
            if (e.type == SDL_MOUSEMOTION && buttonDown == 1){
                //printf("%d",e.motion.xrel);

                // allow movment only on zoom in
                if (zoom_amount>1){
                    rect_t.x+= e.motion.xrel ;
                    rect_t.y+=e.motion.yrel;
                }

                if (rect_t.x>0){
                    rect_t.x = 0;
                }
                if (rect_t.y>0){
                    rect_t.y = 0;
                }
                if (rect_t.y+rect_t.h<height){
                     rect_t.y = height-rect_t.h;
                }
                if (rect_t.x+rect_t.w<width){
                     rect_t.x = width-rect_t.w;
                }                

                
            }
            
            //printf("%d-",rect_t.y+rect_t.h);
            
            
            
        }
        
        SDL_RenderCopy(renderer, texture, NULL , dstrect);
        //SDL_RenderSetScale(renderer, zoom_amount, zoom_amount);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
        SDL_RenderClear(renderer);

    }
    // clearn up
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
}