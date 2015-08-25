// Beginning Game Programming, Second Edition
// Chapter 10
// Paddle_Game program header file


#ifndef _GAME_H
#define _GAME_H 1

//windows/directx headers
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>
#include <dsound.h>
#include <dinput.h>
#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

//framework-specific headers
#include "dxgraphics.h"
#include "dxaudio.h"
#include "dxinput.h"

//application title
#define APPTITLE "Zombie Crysis: The Escape"

//screen setup
#define FULLSCREEN 0      //0 = windowed, 1 = fullscreen
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

//function prototypes
int Game_Init(HWND);
void Game_Run(HWND);
void Game_End(HWND);
void TitleScreen();
void CreditScreen();
void player_shoot();
void player_bomb();
int find_bullet();
void boss_shoot();
int find_boss_bullet();



//sprite structure
typedef struct {
	bool alive,effect;
    float x,y;
	float release;
    int width,height;
    int movex,movey;
    int curframe,lastframe;
    int animdelay,animcount;
    int scalex, scaley;
    int rotation, rotaterate;
	int health;
	float bulletDirection;

    void reset()
    {
        x = (float)(rand() % (SCREEN_WIDTH-128));
        y = 0;
    }

	void releaseE()
	{
		release = (float) (rand() % 2500) + 1;
	}
} SPRITE;


LPD3DXFONT MakeFont(string name, int size);
void FontPrint(LPD3DXFONT font, int x, int y, string text, D3DCOLOR color = D3DCOLOR_XRGB(255,255,255));



#endif

