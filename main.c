#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <stdbool.h>
#include <inttypes.h>
#include <time.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <ctype.h>

SDL_Window  *window = NULL;
SDL_Texture *background = NULL;
SDL_Texture *renderTarget =NULL;
SDL_Texture *BallImage = NULL;
SDL_Texture *BrickImage=NULL;
SDL_Texture *PaddleImage = NULL;
SDL_Texture *GameOver= NULL;
SDL_Texture *Win = NULL;
SDL_Texture *text = NULL;
SDL_Texture *score = NULL;
SDL_Texture *livesImage = NULL;
SDL_Texture *QuitImage = NULL;
SDL_Texture *StartImage = NULL;

SDL_Rect start, quit;

SDL_Rect FakeBall[100];
int count =0;
int FakeVelx[100],FakeVely[100];

SDL_Rect Lives[3];
SDL_Rect textRect;
SDL_Rect scoreRect;
SDL_Rect Paddle;
SDL_Rect Ball;

TTF_Font *font = NULL;
int Score = 0;
char scoreArray[12]="Score: 0000";
char *p = &Score;
const int ScreenW=1000,ScreenH=800;

int level = 4;
int xVel,yVel,Numlives=2,PaddleSpeed=10;
float deltaTime = 0;
int M=5,N=5;
struct node*head=NULL,*tail=NULL;
int BrickDraw=1;

struct node{
    struct node *next,*prev;
    SDL_Rect Brick;
};

SDL_Texture *LoadTexture(char filePath[],SDL_Renderer *renderTarget){

    SDL_Surface *texture = NULL;
    SDL_Surface *surface = IMG_Load(filePath);
    if(surface == NULL){

        printf("Error : Loading image");
    }
    else{
        texture = SDL_CreateTextureFromSurface(renderTarget, surface);
        if(texture==NULL){
            printf("Error : texture rendering");
        }
    }

    SDL_FreeSurface(surface);
    return texture;
}

void insertAtHead(double x_axis, double y_axis, double brick_Xsize, double brick_Ysize){

    struct node* temp = (struct node *)malloc(sizeof(struct node));
    temp->Brick.x = x_axis;
    temp->Brick.y = y_axis;
    temp->Brick.h = brick_Ysize;
    temp->Brick.w = brick_Xsize;

    if(head == NULL){
    temp->prev = head;
    temp->next = head;
    tail = temp;
    head = temp;
    }
    else{
    temp->next = head;
    head->prev = temp;
    head = temp;
    temp->prev = NULL;
    }



}

void levelFunction(int level){

int level_1[M][N],level_2[M][N];

int i=0, j=0;

    if(level==1){
        for(i=0 ; i<M ; i++){
            for(j=0 ; j<N ; j++){
                level_1[i][j] = 1;
            }
        }
        createBricks(level_1);
}

    if(level==2){
        for(i=0 ; i<M ; i++){

            for(j=0 ; j<N ; j++){
                if(i==0 || i==M-1 || j==0 || j==N-1)
                    level_1[i][j] = 1;

                else
                    level_1[i][j] = 0;
            }
        }
    createBricks(level_1);
    }

    if(level==3){
        for(i=0 ; i<M ; i++){

            for(j=0 ; j<N ; j++){
                if(j<=i)
                    level_1[i][j] = 1;

                else
                    level_1[i][j] = 0;
            }
        }
    createBricks(level_1);
    }

    if(level==4){


        for(i=0 ; i<M ; i++){

            for(j=0 ; j<N ; j++){
                if(j%2==0 || i%2==0)
                    level_1[i][j] = 1;
                else
                    level_1[i][j] = 0;
            }
        }
    createBricks(level_1);
    }

    if(level==5){

        for(i=1;i<=30;i++){
            insertAtHead(430,394,120,40);
            }

        }

    }

void createBricks(int lvl[M][N]){

    int i, j;
    double padding = (0.2*ScreenW)/2;
    double innerPadding = (0.1*ScreenW)/(N-1);
    double brick_Xsize =(0.65 * ScreenW)/N;
    double brick_Ysize = (0.35 * (ScreenH/2))/M;
    double x_axis = padding, y_axis = padding;

	for(i=0 ; i<M ; i++){

		for(j=0 ; j<N ; j++){

			if(lvl[i][j] == 1){
                insertAtHead(x_axis,y_axis,brick_Xsize,brick_Ysize);
                x_axis = x_axis + innerPadding + brick_Xsize;
			}
			else if(lvl[i][j]==0)
                x_axis = x_axis + innerPadding + brick_Xsize;

		}
        x_axis = padding;
        y_axis = y_axis + innerPadding + brick_Ysize;
	}
}

