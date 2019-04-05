//#include <stdint.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <SDL.h>
//#include <SDL_opengl.h>
//#include <stdbool.h>
//#include <inttypes.h>
//#include <time.h>
//#include <SDL_image.h>
//#include <SDL_ttf.h>
//
//SDL_Texture *LoadTexture(char filePath[],SDL_Renderer *renderTarget){
//
//    SDL_Surface *texture = NULL;
//    SDL_Surface *surface = IMG_Load(filePath);
//    if(surface == NULL){
//        printf("Error : Loading image");
//    }
//    else{
//        texture = SDL_CreateTextureFromSurface(renderTarget, surface);
//        if(texture==NULL){
//            printf("Error : texture rendering");
//        }
//    }
//
//    SDL_FreeSurface(surface);
//    return texture;
//}
//
//int main(int argc, char** argv){
//    SDL_Init(SDL_INIT_EVERYTHING);
//    TTF_Init();
//    SDL_Window *window = SDL_CreateWindow("Brick Breaker",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,900,700,SDL_WINDOW_SHOWN);
//    SDL_Texture *renderTarget =SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
//    SDL_Texture *background = LoadTexture("res/Blue.jpg",renderTarget);
//    SDL_Event ev;
//    SDL_Color color = {144,77,255,255};
//    TTF_Font *font = TTF_OpenFont("res/Raleway-Black.ttf",20);
//
//    SDL_Surface *textSurface = TTF_RenderText_Solid(font,"Lives ",color);
//    SDL_Texture *text = SDL_CreateTextureFromSurface(renderTarget,textSurface);
//    SDL_Rect textRect;
//    textRect.x=textRect.y=0;
//    SDL_QueryTexture(text,NULL,NULL,&textRect.w,&textRect.h);
//
//    SDL_FreeSurface(textSurface);
//
//    bool isRunning = 1;
//
//        while(isRunning){
//
//        while(SDL_PollEvent(&ev) !=0){
//            if(ev.type == SDL_QUIT)
//                isRunning = false;
//
//            SDL_RenderClear(renderTarget);
//           // SDL_RenderCopy(renderTarget,background,NULL,NULL);
//            SDL_RenderCopy(renderTarget,text,NULL,&textRect);
//            SDL_RenderPresent(renderTarget);
//
//        }
//
//    }
//}
