#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

#ifdef __WIN32
    #include<windows.h>
# endif

#include "lib/vector.h"

#define FRAME_TIME 10
#define WIDTH 90
#define HEIGHT 25

struct s_position{
    int x;
    int y;
};

struct s_star{
    int animation[8];
    struct s_position pos;
    int numOfFrames, curAnimFrame;
};

void sleep_for(unsigned int mSecond)
{
#ifdef WIN32
    Sleep(mSecond);
#elif _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = mSecond / 1000;
    ts.tv_nsec = (mSecond % 1000) * 1000000;
    nanosleep(&ts, NULL);
#else
    if (mSecond >= 1000)
    sleep(mSecond / 1000);
    usleep((mSecond % 1000) * 1000);
#endif
}

void draw_sky(int screen[][WIDTH+3], PtlVector *vec)
{
    wprintf(L"\x1b[H");
    // printf("\x1b[H");
    for(int i=0; i<HEIGHT; i++){
        for(int j=0; j<WIDTH; j++){
            screen[i][j] = 0x00AD;
        }
    }

    for(int i=0; i<vec->len; i++){
        struct s_star cur = ptl_vector_get(struct s_star, vec, i);
        int x = cur.pos.x;
        int y = cur.pos.y;
        screen[y][x] = cur.animation[cur.curAnimFrame];
    }

    for(int j=0; j<WIDTH; j++){
        screen[HEIGHT-3][j] = 0x02D7; // -
    }

    for(int j=0; j<WIDTH; j++){
        screen[HEIGHT-2][j] = 0x003D; // =
    }
    for(int j=0; j<WIDTH; j++){
        screen[HEIGHT-1][j] = 0x0023; // #
    }


    for(int i=0; i<HEIGHT; i++){
        for(int j=0; j<WIDTH; j++){
            wprintf(L"%lc", screen[i][j]);
        }
        wprintf(L"\n");
    }
    sleep_for(1500);
}

void generate_stars(PtlVector *vec, int number){
    
    int animTemp[8];
    animTemp[0]=0x00AD; // 
    animTemp[1]=0x00B7; // · 
    animTemp[2]=0xFE61; // ﹡
    animTemp[3]=0x002A; // *
    animTemp[4]=0x2727; // ✧
    animTemp[5]=0x2726; // ✦­
    animTemp[6]=0xFE61; // ﹡
    animTemp[7]=0x00B7; // · 
    for(int i=0; i<number; i++){
        struct s_star star;
        star.pos.x = rand() % WIDTH;
        star.pos.y = rand() % HEIGHT;
        for(int j=0; j<8; j++){
            star.animation[j] = animTemp[j];
        }
        star.numOfFrames = 8;
        star.curAnimFrame = rand() % star.numOfFrames;

        ptl_vector_add(vec, star);
    }
}

void update_stars(PtlVector *vec, int number)
{
    for(int i=0; i<number; i++){
        struct s_star star = ptl_vector_get(struct s_star, vec, i);
        star.pos.x = star.pos.x + (rand() % 2);
        star.pos.x = star.pos.x - (rand() % 2);
        star.pos.y = star.pos.y + (rand() % 2);
        star.pos.y = star.pos.y - (rand() % 2);
        
        star.pos.x = (star.pos.x > 0) ? star.pos.x : -star.pos.x;
        star.pos.y = (star.pos.y > 0) ? star.pos.y : -star.pos.y;  
        
        star.pos.x = star.pos.x % WIDTH;
        star.pos.y = star.pos.y % HEIGHT; 

        star.curAnimFrame = (star.curAnimFrame+1)%star.numOfFrames; 
        ptl_vector_set(vec, i, star);
    }
}

int main()
{
    setlocale(LC_CTYPE, "");
    int sky[HEIGHT+3][WIDTH+3];

    for(int i=0; i<HEIGHT*5; i++){
        for(int j=0; j<WIDTH*5; j++){
            wprintf(L" ");
        }
    }

    int numOfStars = HEIGHT+WIDTH;
    PtlVector vec_of_stars;
    ptl_vector_init(struct s_star, &vec_of_stars);

    generate_stars(&vec_of_stars, numOfStars);

    // wprintf(L"%lc\n", x.animation[7]);
    while(1){
        draw_sky(sky, &vec_of_stars);
        update_stars(&vec_of_stars, numOfStars);
    }
}

// TODO:
// - change int for storing UNICODE to wide char or unsigned short or any other 2byte variable
// - check how the code compiles and runs on windows
// - make it compatible and equivalent with windows
// - find the memory leaks