void DrawBricks(){
    struct node* temp = head;

    while(temp!=NULL){
        if(level==5){
        if(BrickDraw%2!=0){
                SDL_SetRenderDrawColor(renderTarget,0,0,0,0);
                SDL_RenderDrawRect(renderTarget,&temp->Brick);
               // SDL_RenderCopy(renderTarget,BrickImage,NULL,&temp->Brick);
                temp=temp->next;

            }
        if(BrickDraw%2==0){
                SDL_SetRenderDrawColor(renderTarget,255,0,0,0);
                SDL_RenderDrawRect(renderTarget,&temp->Brick);
                //SDL_RenderCopy(renderTarget,BrickImage,NULL,&temp->Brick);
                temp=temp->next;
            }
            BrickDraw++;

        }
        else{
        SDL_RenderCopy(renderTarget,BrickImage,NULL,&temp->Brick);
        temp=temp->next;
        }
    }
    free(temp);
    return;
}

int myRandom(){
    int randomnumber;
    randomnumber = rand() % 10;
    printf("%d\n", randomnumber);
    return randomnumber;
}

void loadGame(){


    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow("Brick Breaker",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,ScreenW,ScreenH,SDL_WINDOW_SHOWN);

    renderTarget = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

    background = LoadTexture("res/Last.jpg",renderTarget);

    StartImage = LoadTexture("res/Blue.jpg",renderTarget);

    QuitImage = LoadTexture("res/nature.jpg",renderTarget);

    BrickImage = LoadTexture("res/BlueBrick.png",renderTarget);

    PaddleImage = LoadTexture("res/Paddle.png",renderTarget);

    GameOver = LoadTexture("res/GameOver.png",renderTarget);

    Win = LoadTexture("res/Win.png",renderTarget);

    livesImage = LoadTexture("res/Lives.png",renderTarget);

    BallImage = LoadTexture("res/blueball.png",renderTarget);

    Ball.x = 440;
    Ball.y = 340;
    Ball.w = 20;
    Ball.h = 20;

    start.x = ScreenH/2;
    start.y = 340;
    start.w = ScreenW/5;
    start.h = ScreenW/10;

    quit.x = ScreenH/2;
    quit.y = 460;
    quit.w = ScreenW/5;
    quit.h = ScreenW/10;


    Paddle.h = 20;
    Paddle.w = 100;
    Paddle.y = ScreenH-Paddle.h-10;
    Paddle.x = ScreenW/2-Paddle.w/2;


    xVel = 7;
    yVel = 7;

    int fball=0;
    for(fball;fball<100;fball++){
        FakeBall[fball].x=NULL;
        FakeBall[fball].y=NULL;
        FakeBall[fball].w=20;
        FakeBall[fball].h=20;
    }


    lives();


}

void lives(){
    Lives[0].x = 60;
    Lives[0].y = 10;
    Lives[0].h = 20;
    Lives[0].w = 20;

    Lives[1].x = 90;
    Lives[1].y = 10;
    Lives[1].h = 20;
    Lives[1].w = 20;

    Lives[2].x = 120;
    Lives[2].y = 10;
    Lives[2].h = 20;
    Lives[2].w = 20;
}

bool pointInRect(int x, int y, SDL_Rect rec){
   if (x>rec.x && y>rec.y && x<rec.x + rec.w && y<rec.y + rec.h){
        return true;
    }
    return false;
}

bool checkCollision(SDL_Rect r1, SDL_Rect r2){

    if( pointInRect(r1.x, r1.y, r2) ==  true ||
        pointInRect(r1.x+r1.w, r1.y, r2)== true ||
        pointInRect(r1.x, r1.y+r1.h, r2)== true ||
         pointInRect(r1.x+r1.w, r1.y+r1.h, r2)== true)
         {
            return true;
         }

         return false;
}

void mouseController(int x, int y){
    printf("%d %d", x,y);
    if(pointInRect(x,y,start) == true){
        level++;
        levelFunction(level);
        background = LoadTexture("res/Blue.jpg",renderTarget);

    }
    if(pointInRect(x,y,quit) == true){
        Quit();

    }

}

