/*Generate a image of a cube using sdl and rotate it
Created 05/02/26
by Andres Eufrasio Tinajero

*/
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>

typedef struct 
{
    float x, y, z;
}point_3d;

typedef struct {
    float x, y;
}point_2d;

point_2d project(point_3d point, int width, int height){
    const float distance = 50.0f;
    
    float scale = distance / (point.z + distance);
    point_2d projected_point;
    projected_point.x = (int)(point.x * scale + width / 2);
    projected_point.y = (int)(point.y * scale + height / 2);
    return projected_point;
}

point_3d rotateCube(point_3d p, double angleX, double angleY, double angleZ) {
    point_3d rotated;
    double x = p.x, y = p.y, z = p.z;

    // Rotation around X axis
    double tempY = y * cos(angleX) - z * sin(angleX);
    double tempZ = y * sin(angleX) + z * cos(angleX);
    y = tempY;
    z = tempZ;

    // Rotation around Y axis
    double tempX = x * cos(angleY) + z * sin(angleY);
    tempZ = -x * sin(angleY) + z * cos(angleY);
    x = tempX;
    z = tempZ;

    // Rotation around Z axis
    tempX = x * cos(angleZ) - y * sin(angleZ);
    tempY = x * sin(angleZ) + y * cos(angleZ);
    x = tempX;
    y = tempY;

    rotated.x = x;
    rotated.y = y;
    rotated.z = z;
    return rotated;
}

int main(int argc, char * argv[]){
    const char window_name[] = "CUBE"; 
    SDL_Window * window;
    SDL_Renderer * renderer;
    SDL_Texture * texture;
    int width = 500;
    int height = 500;

    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Cube",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED ,width,height,SDL_WINDOW_ALWAYS_ON_TOP);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
   
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, width, height);
    SDL_SetRenderTarget( renderer, texture);
    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 100);
    //points
    float start1 = 0.0f;
    float start2 = 50.0f;
    SDL_Point points [8];
    point_3d points3d[] = {
        {start1, start1, 0},
        {start2, start1, 0},
        {start2, start2, 0},
        {start1, start2, 0},

        {start1, start1, 50},
        {start2, start1, 50},
        {start2, start2, 50},
        {start1, start2, 50}
    };
    for (int i = 0; i < 8; i++){
    points3d[i]=rotateCube(points3d[i],0.0f,0.40f,0.0);
    }
    
    
    point_2d projected_points[8];
    for (int i = 0; i < 8; i++){
        projected_points[i] = project(points3d[i], width, height);
        points[i].x = projected_points[i].x, 
        points[i].y = projected_points[i].y ;
    }


    for (int index = 0; index <4; index++){
    // vertical edges
        SDL_RenderDrawLine(
            renderer,
            points[index].x, points[index].y,
            points[index + 4].x, points[index + 4].y
        );

        // bottom square
        SDL_RenderDrawLine(
            renderer,
            points[index].x, points[index].y,
            points[(index + 1) % 4].x, points[(index + 1) % 4].y
        );

        // top square
        SDL_RenderDrawLine(
            renderer,
            points[index + 4].x, points[index + 4].y,
            points[((index + 1) % 4) + 4].x, points[((index + 1) % 4) + 4].y
        );
    }
    for (int i = 0; i < 2; i++){
    points3d[i]=rotateCube(points3d[i],0.0f,0.40f,0.0);
    }
    SDL_SetRenderTarget( renderer, NULL);
    while (1) {
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            // quit check
            if (e.type == SDL_QUIT) {
                break;
            }
        }
        
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
        SDL_RenderClear(renderer);
        SDL_SetRenderTarget(renderer, texture);

        
        
        point_2d projected_points[8];
        for (int i = 0; i < 8; i++){
            projected_points[i] = project(points3d[i], width, height);
            points[i].x = projected_points[i].x, 
            points[i].y = projected_points[i].y ;
        }


        for (int index = 0; index <4; index++){
        // vertical edges
            SDL_RenderDrawLine(
                renderer,
                points[index].x, points[index].y,
                points[index + 4].x, points[index + 4].y
            );

            // bottom square
            SDL_RenderDrawLine(
                renderer,
                points[index].x, points[index].y,
                points[(index + 1) % 4].x, points[(index + 1) % 4].y
            );

            // top square
            SDL_RenderDrawLine(
                renderer,
                points[index + 4].x, points[index + 4].y,
                points[((index + 1) % 4) + 4].x, points[((index + 1) % 4) + 4].y
            );
        }
        
        SDL_SetRenderTarget(renderer, NULL);
            
        
    }
    



    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}