void Quit(){
    TTF_CloseFont(font);
//    SDL_DestroyTexture(text);
    SDL_DestroyTexture(PaddleImage);
    SDL_DestroyTexture(BrickImage);
    SDL_DestroyTexture(BallImage);
    SDL_DestroyTexture(background);
    SDL_DestroyRenderer(renderTarget);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

void Pause(){
    int p =0;
    Uint8 *keystates = SDL_GetKeyboardState(NULL);
    while(p==0){
    if(keystates[SDL_SCANCODE_P]){
            p=1;
        }
    }
}

void DrawFakeBalls(){
    int i;
    for(i=0;i<100;i++){
        SDL_RenderCopy(renderTarget,BallImage,NULL,&FakeBall[i]);
    }
    return;
}

void BallMovement(){
    Ball.x += xVel;
    Ball.y += yVel;
    int i;
    for( i=0;i<100 && level == 5;i++){
        if( FakeBall[i].y!=NULL){

                if(FakeBall[i].x <1 || FakeBall[i].x > ScreenW-20 ){
                    FakeVelx[i] *= -1;
                }
                if(FakeBall[i].y < 1 || FakeBall[i].y >ScreenH-21){
                    FakeVely[i] *= -1;
                }

            FakeBall[i].x +=FakeVelx[i];
            FakeBall[i].y +=FakeVely[i];
        }
    }


    if(Ball.y < 1){
        yVel *= -1;
    }

    if(Ball.y >= ScreenH-20){
        Lives[Numlives].w = 0;
        Lives[Numlives].h = 0;
        Ball.x=440;
        Ball.y=440;
        Paddle.y = ScreenH-Paddle.h-10;
        Paddle.x = ScreenW/2-Paddle.w/2;

        xVel=1+myRandom();
        SDL_Delay(1500);

        if(Numlives>=0)
        {
            Numlives--;
        }

    }

    if(Ball.x <1 || Ball.x > ScreenW-Ball.w ){
        xVel *= -1;
    }

    if(checkCollision(Ball,Paddle)==true){
        yVel *= -1;
    }

    struct node *temp = head;
    while(temp!=NULL ){
      if(checkCollision(Ball,temp->Brick)==true){
        yVel *= -1;
        Score+=10;


        if(level==5){
                printf("fake ball added!");
            FakeBall[count].x=490;
            FakeBall[count].y=23;
            FakeVelx[count]=myRandom()+2;
            FakeVely[count]=myRandom()+2;
            count++;

        }

        printf("Score %d!\n",Score);
        if(temp==head && temp==tail){
            head=NULL;
            tail=NULL;
            free(temp);
            break;
        }
        if(temp==head){
            temp->next->prev=NULL;
            head=temp->next;
            free(temp);
            break;
            }
        if(temp==tail){
            temp->prev->next=NULL;
            tail=temp->prev;
            free(temp);
            break;
            }
        temp->prev->next=temp->next;
        temp->next->prev=temp->prev;
        free(temp);
        break;
    }
      temp=temp->next;
    }



}

void logic(){

    if(level!=0){
    Uint8 *keystates = SDL_GetKeyboardState(NULL);

    if(keystates[SDL_SCANCODE_A] && Paddle.x > 0){
        Paddle.x -= PaddleSpeed;
    }
    if(keystates[SDL_SCANCODE_D] && Paddle.x+Paddle.w <ScreenW){
        Paddle.x += PaddleSpeed;
    }


    scoreHandler();

    //Ball movement
    BallMovement();
    }



    if(head==NULL && tail==NULL && level <5 && level!=0)
    {

        level++;
        levelFunction(level);
        printf("level is : %d\n",level);

            if(level==2){
                    background = LoadTexture("res/lava-volcano.jpg",renderTarget);
                    BrickImage = LoadTexture("res/RedBrick.png",renderTarget);
                    BallImage = LoadTexture("res/ball.png",renderTarget);
                    PaddleImage = LoadTexture("res/RedPaddle.png",renderTarget);
                }
            if(level==3){
                background = LoadTexture("res/nature.jpg",renderTarget);
                BrickImage = LoadTexture("res/BrownBrick.png",renderTarget);
                BallImage = LoadTexture("res/greenBall.png",renderTarget);
                PaddleImage = LoadTexture("res/GreenPaddle.png",renderTarget);
            }
            if(level==4){
                background = LoadTexture("res/space.jpg",renderTarget);
                BrickImage = LoadTexture("res/BlueBrick.png",renderTarget);
                BallImage = LoadTexture("res/blueball.png",renderTarget);
                PaddleImage = LoadTexture("res/Paddle.png",renderTarget);
            }
            if(level==5){
                background = LoadTexture("res/Last.jpg",renderTarget);

                BallImage = LoadTexture("res/fireball.png",renderTarget);
                PaddleImage = LoadTexture("res/RedPaddle.png",renderTarget);

            }
        }
}

void drawScreen(TTF_Font *font){
    SDL_SetRenderDrawColor(renderTarget,255,0,0,255);
    SDL_Color color = {255,0,0,255};

    if(level == 0){
        SDL_RenderClear(renderTarget);
        SDL_RenderCopy(renderTarget,background,NULL,NULL);
        SDL_RenderCopy(renderTarget,StartImage,NULL,&start);
        SDL_RenderCopy(renderTarget,QuitImage,NULL,&quit);
        SDL_RenderPresent(renderTarget);
        return;
    }

    if(head==NULL && tail==NULL){
        SDL_RenderClear(renderTarget);
        SDL_RenderCopy(renderTarget,Win,NULL,NULL);
        SDL_RenderPresent(renderTarget);
        return;
    }
    if(Numlives==-1){
        SDL_RenderClear(renderTarget);
        SDL_RenderCopy(renderTarget,GameOver,NULL,NULL);
        SDL_RenderPresent(renderTarget);
        return;
    }


    SDL_RenderClear(renderTarget);
    SDL_RenderCopy(renderTarget,background,NULL,NULL);
    SDL_RenderCopy(renderTarget,BallImage,NULL,&Ball);
    SDL_RenderCopy(renderTarget,PaddleImage,NULL,&Paddle);
    DrawBricks();
    SDL_RenderCopy(renderTarget,livesImage,NULL,&Lives[0]);
    SDL_RenderCopy(renderTarget,livesImage,NULL,&Lives[1]);
    SDL_RenderCopy(renderTarget,livesImage,NULL,&Lives[2]);
    truefont(font);
    if(level==5)DrawFakeBalls();
    SDL_RenderPresent(renderTarget);


}

void truefont(TTF_Font *font){
    SDL_Color color = {255,0,0,255};
    SDL_Surface *textSurface = TTF_RenderText_Solid(font,"Lives :",color);
    SDL_Texture *text = SDL_CreateTextureFromSurface(renderTarget,textSurface);
    SDL_RenderCopy(renderTarget,text,NULL,&textRect);
    SDL_FreeSurface(textSurface);
    textRect.x=-2;
    textRect.y=7;
    SDL_QueryTexture(text,NULL,NULL,&textRect.w,&textRect.h);


    SDL_Color colorYellow = {255,255,255,0};
    textSurface = TTF_RenderText_Solid(font, scoreArray,colorYellow);
    SDL_Texture *score = SDL_CreateTextureFromSurface(renderTarget,textSurface);
    SDL_RenderCopy(renderTarget,score,NULL,&scoreRect);
    SDL_FreeSurface(textSurface);
    scoreRect.x=800;
    scoreRect.y=7;
    SDL_QueryTexture(score,NULL,NULL,&scoreRect.w,&scoreRect.h);

}

void scoreHandler(){
	int x = Score/1000;
	scoreArray[7]='0'+x;

	x=(Score/100)%10;
	scoreArray[8]='0'+x;

	x=(Score/10)%10;
	scoreArray[9]='0'+x;

	x=(Score)%10;
	scoreArray[10]='0'+x;

}

int main(int argc, char** argv){
    const int FPS = 60;
    float frameTime = 0;
    int prevTime = 0;
    int currentTime =0;
    srand(time(NULL));
    loadGame();

    TTF_Init();
    TTF_Font *font = TTF_OpenFont("res/Raleway-Black.ttf",20);

    bool isRunning = true;
    SDL_Event ev;

    while(isRunning){
        prevTime = currentTime;
        currentTime= SDL_GetTicks();
        deltaTime = (currentTime - prevTime)/1000.0f;
        while(SDL_PollEvent(&ev) !=0){
            if(ev.type == SDL_QUIT)
                isRunning = false;

            else if(ev.type ==  SDL_MOUSEBUTTONDOWN){
                if(ev.button.button == SDL_BUTTON_LEFT){
                    mouseController(ev.button.x, ev.button.y);
                }
            }

        }
        frameTime+=deltaTime;
        if(frameTime >= 0.01666666f){  //60 frames per sec
            frameTime = 0;
            logic();
            drawScreen(font);
        }
    }

    Quit();
    return 0;
}



