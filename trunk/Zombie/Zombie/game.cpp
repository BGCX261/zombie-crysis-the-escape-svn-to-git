#include "game.h"

LPDIRECT3DSURFACE9 titlescreen;
LPDIRECT3DSURFACE9 creditscreen;

//background images
LPDIRECT3DSURFACE9 levelback1_image;
LPDIRECT3DSURFACE9 levelback2_image;
LPDIRECT3DSURFACE9 levelback3_image;


const int BUFFERW = SCREEN_WIDTH;
const int BUFFERH = SCREEN_HEIGHT * 4;
double scrollx = 0;
double scrolly = 0;
int selectMenu = 0;
const double virtual_level_size = BUFFERH * 5;
double virtual_scrolly = 0;

//transition images
LPDIRECT3DSURFACE9 story;
LPDIRECT3DSURFACE9 leveltran1;
LPDIRECT3DSURFACE9 leveltran2;
LPDIRECT3DSURFACE9 leveltran3;
LPDIRECT3DSURFACE9 winning;
LPDIRECT3DSURFACE9 options;
LPDIRECT3DSURFACE9 losing;

LPDIRECT3DTEXTURE9 arrow_image;
SPRITE arrow;

//sprite handler
LPD3DXSPRITE sprite_handler;

//character sprites
LPDIRECT3DTEXTURE9 level1char_image;
LPDIRECT3DTEXTURE9 level2char_image;
LPDIRECT3DTEXTURE9 level3char_image;
LPDIRECT3DTEXTURE9 playerHealth_image;
SPRITE level1char;
SPRITE level2char;
SPRITE level3char;
SPRITE playerHealth;

//enemy sprites
LPDIRECT3DTEXTURE9 level1enemy_image;
LPDIRECT3DTEXTURE9 level2enemy_image;
LPDIRECT3DTEXTURE9 level3enemy_image;
const int ENEMIES = 20;
SPRITE enemy1[ENEMIES];
SPRITE enemy2[ENEMIES];
SPRITE enemy3[ENEMIES];

LPDIRECT3DTEXTURE9 level1blood_image;
SPRITE blood1;
LPDIRECT3DTEXTURE9 level2blood_image;
SPRITE blood2;
LPDIRECT3DTEXTURE9 level3blood_image;
SPRITE blood3;

LPDIRECT3DTEXTURE9 explosion1_image;
SPRITE explosion1;
LPDIRECT3DTEXTURE9 explosion2_image;
SPRITE explosion2;
LPDIRECT3DTEXTURE9 explosion3_image;
SPRITE explosion3;

LPDIRECT3DTEXTURE9 boss1_image;
SPRITE boss1;
LPDIRECT3DTEXTURE9 boss2_image;
SPRITE boss2;
LPDIRECT3DTEXTURE9 boss3_image;
SPRITE boss3;

//bullets
LPDIRECT3DTEXTURE9 level1bullet_image;
LPDIRECT3DTEXTURE9 level2bullet_image;
LPDIRECT3DTEXTURE9 level3bullet_image;
LPDIRECT3DTEXTURE9 boss1_bullet_image;
LPDIRECT3DTEXTURE9 boss2_bullet1_image;
LPDIRECT3DTEXTURE9 boss3_bullet_image;
LPDIRECT3DTEXTURE9 boss2_bullet2_image;
const int BULLETS = 100;
SPRITE bullets1[BULLETS];
SPRITE bullets2[BULLETS];
SPRITE bullets3[BULLETS];
SPRITE boss1_bullet[BULLETS];
SPRITE boss2_bullet1[BULLETS];
SPRITE boss2_bullet2[BULLETS];
SPRITE boss3_bullet[BULLETS];

LPDIRECT3DTEXTURE9 bomb1_image;
SPRITE bomb1;
LPDIRECT3DTEXTURE9 bomb2_image;
SPRITE bomb2;
LPDIRECT3DTEXTURE9 bomb3_image;
SPRITE bomb3;

LPDIRECT3DTEXTURE9 bomb1_display_image;
SPRITE bomb1Pic;
LPDIRECT3DTEXTURE9 bomb2_display_image;
SPRITE bomb2Pic;
LPDIRECT3DTEXTURE9 bomb3_display_image;
SPRITE bomb3Pic;

int player_shoot_timer = 0;
int player_bomb_timer = 0;
int boss_shoot_timer1 = 0;
int boss_shoot_timer2 = 0;
int boss_shoot_timer3 = 0;
int firepower = 1;
int bullet1count = 0;
int bullet2count = 0;
int bullet3count = 0;
int boss1_bulletcount = 0;
int boss2_bullet1count = 0;
int boss2_bullet2count = 0;
int boss3_bulletcount = 0;
int direction = 1;
int bomb1Count = 0;
int bomb2Count = 0;
int bomb3Count = 0;


//sounds
CSound *sound_bullet1shoot = NULL;
CSound *sound_bullet2shoot = NULL;
CSound *sound_bullet3shoot = NULL;
CSound *sound_boss1shoot = NULL;
CSound *sound_boss2shoot = NULL;
CSound *sound_boss3shoot = NULL;
CSound *sound_hit = NULL;
CSound *sound_background1 = NULL;
CSound *sound_background2 = NULL;
CSound *sound_background3 = NULL;

LPD3DXFONT score;
int playerScore = 0;

//misc
long start = GetTickCount();
HRESULT result;

int levelTrack = 7;

float enemycheck = 2499;
int enemycountmax = 5;
int enemycount = 0;

int increment1 = 0;
int increment2 = 0;

int LEVEL1_WIDTH = 640;
int LEVEL1_HEIGHT = 1920;
int LEVEL2_WIDTH = 640;
int LEVEL2_HEIGHT = 1920;
int LEVEL3_WIDTH = 640;
int LEVEL3_HEIGHT = 1920;

bool keyupReturn = true;
bool keyupUp = true;
bool keyupDown = true;
bool keyupQ = true;
int lastKey;

int boss2SpawnCheck = 0;

LPD3DXFONT MakeFont(string name, int size)
{
	LPD3DXFONT font = NULL;

	D3DXFONT_DESC desc = {
		size,                   //height
		0,                      //width
		0,                      //weight
		0,                      //miplevels
		false,                  //italic
		DEFAULT_CHARSET,        //charset
		OUT_TT_PRECIS,          //output precision
		CLIP_DEFAULT_PRECIS,    //quality
		DEFAULT_PITCH,          //pitch and family
		""                      //font name
	};

	strcpy(desc.FaceName, name.c_str());

	D3DXCreateFontIndirect(d3ddev, &desc, &font);

	return font;
}

template <class T>
std::string static ToString(const T & t, int places = 2)
{
	ostringstream oss;
	oss.precision(places);
	oss.setf(ios_base::fixed);
	oss << t; 
	return oss.str();
}
//initializes the game
int Game_Init(HWND hwnd)
{
	//set random number seed
	srand((unsigned float)time(NULL));

	//initialize mouse
	if (!Init_Mouse(hwnd))
	{
		MessageBox(hwnd, "Error initializing the mouse", "Error", MB_OK);
		return 0;
	}

	//initialize keyboard
	if (!Init_Keyboard(hwnd))
	{
		MessageBox(hwnd, "Error initializing the keyboard", "Error", MB_OK);
		return 0;
	}

	//create sprite handler object
	result = D3DXCreateSprite(d3ddev, &sprite_handler);
	if (result != D3D_OK)
		return 0;

	//load title screen
	titlescreen = LoadSurface("titlescreen.bmp", NULL);
	if (titlescreen == NULL)
		return 0;

	//load credits screen
	creditscreen = LoadSurface("creditscreen.bmp", NULL);
	if (creditscreen == NULL)
		return 0;

	story = LoadSurface("story.bmp", NULL);
	if (story == NULL)
		return 0;

	winning = LoadSurface("winning.bmp", NULL);
	if (winning == NULL)
		return 0;

	options = LoadSurface("option.png", NULL);
	if (options == NULL)
		return 0;

	losing = LoadSurface("losing.bmp", NULL);
	if (losing == NULL)
		return 0;

	arrow_image = LoadTexture("arrow.bmp", D3DCOLOR_XRGB(255,255,255));
	if (arrow_image == NULL)
		return 0;

	score = MakeFont("Arial Bold", 24);

	//load the background images
	levelback1_image = LoadSurface("background1.bmp", NULL);
	if (levelback1_image == NULL)
		return 0;
	levelback2_image = LoadSurface("background2.bmp", NULL);
	if (levelback2_image == NULL)
		return 0;
	levelback3_image = LoadSurface("background3.png", NULL);
	if (levelback3_image == NULL)
		return 0;



	//load the transition images
	leveltran1 = LoadSurface("transition1.bmp", NULL);
	if (leveltran1 == NULL)
		return 0;
	leveltran2 = LoadSurface("transition2.bmp", NULL);
	if (leveltran2 == NULL)
		return 0;
	leveltran3 = LoadSurface("transition3.bmp", NULL);
	if (leveltran3 == NULL)
		return 0;

	//load the characters
	level1char_image = LoadTexture("character1.bmp", D3DCOLOR_XRGB(255,255,255));
	if (level1char_image == NULL)
		return 0;
	level2char_image = LoadTexture("character2.bmp", D3DCOLOR_XRGB(255,255,255));
	if (level2char_image == NULL)
		return 0;
	level3char_image = LoadTexture("character3.png", D3DCOLOR_XRGB(255,255,255));
	if (level3char_image == NULL)
		return 0;

	playerHealth_image = LoadTexture("health sprite.png", D3DCOLOR_XRGB(255,255,255));
	if (playerHealth_image == NULL)
		return 0;

	arrow.x = 15;
	arrow.y = 160;
	arrow.width = 35;
	arrow.height = 35;

	//set the characters properties
	level1char.x = 320;
	level1char.y = 350;
	level1char.width = 70;
	level1char.height = 50;
	level1char.movex = 5;
	level1char.movey = 5;
	level1char.curframe = 0;
	level1char.lastframe = 3;
	level1char.animdelay = 3;
	level1char.animcount = 0;

	level2char.x = 400;
	level2char.y = 300;
	level2char.width = 50;
	level2char.height = 96;
	level2char.movex = 5;
	level2char.movey = 5;
	/*level1char.curframe = 0;
	level1char.lastframe = 3;
	level1char.animdelay = 3;
	level1char.animcount = 0;*/

	level3char.x = 400;
	level3char.y = 300;
	level3char.width = 95;
	level3char.height = 95;
	level3char.movex = 5;
	level3char.movey = 5;

	playerHealth.x = 0;
	playerHealth.y = 435;
	playerHealth.width = 240;
	playerHealth.height = 40;
	playerHealth.curframe = 0;
	playerHealth.lastframe = 4;
	/*playerHealth.animdelay = 3;
	playerHealth.animcount = 0;*/


	//load the enemies sprites
	level1enemy_image = LoadTexture("enemy1.bmp", D3DCOLOR_XRGB(255,255,255));
	if (level1enemy_image == NULL)
		return 0;
	level2enemy_image = LoadTexture("enemy2.png", D3DCOLOR_XRGB(255,255,255));
	if (level2enemy_image == NULL)
		return 0;
	level3enemy_image = LoadTexture("enemy3.png", D3DCOLOR_XRGB(255,255,255));
	if (level3enemy_image == NULL)
		return 0;

	//load the explosion sprites
	level1blood_image = LoadTexture("blood1.png", D3DCOLOR_XRGB(0,128,192));
	if (level1blood_image == NULL)
		return 0;

	level2blood_image = LoadTexture("blood2.png", D3DCOLOR_XRGB(0,128,192));
	if (level2blood_image == NULL)
		return 0;

	level3blood_image = LoadTexture("blood3.png", D3DCOLOR_XRGB(0,128,192));
	if (level3blood_image == NULL)
		return 0;

	//load the explosion sprites
	explosion1_image = LoadTexture("explosion1.png", D3DCOLOR_XRGB(0,248,0));
	if (explosion1_image == NULL)
		return 0;

	explosion2_image = LoadTexture("explosion2.png", D3DCOLOR_XRGB(0,248,0));
	if (explosion2_image == NULL)
		return 0;

	explosion3_image = LoadTexture("explosion3.png", D3DCOLOR_XRGB(0,248,0));
	if (explosion3_image == NULL)
		return 0;

	//set explosion properties

	blood1.alive = false;
	blood1.x = 0;
	blood1.y = 0;
	blood1.width = 35;
	blood1.height = 49;
	/*blood1[n].movey = 2;*/
	blood1.curframe = 0;
	blood1.lastframe = 7;
	blood1.animdelay = 2;
	blood1.animcount = 0;

	blood2.alive = false;
	blood2.x = 0;
	blood2.y = 0;
	blood2.width = 35;
	blood2.height = 49;
	/*blood2[n].movey = 2;*/
	blood2.curframe = 0;
	blood2.lastframe = 7;
	blood2.animdelay = 2;
	blood2.animcount = 0;

	blood3.alive = false;
	blood3.x = 0;
	blood3.y = 0;
	blood3.width = 35;
	blood3.height = 49;
	/*blood3[n].movey = 2;*/
	blood3.curframe = 0;
	blood3.lastframe = 7;
	blood3.animdelay = 2;
	blood3.animcount = 0;

	explosion1.alive = false;
	explosion1.x = 0;
	explosion1.y = 0;
	explosion1.width = 55;
	explosion1.height = 110;
	/*explosion1[n].movey = 2;*/
	explosion1.curframe = 0;
	explosion1.lastframe = 26;
	explosion1.animdelay = 1;
	explosion1.animcount = 0;
	explosion1.effect = false;

	explosion2.alive = false;
	explosion2.x = 0;
	explosion2.y = 0;
	explosion2.width = 55;
	explosion2.height = 110;
	/*explosion2[n].movey = 2;*/
	explosion2.curframe = 0;
	explosion2.lastframe = 26;
	explosion2.animdelay = 1;
	explosion2.animcount = 0;
	explosion2.effect = false;

	explosion3.alive = false;
	explosion3.x = 0;
	explosion3.y = 0;
	explosion3.width = 55;
	explosion3.height = 110;
	/*explosion3[n].movey = 2;*/
	explosion3.curframe = 0;
	explosion3.lastframe = 26;
	explosion3.animdelay = 1;
	explosion3.animcount = 0;
	explosion3.effect = false;


	//set enemy properties
	for (int n = 0;n < ENEMIES; n++)
	{
		enemy1[n].alive = false;
		/*enemy1[n].x = 300;
		enemy1[n].y = SCREEN_HEIGHT - 50;*/
		enemy1[n].width = 70;
		enemy1[n].height = 70;
		enemy1[n].movey = 2;
		enemy1[n].curframe = 0;
		enemy1[n].lastframe = 3;
		enemy1[n].animdelay = 3;
		enemy1[n].animcount = 0;
		enemy1[n].reset();
		enemy1[n].releaseE();
	}
	for (int n = 0; n < ENEMIES; n++)
	{
		enemy2[n].alive = false;
		/*enemy2[n].x = 300;
		enemy2[n].y = SCREEN_HEIGHT - 50;*/
		enemy2[n].width = 65;
		enemy2[n].height = 115;
		enemy2[n].movey = 1;
		enemy2[n].curframe = 0;
		enemy2[n].lastframe = 9;
		enemy2[n].animdelay = 3;
		enemy2[n].animcount = 0;
		enemy2[n].reset();
		enemy2[n].releaseE();
		/*enemy2[n].reset();*/
	}
	for (int n = 0; n < ENEMIES; n++)
	{
		enemy3[n].alive = false;
		/*enemy3[n].x = 300;
		enemy3[n].y = SCREEN_HEIGHT - 50;*/
		enemy3[n].width = 56;
		enemy3[n].height = 57;
		enemy3[n].movey = 2;
		enemy3[n].reset();
		enemy3[n].releaseE();
		/*enemy3[n].reset();*/
	}

	//load the enemies sprites
	boss1_image = LoadTexture("boss1.png", D3DCOLOR_XRGB(255,0,255));
	if (boss1_image == NULL)
		return 0;

	boss2_image = LoadTexture("boss2.png", D3DCOLOR_XRGB(255,255,255));
	if (boss2_image == NULL)
		return 0;

	boss3_image = LoadTexture("boss3.png", D3DCOLOR_XRGB(255,255,255));
	if (boss3_image == NULL)
		return 0;

	//load bomb images
	bomb1_image = LoadTexture("bomb1.png", D3DCOLOR_XRGB(255,255,255));
	if (bomb1_image == NULL)
		return 0;

	bomb2_image = LoadTexture("bomb2.png", D3DCOLOR_XRGB(255,255,255));
	if (bomb2_image == NULL)
		return 0;

	bomb3_image = LoadTexture("bomb3.png", D3DCOLOR_XRGB(255,255,255));
	if (bomb3_image == NULL)
		return 0;

	//load bomb Display images
	bomb1_display_image = LoadTexture("bomb1Pic.png", D3DCOLOR_XRGB(255,255,255));
	if (bomb1_display_image == NULL)
		return 0;

	bomb2_display_image = LoadTexture("bomb2Pic.png", D3DCOLOR_XRGB(255,255,255));
	if (bomb2_display_image == NULL)
		return 0;

	bomb3_display_image = LoadTexture("bomb3Pic.png", D3DCOLOR_XRGB(255,255,255));
	if (bomb3_display_image == NULL)
		return 0;

	bomb1.alive = false;
	bomb1.x = SCREEN_WIDTH / 2 - 100;
	bomb1.y = 0;
	bomb1.width = 224;
	bomb1.height = 224;
	bomb1.movey = 2;	

	bomb2.alive = false;
	bomb2.x = SCREEN_WIDTH / 2 - 100;
	bomb2.y = 0;
	bomb2.width = 224;
	bomb2.height = 224;
	bomb2.movey = 2;

	bomb3.alive = false;
	bomb3.x = SCREEN_WIDTH / 2 - 100;
	bomb3.y = 0;
	bomb3.width = 224;
	bomb3.height = 224;
	bomb3.movey = 2;

	bomb1Pic.x = SCREEN_WIDTH / 2 - 30;
	bomb1Pic.y = 425;
	bomb1Pic.width = 141;
	bomb1Pic.height = 48;
	bomb1Pic.curframe = 0;
	bomb1Pic.lastframe = 2;

	bomb2Pic.x = SCREEN_WIDTH / 2 - 30;
	bomb2Pic.y = 425;
	bomb2Pic.width = 141;
	bomb2Pic.height = 48;
	bomb2Pic.curframe = 0;
	bomb2Pic.lastframe = 2;

	bomb3Pic.x = SCREEN_WIDTH / 2 - 30;
	bomb3Pic.y = 425;
	bomb3Pic.width = 141;
	bomb3Pic.height = 48;
	bomb3Pic.curframe = 0;
	bomb3Pic.lastframe = 2;

	boss1.alive = false;
	boss1.x = SCREEN_WIDTH / 2 - 100;
	boss1.y = 0;
	boss1.width = 137;
	boss1.height = 74;
	boss1.movey = 2;
	boss1.movex = 2;
	boss1.health = 20;
	boss1.curframe = 0;
	boss1.lastframe = 9;
	boss1.animdelay = 3;
	boss1.animcount = 0;

	boss2.alive = false;
	boss2.x = SCREEN_WIDTH / 2 - 100;
	boss2.y = 0;
	boss2.width = 133;
	boss2.height = 126;
	boss2.movey = 2;
	boss2.movex = 2;
	boss2.health = 20;

	boss3.alive = false;
	boss3.x = SCREEN_WIDTH / 2 - 100;
	boss3.y = 0;
	boss3.width = 137;
	boss3.height = 71;
	boss3.movey = 2;
	boss3.movex = 2;
	boss3.health = 20;	



	//load bullet images
	level1bullet_image = LoadTexture("bullet1.bmp", D3DCOLOR_XRGB(255,0,255));
	if (level1bullet_image == NULL)
		return 0;
	level2bullet_image = LoadTexture("bullet2.png", D3DCOLOR_XRGB(0,248,0));
	if (level2bullet_image == NULL)
		return 0;
	level3bullet_image = LoadTexture("bullet3.png", D3DCOLOR_XRGB(255,255,255));
	if (level3bullet_image == NULL)
		return 0;

	boss1_bullet_image = LoadTexture("boss_bullet1.bmp", D3DCOLOR_XRGB(255,0,255));
	if (boss1_bullet_image == NULL)
		return 0;

	boss2_bullet1_image = LoadTexture("boss_bullet2.png", D3DCOLOR_XRGB(255,255,255));
	if (boss2_bullet1_image == NULL)
		return 0;

	boss2_bullet2_image = LoadTexture("boss_bullet2.png", D3DCOLOR_XRGB(255,255,255));
	if (boss2_bullet2_image == NULL)
		return 0;

	boss3_bullet_image = LoadTexture("boss_bullet3.png", D3DCOLOR_XRGB(255,255,255));
	if (boss3_bullet_image == NULL)
		return 0;

	//set bullet properties
	for (int n=0; n<BULLETS; n++)
	{
		bullets1[n].alive = false;
		bullets1[n].x = 0;
		bullets1[n].y = 0;
		bullets1[n].width = 5;
		bullets1[n].height = 7;
		bullets1[n].movey = 5;
	}
	for (int n=0; n<BULLETS; n++)
	{
		bullets2[n].alive = false;
		bullets2[n].x = 0;
		bullets2[n].y = 0;
		bullets2[n].width = 16;
		bullets2[n].height = 15;
		bullets2[n].movey = 5;
		bullets2[n].curframe = 0;
		bullets2[n].lastframe = 1;
		bullets2[n].animdelay = 3;
		bullets2[n].animcount = 0;
	}
	for (int n=0; n<BULLETS; n++)
	{
		bullets3[n].alive = false;
		bullets3[n].x = 0;
		bullets3[n].y = 0;
		bullets3[n].width = 10;
		bullets3[n].height = 16;
		bullets3[n].movey = 5;
	}	

	for (int n=0; n<BULLETS; n++)
	{
		boss1_bullet[n].alive = false;
		boss1_bullet[n].x = 0;
		boss1_bullet[n].y = 0;
		boss1_bullet[n].width = 17;
		boss1_bullet[n].height = 19;
		boss1_bullet[n].movey = 5;

		boss1_bullet[n].bulletDirection = ((rand() % 5) * -1) + (rand() % 5);
	}

	for (int n=0; n<BULLETS; n++)
	{
		boss2_bullet1[n].alive = false;
		boss2_bullet1[n].x = 0;
		boss2_bullet1[n].y = 0;
		boss2_bullet1[n].width = 16;
		boss2_bullet1[n].height = 16;
		boss2_bullet1[n].movey = 5;
		boss2_bullet1[n].bulletDirection = ((rand() % 5) * -1) + (rand() % 5);
		boss2_bullet1[n].curframe = 0;
		boss2_bullet1[n].lastframe = 6;
		boss2_bullet1[n].animdelay = 2;
		boss2_bullet1[n].animcount = 0;
	}

	for (int n=0; n<BULLETS; n++)
	{
		boss2_bullet2[n].alive = false;
		boss2_bullet2[n].x = 0;
		boss2_bullet2[n].y = 0;
		boss2_bullet2[n].width = 16;
		boss2_bullet2[n].height = 16;
		boss2_bullet2[n].movey = 5;
		boss2_bullet2[n].bulletDirection = ((rand() % 5) * -1) + (rand() % 5);
		boss2_bullet2[n].curframe = 0;
		boss2_bullet2[n].lastframe = 6;
		boss2_bullet2[n].animdelay = 2;
		boss2_bullet2[n].animcount = 0;
	}

	for (int n=0; n<BULLETS; n++)
	{
		boss3_bullet[n].alive = false;
		boss3_bullet[n].x = 0;
		boss3_bullet[n].y = 0;
		boss3_bullet[n].width = 15;
		boss3_bullet[n].height = 26;
		boss3_bullet[n].movey = 5;
		boss3_bullet[n].bulletDirection = ((rand() % 5) * -1) + (rand() % 5);
	}

	//load shoot wave files
	sound_bullet1shoot = LoadSound("bullet1_sound.wav");
	if (sound_bullet1shoot == NULL)
		return 0;

	sound_bullet2shoot = LoadSound("bullet2_sound.wav");
	if (sound_bullet2shoot == NULL)
		return 0;

	sound_bullet3shoot = LoadSound("bullet3_sound.wav");
	if (sound_bullet3shoot == NULL)
		return 0;

	sound_boss1shoot = LoadSound("boss_bullet_sound.wav");
	if (sound_boss1shoot == NULL)
		return 0;

	sound_boss2shoot = LoadSound("boss_bullet_sound.wav");
	if (sound_boss2shoot == NULL)
		return 0;

	sound_boss3shoot = LoadSound("boss_bullet_sound.wav");
	if (sound_boss3shoot == NULL)
		return 0;


	//load the hit wave file
	sound_hit = LoadSound("hit.wav");
	if (sound_hit == NULL)
		return 0;

	//load the background music
	sound_background1 = LoadSound("level1_music.wav");
	if (sound_background1 == NULL)
		return 0;

	//load the hit wave file
	sound_background2 = LoadSound("level2_music.wav");
	if (sound_background2 == NULL)
		return 0;

	//load the hit wave file
	sound_background3 = LoadSound("level3_music.wav");
	if (sound_background3 == NULL)
		return 0;

	//return okay
	return 1;
}

void FontPrint(LPD3DXFONT font, int x, int y, string text, D3DCOLOR color)
{
	//figure out the text boundary
	RECT rect = { x, y, 0, 0 };
	font->DrawText( NULL, text.c_str(), text.length(), &rect, DT_CALCRECT, color); 

	//print the text
	font->DrawText(sprite_handler, text.c_str(), text.length(), &rect, DT_LEFT, color); 
}

int Collision(SPRITE sprite1, SPRITE sprite2)
{
	RECT rect1;
	rect1.left = sprite1.x+1;
	rect1.top = sprite1.y+1;
	rect1.right = sprite1.x + sprite1.width-1;
	rect1.bottom = sprite1.y + sprite1.height-1;

	RECT rect2;
	rect2.left = sprite2.x+1;
	rect2.top = sprite2.y+1;
	rect2.right = sprite2.x + sprite2.width-1;
	rect2.bottom = sprite2.y + sprite2.height-1;

	RECT dest;
	return IntersectRect(&dest, &rect1, &rect2);
}

//the main game loop
void Game_Run(HWND hwnd)
{

	//ball position vector
	D3DXVECTOR3 position(0,0,0);

	//make sure the Direct3D device is valid
	if (d3ddev == NULL)
		return;

	//update mouse and keyboard
	Poll_Mouse();
	Poll_Keyboard();

	//after short delay, ready for next frame?
	//this keeps the game running at a steady frame rate
	if (GetTickCount() - start >= 30)
	{
		//reset timing
		start = GetTickCount();


		switch(levelTrack)
		{
		case 1:
			{
				PlaySound(sound_background1);

				if(increment1 > 4)
				{
					levelTrack = 12;
					StopSound(sound_background1);
					break;
				}

				//Keep character in bounds
				if (level1char.x > SCREEN_WIDTH - level1char.width)
				{
					level1char.x -= level1char.movex;
				}
				if (level1char.x < 0)
				{
					level1char.x += level1char.movex;
				}
				if (level1char.y > SCREEN_HEIGHT - level1char.height)
				{
					level1char.y -= level1char.movey;
				}
				if (level1char.y < (SCREEN_WIDTH / 2) - level1char.height)
				{
					level1char.y += level1char.movey;
				}

				//move the character
				if (Key_Down(DIK_LEFT))
					level1char.x -= level1char.movex;
				if (Key_Down(DIK_RIGHT))
					level1char.x += level1char.movex;
				if (Key_Down(DIK_UP))
					level1char.y -= level1char.movey;
				if (Key_Down(DIK_DOWN))
					level1char.y += level1char.movey;
				if (Key_Down(DIK_A))
					level1char.x -= level1char.movex;
				if (Key_Down(DIK_D))
					level1char.x += level1char.movex;
				if (Key_Down(DIK_W))
					level1char.y -= level1char.movey;
				if (Key_Down(DIK_S))
					level1char.y += level1char.movey;



				if (++level1char.animcount > level1char.animdelay)
				{
					//reset counter
					level1char.animcount = 0;

					//animate the sprite
					if (++level1char.curframe > level1char.lastframe)
						level1char.curframe = 0;
				}

				for (int n = 0;n < ENEMIES; n++)
				{
					if (++enemy1[n].animcount > enemy1[n].animdelay)
					{
						//reset counter
						enemy1[n].animcount = 0;

						//animate the sprite
						if (++enemy1[n].curframe > enemy1[n].lastframe)
							enemy1[n].curframe = 0;
					}
				}

				//shooting
				if (Key_Down(DIK_SPACE))
					player_shoot();

				//shooting bomb
				if (Key_Down(DIK_V))
					player_bomb();



				if(boss1.alive)
				{
					if (++boss1.animcount > boss1.animdelay)
					{
						boss1.animcount = 0;

						if (++boss1.curframe > boss1.lastframe)
						{
							boss1.curframe = 0;
						}
					}

					boss_shoot();
				}

				scrolly += 2.0;
				if (scrolly < 0)
					scrolly = 0;
				if (scrolly > BUFFERH - SCREEN_HEIGHT)
					scrolly = BUFFERH - SCREEN_HEIGHT;

				//update virtual scroll position
				virtual_scrolly -= 1.0;
				/*if (virtual_scrolly > virtual_level_size)
				virtual_scrolly = 0.0;*/

				//check bullet Collision
				for (int n=0; n<BULLETS; n++)
				{
					for (int m=0; m<ENEMIES; m++)
					{
						if (bullets1[n].alive == true && enemy1[m].alive == true)
						{
							if (Collision(bullets1[n], enemy1[m]))
							{
								enemy1[m].alive = false;
								blood1.alive = true;
								blood1.x = enemy1[m].x;
								blood1.y = enemy1[m].y;
								enemy1[m].reset();
								bullets1[n].alive = false;
								enemy1[m].releaseE();
								enemycount--;
								playerScore++;							
							}					
						}					
					}
				}

				//Bomb & Explosion Animation & Collision
				if(bomb1.alive)
				{
					bomb1.y -= bomb1.movey;

					if(bomb1.y < level1char.y - 200)
					{
						bomb1.alive = false;
						explosion1.alive = true;
						explosion1.effect = true;
						explosion1.x = bomb1.x;
						explosion1.y = bomb1.y - 110;
					}	
					for (int n = 0; n < ENEMIES; n++)
					{
						if(Collision(bomb1, enemy1[n]))
						{
							bomb1.alive = false;
							explosion1.alive = true;
							explosion1.effect = true;
							explosion1.x = bomb1.x;
							explosion1.y = bomb1.y - 110;
						}	
					}
				}

				if(explosion1.alive)
				{


					for (int n=0; n<ENEMIES; n++)
					{
						if (Collision(explosion1, enemy1[n]))
						{
							enemy1[n].alive = false;
						}
					}


					if (Collision(explosion1, boss1) && explosion1.effect)
					{
						boss1.health -= 5;

						if(boss1.health <= 0)
						{
							boss1.alive = false;
						}


						if(boss1.health <= 0)
						{
							boss1.alive = false;
						}

						explosion1.effect = false;
					}


					if (++explosion1.animcount > explosion1.animdelay)
					{
						explosion1.animcount = 0;

						if (++explosion1.curframe > explosion1.lastframe)
						{
							explosion1.curframe = 0;
							explosion1.alive = false;
						}
					}
				}



				if(blood1.alive == true)
				{
					/*if(++blood1.animcount > blood1.animdelay)
					{
					blood1.animcount = 0;*/

					++blood1.curframe;

					if(blood1.curframe >= 24)
					{
						blood1.alive = false;
						blood1.curframe = 0;
					}
					/*}*/
				}

				for (int n=0; n<BULLETS; n++)
				{

					if (boss1_bullet[n].alive == true)
					{
						if (Collision(boss1_bullet[n], level1char))
						{
							boss1_bullet[n].alive = false;
							increment1++;
						}					
					}					

				}

				for (int n=0; n<BULLETS; n++)
				{
					if(bullets1[n].alive == true && boss1.alive == true)
					{

						if (Collision(bullets1[n], boss1))
						{
							bullets1[n].alive = false;
							if(boss1.health <= 0)
							{
								boss1.alive = false;
								playerScore += 50;
								StopSound(sound_background1);
							}
							else
							{
								boss1.health--;
							}
						}
					}
				}

				//check enemy @ bottom of screen
				for (int n=0; n<ENEMIES; n++)
				{
					if (enemy1[n].y > SCREEN_HEIGHT + enemy1[n].height)
					{
						enemy1[n].alive = false;
						enemy1[n].reset();
						enemy1[n].releaseE();
						enemycount--;
						playerScore--;
					}
				}

				//reset zombie
				for (int n=0; n<ENEMIES; n++)
				{
					if (enemy1[n].alive == false)
					{
						enemy1[n].reset();
						enemy1[n].releaseE();
					}
				}


				//check enemy vs player
				for (int n=0; n<ENEMIES; n++)
				{
					if (Collision(level1char, enemy1[n]))
					{
						enemy1[n].alive = false;
						enemy1[n].reset();
						enemy1[n].releaseE();
						enemycount--;
						increment1++;
					}
				}

				bullet1count = 0;
				for (int n=0; n<BULLETS; n++)
				{
					if (bullets1[n].alive) 
					{
						bullet1count++;
						bullets1[n].y -= bullets1[n].movey;

						if (bullets1[n].x < 0 || bullets1[n].x > SCREEN_WIDTH
							|| bullets1[n].y < 0 || bullets1[n].y > SCREEN_HEIGHT)
							bullets1[n].alive = false;
					}
				}

				boss1_bulletcount = 0;
				for (int n=0; n<BULLETS; n++)
				{
					if (boss1_bullet[n].alive) 
					{
						boss1_bulletcount++;
						boss1_bullet[n].y += boss1_bullet[n].movey;
						boss1_bullet[n].x += boss1_bullet[n].bulletDirection;

						if (boss1_bullet[n].x < 0 || boss1_bullet[n].x > SCREEN_WIDTH
							|| boss1_bullet[n].y < 0 || boss1_bullet[n].y > SCREEN_HEIGHT)
							boss1_bullet[n].alive = false;
					}				}		




				if(boss1.alive)
				{
					if(boss1.y <= SCREEN_HEIGHT / 2 - 200)
						boss1.y += boss1.movey;

					if(boss1.x > SCREEN_WIDTH - boss1.width && direction == 1)
					{
						direction = 2;
					}
					else if (boss1.x < 1 && direction == 2)
					{
						direction = 1;
					}

					if (direction == 1)
						boss1.x += boss1.movex;
					else if (direction == 2)
						boss1.x -= boss1.movex;
				}



				for (int n=0; n<ENEMIES; n++)
				{
					if (enemy1[n].alive) 
					{
						//move horiz based on x velocity
						enemy1[n].y += enemy1[n].movey;
					}
				}


				break;
			}
		case 2:			
			{
				PlaySound (sound_background2);

				if(increment1 > 4)
				{
					levelTrack = 12;
					StopSound(sound_background2);
					break;
				}

				//Keep character in bounds
				if (level2char.x > SCREEN_WIDTH - level2char.width)
				{
					level2char.x -= level2char.movex;
				}
				if (level2char.x < 0)
				{
					level2char.x += level2char.movex;
				}
				if (level2char.y > SCREEN_HEIGHT - level2char.height)
				{
					level2char.y -= level2char.movey;
				}
				if (level2char.y < (SCREEN_WIDTH / 2) - level2char.height)
				{
					level2char.y += level2char.movey;
				}

				//move the character
				if (Key_Down(DIK_LEFT))
					level2char.x -= level2char.movex;
				if (Key_Down(DIK_RIGHT))
					level2char.x += level2char.movex;
				if (Key_Down(DIK_UP))
					level2char.y -= level2char.movey;
				if (Key_Down(DIK_DOWN))
					level2char.y += level2char.movey;
				if (Key_Down(DIK_A))
					level2char.x -= level2char.movex;
				if (Key_Down(DIK_D))
					level2char.x += level2char.movex;
				if (Key_Down(DIK_W))
					level2char.y -= level2char.movey;
				if (Key_Down(DIK_S))
					level2char.y += level2char.movey;


				//shooting
				if (Key_Down(DIK_SPACE))
					player_shoot();

				//shooting bomb
				if (Key_Down(DIK_V))
					player_bomb();

				for (int n=0; n<BULLETS; n++)
				{
					if(bullets2[n].alive == true)
					{
						if(++bullets2[n].animcount > bullets2[n].animdelay)
						{
							bullets2[n].animcount = 0;
							++bullets2[n].curframe;
						}						

						if(bullets2[n].curframe >= 2)
						{							
							bullets2[n].curframe = 0;
						}
					}

				}

				if(boss2.alive)
				{
					boss_shoot();
				}

				scrolly += 10.0;
				if (scrolly < 0)
					scrolly = 0;
				if (scrolly > BUFFERH - SCREEN_HEIGHT)
					scrolly = BUFFERH - SCREEN_HEIGHT;

				//update virtual scroll position
				virtual_scrolly -= 1.0;
				/*if (virtual_scrolly > virtual_level_size)
				virtual_scrolly = 0.0;*/

				for (int n = 0;n < ENEMIES; n++)
				{
					if (++enemy2[n].animcount > enemy2[n].animdelay)
					{
						//reset counter
						enemy2[n].animcount = 0;

						//animate the sprite
						if (++enemy2[n].curframe > enemy2[n].lastframe)
							enemy2[n].curframe = 0;
					}
				}

				//Bomb & Explosion Animation & Collision
				if(bomb2.alive)
				{
					bomb2.y -= bomb2.movey;

					if(bomb2.y < level2char.y - 200)
					{
						bomb2.alive = false;
						explosion2.alive = true;
						explosion2.effect = true;
						explosion2.x = bomb2.x;
						explosion2.y = bomb2.y - 110;
					}
					for (int n = 0; n < ENEMIES; n++)
					{
						if(Collision(bomb2, enemy2[n]))
						{
							bomb2.alive = false;
							explosion2.alive = true;
							explosion2.effect = true;
							explosion2.x = bomb2.x;
							explosion2.y = bomb2.y - 110;
						}	
					}
				}

				if(explosion2.alive)
				{


					for (int n=0; n<ENEMIES; n++)
					{
						if (Collision(explosion2, enemy2[n]))
						{
							enemy2[n].alive = false;
						}
					}


					if (Collision(explosion2, boss2) && explosion2.effect == true)
					{
						boss2.health -= 5;

						if(boss2.health <= 0)
						{
							boss2.alive = false;
						}

						if(boss2.health <= 0)
						{
							boss2.alive = false;
						}


						explosion2.effect = false;
					}



					if (++explosion2.animcount > explosion2.animdelay)
					{
						explosion2.animcount = 0;

						if (++explosion2.curframe > explosion2.lastframe)
						{
							explosion2.curframe = 0;
							explosion2.alive = false;
						}
					}
				}


				//check bullet Collision
				for (int n=0; n<BULLETS; n++)
				{
					for (int m=0; m<ENEMIES; m++)
					{
						if (bullets2[n].alive == true && enemy2[m].alive == true)
						{
							if (Collision(bullets2[n], enemy2[m]))
							{
								enemy2[m].alive = false;
								enemy2[m].reset();
								bullets2[n].alive = false;
								enemy2[m].releaseE();
								enemycount--;
								playerScore++;							
							}					
						}					
					}
				}

				for (int n=0; n<BULLETS; n++)
				{

					if (boss2_bullet1[n].alive == true)
					{
						if (Collision(boss2_bullet1[n], level2char))
						{
							boss2_bullet1[n].alive = false;
							boss2_bullet1[n].curframe = 0;
							increment1++;
						}	
						if (++boss2_bullet1[n].animcount > boss2_bullet1[n].animdelay)
						{
							boss2_bullet1[n].animcount = 0;

							if (++boss2_bullet1[n].curframe > boss2_bullet1[n].lastframe)
							{
								boss2_bullet1[n].curframe = 5;
							}
						}
					}

					if (boss2_bullet2[n].alive == true)
					{
						if (Collision(boss2_bullet2[n], level2char))
						{
							boss2_bullet2[n].alive = false;
							boss2_bullet2[n].curframe = 0;
							increment1++;
						}				
						if (++boss2_bullet2[n].animcount > boss2_bullet2[n].animdelay)
						{
							boss2_bullet2[n].animcount = 0;

							if (++boss2_bullet2[n].curframe > boss2_bullet2[n].lastframe)
							{
								boss2_bullet2[n].curframe = 5;
							}
						}
					}	

				}

				for (int n=0; n<BULLETS; n++)
				{
					if(bullets2[n].alive == true && boss2.alive == true)
					{

						if (Collision(bullets2[n], boss2))
						{
							bullets2[n].alive = false;
							if(boss2.health <= 0)
							{
								boss2.alive = false;
								playerScore += 50;
								break;
							}
							else
							{
								boss2.health--;
							}
						}
					}
				}

				//check enemy @ bottom of screen
				for (int n=0; n<ENEMIES; n++)
				{
					if (enemy2[n].y > SCREEN_HEIGHT + enemy2[n].height)
					{
						enemy2[n].alive = false;
						enemy2[n].reset();
						enemy2[n].releaseE();
						enemycount--;
						playerScore--;
					}
				}
				//reset zombie
				for (int n=0; n<ENEMIES; n++)
				{
					if (enemy2[n].alive == false)
					{
						enemy2[n].reset();
						enemy2[n].releaseE();
					}
				}

				//check enemy vs player
				for (int n=0; n<ENEMIES; n++)
				{
					if (Collision(level2char, enemy2[n]))
					{
						enemy2[n].alive = false;
						enemy2[n].reset();
						enemy2[n].releaseE();
						enemycount--;
						increment1++;
					}
				}

				bullet2count = 0;
				for (int n=0; n<BULLETS; n++)
				{
					if (bullets2[n].alive) 
					{
						bullet2count++;
						bullets2[n].y -= bullets2[n].movey;

						if (bullets2[n].x < 0 || bullets2[n].x > SCREEN_WIDTH
							|| bullets2[n].y < 0 || bullets2[n].y > SCREEN_HEIGHT)
							bullets2[n].alive = false;
					}
				}

				boss2_bullet1count = 0;
				boss2_bullet2count = 0;
				for (int n=0; n<BULLETS; n++)
				{
					if (boss2_bullet1[n].alive) 
					{
						boss2_bullet1count++;
						boss2_bullet1[n].y += boss2_bullet1[n].movey;
						boss2_bullet1[n].x += boss2_bullet1[n].bulletDirection;

						if (boss2_bullet1[n].x < 0 || boss2_bullet1[n].x > SCREEN_WIDTH
							|| boss2_bullet1[n].y < 0 || boss2_bullet1[n].y > SCREEN_HEIGHT)
						{
							boss2_bullet1[n].alive = false;
							boss2_bullet1[n].curframe = 0;
						}
					}

					if (boss2_bullet2[n].alive) 
					{
						boss2_bullet2count++;
						boss2_bullet2[n].y += boss2_bullet2[n].movey;
						boss2_bullet2[n].x += boss2_bullet2[n].bulletDirection;

						if (boss2_bullet2[n].x < 0 || boss2_bullet2[n].x > SCREEN_WIDTH
							|| boss2_bullet2[n].y < 0 || boss2_bullet2[n].y > SCREEN_HEIGHT)
						{
							boss2_bullet2[n].alive = false;
							boss2_bullet2[n].curframe = 0;
						}
					}
				}			

				if(boss2.alive)
				{
					if(boss2.y <= SCREEN_HEIGHT / 2 - 200)
						boss2.y += boss2.movey;


					if(boss2.x > SCREEN_WIDTH - boss2.width && direction == 1)
					{
						direction = 2;
					}
					else if (boss2.x < 1 && direction == 2)
					{
						direction = 1;
					}

					if (direction == 1)
						boss2.x += boss2.movex;
					else if (direction == 2)
						boss2.x -= boss2.movex;



				}



				for (int n=0; n<ENEMIES; n++)
				{
					if (enemy2[n].alive) 
					{
						//move horiz based on x velocity
						enemy2[n].y += enemy2[n].movey;
					}
				}

				break;
			}
		case 3:
			{
				PlaySound (sound_background3);

				if(increment1 > 4)
				{
					levelTrack = 12;
					StopSound(sound_background3);
					break;
				}

				//Keep character in bounds
				if (level3char.x > SCREEN_WIDTH - level3char.width)
				{
					level3char.x -= level3char.movex;
				}
				if (level3char.x < 0)
				{
					level3char.x += level3char.movex;
				}
				if (level3char.y > SCREEN_HEIGHT - level3char.height)
				{
					level3char.y -= level3char.movey;
				}
				if (level3char.y < (SCREEN_WIDTH / 2) - level3char.height)
				{
					level3char.y += level3char.movey;
				}

				//move the character
				if (Key_Down(DIK_LEFT))
					level3char.x -= level3char.movex;
				if (Key_Down(DIK_RIGHT))
					level3char.x += level3char.movex;
				if (Key_Down(DIK_UP))
					level3char.y -= level3char.movey;
				if (Key_Down(DIK_DOWN))
					level3char.y += level3char.movey;
				if (Key_Down(DIK_A))
					level3char.x -= level3char.movex;
				if (Key_Down(DIK_D))
					level3char.x += level3char.movex;
				if (Key_Down(DIK_W))
					level3char.y -= level3char.movey;
				if (Key_Down(DIK_S))
					level3char.y += level3char.movey;


				//shooting
				if (Key_Down(DIK_SPACE))
					player_shoot();


				//shooting bomb
				if (Key_Down(DIK_V))
					player_bomb();

				if(boss3.alive)
				{
					boss_shoot();
				}

				scrolly += 2.0;
				if (scrolly < 0)
					scrolly = 0;
				if (scrolly > BUFFERH - SCREEN_HEIGHT)
					scrolly = BUFFERH - SCREEN_HEIGHT;

				//update virtual scroll position
				virtual_scrolly -= 1.0;
				/*if (virtual_scrolly > virtual_level_size)
				virtual_scrolly = 0.0;*/

				//check bullet Collision
				for (int n=0; n<BULLETS; n++)
				{
					for (int m=0; m<ENEMIES; m++)
					{
						if (bullets3[n].alive == true && enemy3[m].alive == true)
						{
							if (Collision(bullets3[n], enemy3[m]))
							{
								enemy3[m].alive = false;
								enemy3[m].reset();
								bullets3[n].alive = false;
								enemy3[m].releaseE();
								enemycount--;
								playerScore++;							
							}					
						}					
					}
				}

				for (int n=0; n<BULLETS; n++)
				{

					if (boss3_bullet[n].alive == true)
					{
						if (Collision(boss3_bullet[n], level3char))
						{
							boss3_bullet[n].alive = false;
							increment1++;
						}					
					}					

				}

				//Bomb & Explosion Animation & Collision
				if(bomb3.alive)
				{
					bomb3.y -= bomb3.movey;

					if(bomb3.y < level3char.y - 200)
					{
						bomb3.alive = false;
						explosion3.alive = true;
						explosion3.effect = true;
						explosion3.x = bomb3.x;
						explosion3.y = bomb3.y - 110;
					}
					for (int n = 0; n < ENEMIES; n++)
					{
						if(Collision(bomb3, enemy3[n]))
						{
							bomb3.alive = false;
							explosion3.alive = true;
							explosion3.effect = true;
							explosion3.x = bomb3.x;
							explosion3.y = bomb3.y - 110;
						}	
					}
				}

				if(explosion3.alive)
				{


					for (int n=0; n<ENEMIES; n++)
					{
						if (Collision(explosion3, enemy3[n]))
						{
							enemy3[n].alive = false;
						}
					}

					if (Collision(explosion3, boss3) && explosion3.effect)
					{
						boss3.health -= 5;

						if(boss3.health <= 0)
						{
							boss3.alive = false;
						}

						explosion3.effect = false;
					}


					if (++explosion3.animcount > explosion3.animdelay)
					{
						explosion3.animcount = 0;

						if (++explosion3.curframe > explosion3.lastframe)
						{
							explosion3.curframe = 0;
							explosion3.alive = false;
						}
					}
				}

				for (int n=0; n<BULLETS; n++)
				{
					if(bullets3[n].alive == true && boss3.alive == true)
					{

						if (Collision(bullets3[n], boss3))
						{
							bullets3[n].alive = false;
							if(boss3.health <= 0)
							{
								boss3.alive = false;
								playerScore += 50;
								StopSound(sound_background3);
								break;
							}
							else
							{
								boss3.health--;
							}
						}
					}
				}

				//check enemy @ bottom of screen
				for (int n=0; n<ENEMIES; n++)
				{
					if (enemy3[n].y > SCREEN_HEIGHT + enemy3[n].height)
					{
						enemy3[n].alive = false;
						enemy3[n].reset();
						enemy3[n].releaseE();
						enemycount--;
						playerScore--;
					}
				}
				//reset zombie
				for (int n=0; n<ENEMIES; n++)
				{
					if (enemy3[n].alive == false)
					{
						enemy3[n].reset();
						enemy3[n].releaseE();
					}
				}

				//check enemy vs player
				for (int n=0; n<ENEMIES; n++)
				{
					if (Collision(level3char, enemy3[n]))
					{
						enemy3[n].alive = false;
						enemy3[n].reset();
						enemy3[n].releaseE();
						enemycount--;
						increment1++;
					}
				}

				bullet3count = 0;
				for (int n=0; n<BULLETS; n++)
				{
					if (bullets3[n].alive) 
					{
						bullet3count++;
						bullets3[n].y -= bullets3[n].movey;

						if (bullets3[n].x < 0 || bullets3[n].x > SCREEN_WIDTH
							|| bullets3[n].y < 0 || bullets3[n].y > SCREEN_HEIGHT)
							bullets3[n].alive = false;
					}
				}

				boss3_bulletcount = 0;
				for (int n=0; n<BULLETS; n++)
				{
					if (boss3_bullet[n].alive) 
					{
						boss3_bulletcount++;
						boss3_bullet[n].y += boss3_bullet[n].movey;
						boss3_bullet[n].x += boss3_bullet[n].bulletDirection;

						if (boss3_bullet[n].x < 0 || boss3_bullet[n].x > SCREEN_WIDTH
							|| boss3_bullet[n].y < 0 || boss3_bullet[n].y > SCREEN_HEIGHT)
							boss3_bullet[n].alive = false;
					}
				}			

				if(boss3.alive)
				{
					if(boss3.y <= SCREEN_HEIGHT / 2 - 200)
						boss3.y += boss3.movey;

					if(boss3.x > SCREEN_WIDTH - boss3.width && direction == 1)
					{
						direction = 2;
					}
					else if (boss3.x < 1 && direction == 2)
					{
						direction = 1;
					}

					if (direction == 1)
						boss3.x += boss3.movex;
					else if (direction == 2)
						boss3.x -= boss3.movex;



				}



				for (int n=0; n<ENEMIES; n++)
				{
					if (enemy3[n].alive) 
					{
						//move horiz based on x velocity
						enemy3[n].y += enemy3[n].movey;
					}
				}

				break;
			}
		case 4:
			{
				Key_Up_RETURN(DIK_RETURN, lastKey);

				if (Key_Down(DIK_RETURN) && keyupReturn == true)
				{
					lastKey = DIK_RETURN;
					keyupReturn = false;
					levelTrack = 2;
				}

				if (Key_Up_RETURN(DIK_RETURN, lastKey) == true)
				{
					keyupReturn = true;
				}
				break;
			}
		case 5:
			{
				Key_Up_RETURN(DIK_RETURN, lastKey);

				if (Key_Down(DIK_RETURN) && keyupReturn == true)
				{
					lastKey = DIK_RETURN;
					keyupReturn = false;
					levelTrack = 3;
				}

				if (Key_Up_RETURN(DIK_RETURN, lastKey) == true)
				{
					keyupReturn = true;
				}
				break;
			}
		case 6:
			{
				Key_Up_RETURN(DIK_RETURN, lastKey);

				if (Key_Down(DIK_RETURN) && keyupReturn == true)
				{
					lastKey = DIK_RETURN;
					keyupReturn = false;
					levelTrack = 11;
				}

				if (Key_Up_RETURN(DIK_RETURN, lastKey) == true)
				{
					keyupReturn = true;
				}
				break;
			}
		case 7:
			{

				Key_Up_RETURN(DIK_RETURN, lastKey);
				Key_Up_UP(DIK_UP, lastKey);
				Key_Up_DOWN(DIK_DOWN, lastKey);

				if (Key_Down(DIK_UP) && keyupUp == true)
				{
					lastKey = DIK_UP;
					keyupUp = false;
					if(selectMenu <= 0)
					{
						selectMenu = 3;
					}
					else
					{
						selectMenu--;
					}
				}


				if (Key_Down(DIK_DOWN) && keyupDown == true)
				{
					lastKey = DIK_DOWN;
					keyupDown = false;
					if(selectMenu >= 3)
					{
						selectMenu = 0;
					}
					else
					{
						selectMenu++;
					}
				}

				if(Key_Down(DIK_RETURN) && keyupReturn == true)
				{
					lastKey = DIK_RETURN;
					keyupReturn = false;
					if(selectMenu == 0)
					{
						levelTrack = 10;
						break;
					}
					else if(selectMenu == 1)
					{
						levelTrack = 8;

					}
					else if(selectMenu == 2)
					{
						levelTrack = 9;
					}
					else if(selectMenu == 3)
					{
						SendMessage(hwnd,WM_CLOSE,0,0);
					}	
				}


				if(selectMenu == 0)
				{
					arrow.y = 200;
				}
				else if(selectMenu == 1)
				{
					arrow.y = 270;
				}
				else if(selectMenu == 2)
				{
					arrow.y = 340;
				}
				else if(selectMenu == 3)
				{
					arrow.y = 400;
				}
				if (Key_Up_RETURN(DIK_RETURN, lastKey) == true)
				{
					keyupReturn = true;
				}

				if (Key_Up_UP(DIK_UP, lastKey) == true)
				{
					keyupUp = true;
				}

				if (Key_Up_DOWN(DIK_DOWN, lastKey) == true)
				{
					keyupDown = true;
				}
				break;
			}
		case 8:
			{
				Key_Up_RETURN(DIK_RETURN, lastKey);

				if (Key_Down(DIK_RETURN) && keyupReturn == true)
				{
					lastKey = DIK_RETURN;
					keyupReturn = false;
					levelTrack = 7;
				}

				if (Key_Up_RETURN(DIK_RETURN, lastKey) == true)
				{
					keyupReturn = true;
				}
				break;
			}
		case 9:
			{
				Key_Up_RETURN(DIK_RETURN, lastKey);

				if (Key_Down(DIK_RETURN) && keyupReturn == true)
				{
					lastKey = DIK_RETURN;
					keyupReturn = false;
					levelTrack = 7;
				}

				if (Key_Up_RETURN(DIK_RETURN, lastKey) == true)
				{
					keyupReturn = true;
				}
				break;
			}
		case 10:
			{
				Key_Up_RETURN(DIK_RETURN, lastKey);

				if (Key_Down(DIK_RETURN) && keyupReturn == true)
				{
					lastKey = DIK_RETURN;
					keyupReturn = false;
					levelTrack = 1;
				}

				if (Key_Up_RETURN(DIK_RETURN, lastKey) == true)
				{
					keyupReturn = true;
				}
				break;
			}
		case 11:
			{
				Key_Up_RETURN(DIK_RETURN, lastKey);

				if (Key_Down(DIK_RETURN) && keyupReturn == true)
				{
					lastKey = DIK_RETURN;
					keyupReturn = false;
					levelTrack = 9;
				}

				if (Key_Up_RETURN(DIK_RETURN, lastKey) == true)
				{
					keyupReturn = true;
				}
				break;
			}
		case 12:
			{
				Key_Up_RETURN(DIK_RETURN, lastKey);

				if (Key_Down(DIK_RETURN) && keyupReturn == true)
				{
					lastKey = DIK_RETURN;
					keyupReturn = false;
					levelTrack = 7;
					increment1 = 0;
					scrolly = 0;
				}

				if (Key_Up_RETURN(DIK_RETURN, lastKey) == true)
				{
					keyupReturn = true;
				}
				break;
			}

		}

	}


	//case statement guide
	// 1  - level 1
	// 2  - level 2
	// 3 - level 3
	// 4 - level 1 transition
	// 5 - level 2 transition
	// 6 - level 3 transition
	// 7 - main menu
	// 8 - options
	// 9 - credit screen
	// 10 - story
	// 11 - winning

	//start rendering
	if (d3ddev->BeginScene())
	{
		switch(levelTrack)
		{
		case 1:
			{
				D3DXVECTOR3 position3((float)blood1.x, (float)blood1.y, 0);
				D3DXVECTOR3 position2((float)playerHealth.x, (float)playerHealth.y, 0);
				D3DXVECTOR3 positionBoss((float)boss1.x, (float) boss1.y, 0);
				D3DXVECTOR3 position4((float)explosion1.x, (float) explosion1.y, 0);
				D3DXVECTOR3 positionBomb1((float)bomb1Pic.x, (float) bomb1Pic.y, 0);

				//draw scrolling background
				RECT source_rect = {
					(long)0, 
					(long)(LEVEL1_HEIGHT - SCREEN_HEIGHT) - scrolly, 
					(long)LEVEL1_WIDTH, 
					(long)LEVEL1_HEIGHT - scrolly 
				};
				RECT dest_rect = { 0, 0 , SCREEN_WIDTH, SCREEN_HEIGHT};

				d3ddev->StretchRect(levelback1_image, &source_rect, backbuffer, 
					&dest_rect, D3DTEXF_NONE);
				//erase the entire background
				/*d3ddev->StretchRect(levelback1, NULL, backbuffer, NULL, D3DTEXF_NONE);*/

				//start sprite handler
				sprite_handler->Begin(D3DXSPRITE_ALPHABLEND);

				RECT srcRect1;

				int columns1 = 7;
				srcRect1.left = (level1char.curframe % columns1) * level1char.width;
				srcRect1.top = (level1char.curframe / columns1) * level1char.height;
				srcRect1.right = srcRect1.left + level1char.width;
				srcRect1.bottom = srcRect1.top + level1char.height;
				//draw the character
				position.x = (float)level1char.x;
				position.y = (float)level1char.y;
				sprite_handler->Draw(
					level1char_image, 
					&srcRect1,
					NULL,
					&position,
					D3DCOLOR_XRGB(255,255,255));

				//Spawn boss
				if((LEVEL1_HEIGHT - SCREEN_HEIGHT) - scrolly < 50)
				{
					if(boss1.health > 0)
					{
						boss1.alive = true;

					}


					if (boss1.alive == true)
					{
						RECT srcRectBoss;

						int columnsBoss = 5;
						srcRectBoss.left = (boss1.curframe % columnsBoss) * boss1.width;
						srcRectBoss.top = (boss1.curframe / columnsBoss) * boss1.height;
						srcRectBoss.right = srcRectBoss.left + boss1.width;
						srcRectBoss.bottom = srcRectBoss.top + boss1.height;

						positionBoss.x = (float)boss1.x;
						positionBoss.y = (float)boss1.y;
						sprite_handler->Draw(
							boss1_image, 
							&srcRectBoss,
							NULL,
							&positionBoss,
							D3DCOLOR_XRGB(255,255,255));
					}

					if(boss1.alive == false && boss1.health <= 0)
					{
						levelTrack = 4;
						scrolly = 0;
					}
				}

				//Draw Explosion

				if(blood1.alive == true)
				{
					RECT bloodRect1;
					int columns2 = 7;
					bloodRect1.left = (blood1.curframe % columns2) * blood1.width;
					bloodRect1.top = (blood1.curframe / columns2) * blood1.height;
					bloodRect1.right = bloodRect1.left + blood1.width;
					bloodRect1.bottom = bloodRect1.top + blood1.height;

					//draw player health
					sprite_handler->Draw(
						level1blood_image, 
						&bloodRect1,
						NULL,
						&position3,
						D3DCOLOR_XRGB(255,255,255));
				}

				//Draw Explosion

				if(explosion1.alive == true)
				{
					RECT expRect1;
					int columns2 = 14;
					expRect1.left = (explosion1.curframe % columns2) * explosion1.width;
					expRect1.top = (explosion1.curframe / columns2) * explosion1.height;
					expRect1.right = expRect1.left + explosion1.width;
					expRect1.bottom = expRect1.top + explosion1.height;

					//draw player health
					sprite_handler->Draw(
						explosion1_image, 
						&expRect1,
						NULL,
						&position4,
						D3DCOLOR_XRGB(255,255,255));
				}

				if(bomb1.alive == true)
				{

					position.x = (float)bomb1.x;
					position.y = (float)bomb1.y;

					sprite_handler->Draw(
						bomb1_image, 
						NULL,
						NULL,
						&position,
						D3DCOLOR_XRGB(255,255,255));
				}





				RECT healthRect;

				int columns = 4;
				healthRect.left = (playerHealth.curframe % columns) * playerHealth.width;
				healthRect.top = increment1 * playerHealth.height;
				healthRect.right = healthRect.left + playerHealth.width;
				healthRect.bottom = healthRect.top + playerHealth.height;

				//draw player health
				sprite_handler->Draw(
					playerHealth_image, 
					&healthRect,
					NULL,
					&position2,
					D3DCOLOR_XRGB(255,255,255));

				//Draw Bomb display

				RECT bomb1Rect;

				int columns4 = 4;
				bomb1Rect.left = (bomb1Pic.curframe % columns4) * bomb1Pic.width;
				bomb1Rect.top = increment2 * bomb1Pic.height;
				bomb1Rect.right = bomb1Rect.left + bomb1Pic.width;
				bomb1Rect.bottom = bomb1Rect.top + bomb1Pic.height;

				//draw player health
				sprite_handler->Draw(
					bomb1_display_image, 
					&bomb1Rect,
					NULL,
					&positionBomb1,
					D3DCOLOR_XRGB(255,255,255));

				//draw the enemies
				if(boss1.alive == false)
				{
					for (int n=0; n<ENEMIES; n++)
					{					
						if (enemy1[n].release >= enemycheck)
						{
							enemy1[n].alive = true;
						}
						//is this enemy sprite alive?
						if (enemy1[n].alive == true)
						{
							/*if (enemycount < enemycountmax)
							{*/
							RECT srcRect2;

							int columns2 = 4;
							srcRect2.left = (enemy1[n].curframe % columns2) * enemy1[n].width;
							srcRect2.top = (enemy1[n].curframe / columns2) * enemy1[n].height;
							srcRect2.right = srcRect2.left + enemy1[n].width;
							srcRect2.bottom = srcRect2.top + enemy1[n].height;
							position.x = (float)enemy1[n].x;
							position.y = (float)enemy1[n].y;
							sprite_handler->Draw( 
								level1enemy_image,
								&srcRect2, 
								NULL,
								&position, 
								D3DCOLOR_XRGB(255,255,255));
							/*enemycount++;
							}*/

						}

					}
				}
				//draw bullets
				for (int n=0; n<BULLETS; n++)
				{
					if (bullets1[n].alive) 
					{
						position.x = (float)bullets1[n].x;
						position.y = (float)bullets1[n].y;
						sprite_handler->Draw( 
							level1bullet_image,
							NULL, 
							NULL,
							&position, 
							D3DCOLOR_XRGB(255,255,255));
					}
				}

				//draw boss bullets
				for (int n=0; n<BULLETS; n++)
				{
					if (boss1_bullet[n].alive) 
					{
						position.x = (float)boss1_bullet[n].x;
						position.y = (float)boss1_bullet[n].y;
						sprite_handler->Draw( 
							boss1_bullet_image,
							NULL, 
							NULL,
							&position, 
							D3DCOLOR_XRGB(255,255,255));
					}
				}


				/*if (boss1.alive = true)
				{
					for (int n = 0; n < ENEMIES; n++)
						{
							enemy1[n].alive = false;
						}
				}*/

				int x = SCREEN_WIDTH - 150;
				int y = SCREEN_HEIGHT - 20;
				D3DCOLOR color = D3DCOLOR_ARGB(200, 255, 255, 255);
				D3DCOLOR debugcolor = D3DCOLOR_ARGB(255, 255, 255, 255);

				/*score = playerScore;*/
				FontPrint(score, x, y-12, 
					"Score = " + ToString(playerScore), color);

				//stop drawing
				sprite_handler->End();

				//stop rendering
				d3ddev->EndScene();

				break;
			}
		case 2:
			{
				D3DXVECTOR3 position3((float)blood2.x, (float)blood2.y, 0);
				D3DXVECTOR3 position2((float)playerHealth.x, (float)playerHealth.y, 0);
				D3DXVECTOR3 positionBoss((float)boss2.x, (float) boss2.y, 0);
				D3DXVECTOR3 position4((float)explosion2.x, (float) explosion2.y, 0);
				D3DXVECTOR3 positionBomb2((float)bomb2Pic.x, (float) bomb2Pic.y, 0);

				//draw scrolling background
				RECT source_rect = {
					(long)0, 
					(long)(LEVEL2_HEIGHT - SCREEN_HEIGHT) - scrolly, 
					(long)LEVEL2_WIDTH, 
					(long)LEVEL2_HEIGHT - scrolly 
				};
				RECT dest_rect = { 0, 0 , SCREEN_WIDTH, SCREEN_HEIGHT};

				d3ddev->StretchRect(levelback2_image, &source_rect, backbuffer, 
					&dest_rect, D3DTEXF_NONE);
				//erase the entire background
				/*d3ddev->StretchRect(levelback1, NULL, backbuffer, NULL, D3DTEXF_NONE);*/

				//start sprite handler
				sprite_handler->Begin(D3DXSPRITE_ALPHABLEND);

				RECT char2Rect;

				int columns = 9;
				char2Rect.left = (level2char.curframe % columns) * level2char.width;
				char2Rect.top = (level2char.curframe / columns) * level2char.height;
				char2Rect.right = char2Rect.left + level2char.width;
				char2Rect.bottom = char2Rect.top + level2char.height;

				//draw the character
				position.x = (float)level2char.x;
				position.y = (float)level2char.y;
				sprite_handler->Draw(
					level2char_image, 
					&char2Rect,
					NULL,
					&position,
					D3DCOLOR_XRGB(255,255,255));

				//Spawn boss
				if((LEVEL2_HEIGHT - SCREEN_HEIGHT) - scrolly < 50)
				{
					if (boss2SpawnCheck > 1)
					{
						if(boss2.health > 0)
						{
							boss2.alive = true;
							scrolly = 0;
						}
					}
					else
					{
						boss2SpawnCheck++;
						scrolly = 0;
					}




					if(boss2.alive == false && boss2.health <= 0)
					{
						levelTrack = 5;
						StopSound(sound_background2);
						scrolly = 0;
						StopSound(sound_background2);
					}
				}

				if (boss2.alive == true)
				{

					positionBoss.x = (float)boss2.x;
					positionBoss.y = (float)boss2.y;
					sprite_handler->Draw(
						boss2_image, 
						NULL,
						NULL,
						&positionBoss,
						D3DCOLOR_XRGB(255,255,255));
				}

				RECT healthRect;
				int columns3 = 4;
				healthRect.left = (playerHealth.curframe % columns3) * playerHealth.width;
				healthRect.top = increment1 * playerHealth.height;
				healthRect.right = healthRect.left + playerHealth.width;
				healthRect.bottom = healthRect.top + playerHealth.height;

				//draw player health
				sprite_handler->Draw(
					playerHealth_image, 
					&healthRect,
					NULL,
					&position2,
					D3DCOLOR_XRGB(255,255,255));

				//Draw Explosion

				if(explosion2.alive == true)
				{
					RECT expRect2;
					int columns2 = 14;
					expRect2.left = (explosion2.curframe % columns2) * explosion2.width;
					expRect2.top = (explosion2.curframe / columns2) * explosion2.height;
					expRect2.right = expRect2.left + explosion2.width;
					expRect2.bottom = expRect2.top + explosion2.height;

					//draw player health
					sprite_handler->Draw(
						explosion2_image, 
						&expRect2,
						NULL,
						&position4,
						D3DCOLOR_XRGB(255,255,255));
				}

				if(bomb2.alive == true)
				{

					position.x = (float)bomb2.x;
					position.y = (float)bomb2.y;

					sprite_handler->Draw(
						bomb2_image, 
						NULL,
						NULL,
						&position,
						D3DCOLOR_XRGB(255,255,255));
				}

				//Draw Bomb display

				RECT bomb2Rect;

				int columns4 = 4;
				bomb2Rect.left = (bomb2Pic.curframe % columns4) * bomb2Pic.width;
				bomb2Rect.top = increment2 * bomb2Pic.height;
				bomb2Rect.right = bomb2Rect.left + bomb2Pic.width;
				bomb2Rect.bottom = bomb2Rect.top + bomb2Pic.height;

				//draw player health
				sprite_handler->Draw(
					bomb2_display_image, 
					&bomb2Rect,
					NULL,
					&positionBomb2,
					D3DCOLOR_XRGB(255,255,255));


				//draw the enemies
				if(boss2.alive == false)
				{
					for (int n=0; n<ENEMIES; n++)
					{					
						if (enemy2[n].release >= enemycheck)
						{
							enemy2[n].alive = true;
						}
						//is this enemy sprite alive?
						if (enemy2[n].alive == true)
						{

							RECT enemy2Rect;

							int columns2 = 10;

							enemy2Rect.left = (enemy2[n].curframe % columns2) * enemy2[n].width;
							enemy2Rect.top = (enemy2[n].curframe / columns2) * enemy2[n].height;
							enemy2Rect.right = enemy2Rect.left + enemy2[n].width;
							enemy2Rect.bottom = enemy2Rect.top + enemy2[n].height;

							//draw the character
							position.x = (float)enemy2[n].x;
							position.y = (float)enemy2[n].y;
							sprite_handler->Draw(
								level2enemy_image, 
								&enemy2Rect,
								NULL,
								&position,
								D3DCOLOR_XRGB(255,255,255));							

						}

					}
				}
				//draw bullets
				for (int n=0; n<BULLETS; n++)
				{
					if (bullets2[n].alive) 
					{

						RECT bullet2Rect;

						int columns = 2;
						bullet2Rect.left = (bullets2[n].curframe % columns) * bullets2[n].width;
						bullet2Rect.top = (bullets2[n].curframe / columns) * bullets2[n].height;
						bullet2Rect.right = bullet2Rect.left + bullets2[n].width;
						bullet2Rect.bottom = bullet2Rect.top + bullets2[n].height;

						position3.x = (float)bullets2[n].x;
						position3.y = (float)bullets2[n].y;
						sprite_handler->Draw( 
							level2bullet_image,
							&bullet2Rect, 
							NULL,
							&position3, 
							D3DCOLOR_XRGB(255,255,255));
					}
				}

				//draw the character
				position.x = (float)level2char.x;
				position.y = (float)level2char.y;
				sprite_handler->Draw(
					level2char_image, 
					&char2Rect,
					NULL,
					&position,
					D3DCOLOR_XRGB(255,255,255));

				////draw bullets
				//for (int n=0; n<BULLETS; n++)
				//{
				//	if (bullets2[n].alive) 
				//	{
				//		position.x = (float)bullets2[n].x;
				//		position.y = (float)bullets2[n].y;
				//		sprite_handler->Draw( 
				//			level2bullet_image,
				//			NULL, 
				//			NULL,
				//			&position, 
				//			D3DCOLOR_XRGB(255,255,255));
				//	}
				//}

				//draw boss bullets

				for (int n=0; n<BULLETS; n++)
				{
					if (boss2_bullet1[n].alive) 
					{
						RECT boss2bullet1Rect;

						int columns6 = 7;
						boss2bullet1Rect.left = (boss2_bullet1[n].curframe % columns6) * boss2_bullet1[n].width;
						boss2bullet1Rect.top = (boss2_bullet1[n].curframe / columns6) * boss2_bullet1[n].height;
						boss2bullet1Rect.right = boss2bullet1Rect.left + boss2_bullet1[n].width;
						boss2bullet1Rect.bottom = boss2bullet1Rect.top + boss2_bullet1[n].height;

						position.x = (float)boss2_bullet1[n].x;
						position.y = (float)boss2_bullet1[n].y;
						sprite_handler->Draw( 
							boss2_bullet1_image,
							&boss2bullet1Rect, 
							NULL,
							&position, 
							D3DCOLOR_XRGB(255,255,255));
					}

					if (boss2_bullet2[n].alive) 
					{
						RECT boss2bullet2Rect;

						int columns7 = 7;
						boss2bullet2Rect.left = (boss2_bullet2[n].curframe % columns7) * boss2_bullet2[n].width;
						boss2bullet2Rect.top = (boss2_bullet2[n].curframe / columns7) * boss2_bullet2[n].height;
						boss2bullet2Rect.right = boss2bullet2Rect.left + boss2_bullet2[n].width;
						boss2bullet2Rect.bottom = boss2bullet2Rect.top + boss2_bullet2[n].height;

						position.x = (float)boss2_bullet2[n].x;
						position.y = (float)boss2_bullet2[n].y;
						sprite_handler->Draw( 
							boss2_bullet2_image,
							&boss2bullet2Rect, 
							NULL,
							&position, 
							D3DCOLOR_XRGB(255,255,255));
					}
				}

				/*if (boss2.alive = true)
				{
					for (int n = 0; n < ENEMIES; n++)
						{
							enemy2[n].alive = false;
						}
				}*/

				int x = SCREEN_WIDTH - 150;
				int y = SCREEN_HEIGHT - 20;
				D3DCOLOR color = D3DCOLOR_ARGB(200, 255, 255, 255);
				D3DCOLOR debugcolor = D3DCOLOR_ARGB(255, 255, 255, 255);

				/*score = playerScore;*/
				FontPrint(score, x, y-12, 
					"Score = " + ToString(playerScore), color);

				//stop drawing
				sprite_handler->End();

				//stop rendering
				d3ddev->EndScene();

				break;
			}
		case 3:
			{
				D3DXVECTOR3 position3((float)blood3.x, (float)blood3.y, 0);
				D3DXVECTOR3 position2((float)playerHealth.x, (float)playerHealth.y, 0);
				D3DXVECTOR3 positionBoss((float)boss3.x, (float) boss3.y, 0);
				D3DXVECTOR3 position4((float)explosion3.x, (float) explosion3.y, 0);
				D3DXVECTOR3 positionBomb3((float)bomb3Pic.x, (float) bomb3Pic.y, 0);

				//draw scrolling background
				RECT source_rect = {
					(long)0, 
					(long)(LEVEL3_HEIGHT - SCREEN_HEIGHT) - scrolly, 
					(long)LEVEL3_WIDTH, 
					(long)LEVEL3_HEIGHT - scrolly 
				};
				RECT dest_rect = { 0, 0 , SCREEN_WIDTH, SCREEN_HEIGHT};

				d3ddev->StretchRect(levelback3_image, &source_rect, backbuffer, 
					&dest_rect, D3DTEXF_NONE);
				//erase the entire background
				/*d3ddev->StretchRect(levelback1, NULL, backbuffer, NULL, D3DTEXF_NONE);*/

				//start sprite handler
				sprite_handler->Begin(D3DXSPRITE_ALPHABLEND);

				//draw the character
				position.x = (float)level3char.x;
				position.y = (float)level3char.y;
				sprite_handler->Draw(
					level3char_image, 
					NULL,
					NULL,
					&position,
					D3DCOLOR_XRGB(255,255,255));

				//Spawn boss

				if((LEVEL3_HEIGHT - SCREEN_HEIGHT) - scrolly < 50)
				{
					if(boss3.health > 0)
					{
						boss3.alive = true;
					}


					if (boss3.alive == true)
					{

						position.x = (float)boss3.x;
						position.y = (float)boss3.y;
						sprite_handler->Draw(
							boss3_image, 
							NULL,
							NULL,
							&position,
							D3DCOLOR_XRGB(255,255,255));
					}

					if(boss3.alive == false && boss3.health <= 0)
					{
						levelTrack = 6;

						scrolly = 0;

					}
				}


				RECT healthRect;
				int columns = 4;
				healthRect.left = (playerHealth.curframe % columns) * playerHealth.width;
				healthRect.top = increment1 * playerHealth.height;
				healthRect.right = healthRect.left + playerHealth.width;
				healthRect.bottom = healthRect.top + playerHealth.height;

				//draw player health
				sprite_handler->Draw(
					playerHealth_image, 
					&healthRect,
					NULL,
					&position2,
					D3DCOLOR_XRGB(255,255,255));

				if(explosion3.alive == true)
				{
					RECT expRect3;
					int columns2 = 14;
					expRect3.left = (explosion3.curframe % columns2) * explosion3.width;
					expRect3.top = (explosion3.curframe / columns2) * explosion3.height;
					expRect3.right = expRect3.left + explosion3.width;
					expRect3.bottom = expRect3.top + explosion3.height;

					//draw player health
					sprite_handler->Draw(
						explosion3_image, 
						&expRect3,
						NULL,
						&position4,
						D3DCOLOR_XRGB(255,255,255));
				}

				if(bomb3.alive == true)
				{

					position.x = (float)bomb3.x;
					position.y = (float)bomb3.y;

					sprite_handler->Draw(
						bomb3_image, 
						NULL,
						NULL,
						&position,
						D3DCOLOR_XRGB(255,255,255));
				}

				//Draw Bomb display

				RECT bomb3Rect;

				int columns4 = 4;
				bomb3Rect.left = (bomb3Pic.curframe % columns4) * bomb3Pic.width;
				bomb3Rect.top = increment2 * bomb3Pic.height;
				bomb3Rect.right = bomb3Rect.left + bomb3Pic.width;
				bomb3Rect.bottom = bomb3Rect.top + bomb3Pic.height;

				//draw player health
				sprite_handler->Draw(
					bomb3_display_image, 
					&bomb3Rect,
					NULL,
					&positionBomb3,
					D3DCOLOR_XRGB(255,255,255));


				//draw the enemies

				if(boss3.alive == false)
				{
					for (int n=0; n<ENEMIES; n++)
					{					
						if (enemy3[n].release >= enemycheck)
						{
							enemy3[n].alive = true;
						}
						//is this enemy sprite alive?
						if (enemy3[n].alive == true)
						{
							/*if (enemycount < enemycountmax)
							{*/
							position.x = (float)enemy3[n].x;
							position.y = (float)enemy3[n].y;
							sprite_handler->Draw( 
								level3enemy_image,
								NULL, 
								NULL,
								&position, 
								D3DCOLOR_XRGB(255,255,255));
							/*enemycount++;
							}*/

						}

					}
				}
				//draw bullets
				for (int n=0; n<BULLETS; n++)
				{
					if (bullets3[n].alive) 
					{
						position.x = (float)bullets3[n].x;
						position.y = (float)bullets3[n].y;
						sprite_handler->Draw( 
							level3bullet_image,
							NULL, 
							NULL,
							&position, 
							D3DCOLOR_XRGB(255,255,255));
					}
				}

				//draw boss bullets

				for (int n=0; n<BULLETS; n++)
				{
					if (boss3_bullet[n].alive) 
					{
						position.x = (float)boss3_bullet[n].x;
						position.y = (float)boss3_bullet[n].y;
						sprite_handler->Draw( 
							boss3_bullet_image,
							NULL, 
							NULL,
							&position,
							D3DCOLOR_XRGB(255,255,255));
					}
				}

				/*if (boss3.alive = true)
				{
					for (int n = 0; n < ENEMIES; n++)
						{
							enemy3[n].alive = false;
						}
				}*/

				int x = SCREEN_WIDTH - 150;
				int y = SCREEN_HEIGHT - 20;
				D3DCOLOR color = D3DCOLOR_ARGB(200, 255, 255, 255);
				D3DCOLOR debugcolor = D3DCOLOR_ARGB(255, 255, 255, 255);

				/*score = playerScore;*/
				FontPrint(score, x, y-12, 
					"Score = " + ToString(playerScore), color);

				//stop drawing
				sprite_handler->End();

				//stop rendering
				d3ddev->EndScene();

				break;
			}
		case 4:
			{
				if (d3ddev->BeginScene())
				{
					//erase the entire background
					d3ddev->StretchRect(leveltran1, NULL, backbuffer, NULL, D3DTEXF_NONE);

					sprite_handler->Begin(D3DXSPRITE_ALPHABLEND);

					sprite_handler->End();

					//stop rendering
					d3ddev->EndScene();
				}

				break;
			}
		case 5:
			{
				if (d3ddev->BeginScene())
				{
					//erase the entire background
					d3ddev->StretchRect(leveltran2, NULL, backbuffer, NULL, D3DTEXF_NONE);

					sprite_handler->Begin(D3DXSPRITE_ALPHABLEND);

					sprite_handler->End();

					//stop rendering
					d3ddev->EndScene();
				}

				break;
			}
		case 6:
			{
				if (d3ddev->BeginScene())
				{
					//erase the entire background
					d3ddev->StretchRect(leveltran3, NULL, backbuffer, NULL, D3DTEXF_NONE);

					sprite_handler->Begin(D3DXSPRITE_ALPHABLEND);

					sprite_handler->End();

					//stop rendering
					d3ddev->EndScene();
				}

				d3ddev->Present(NULL, NULL, NULL, NULL);
				break;
			}
		case 7:
			{

				RECT main_rect = { 0, 0 , 640, 400};

				//erase the entire background
				d3ddev->StretchRect(titlescreen, &main_rect, backbuffer, NULL, D3DTEXF_NONE);

				sprite_handler->Begin(D3DXSPRITE_ALPHABLEND);		

				//draw the character
				position.x = (float)arrow.x;
				position.y = (float)arrow.y;
				sprite_handler->Draw(
					arrow_image, 
					NULL,
					NULL,
					&position,
					D3DCOLOR_XRGB(255,255,255));

				//stop drawing
				sprite_handler->End();

				//stop rendering
				d3ddev->EndScene();

				break;
			}
		case 8:
			{
				if (d3ddev->BeginScene())
				{
					//erase the entire background
					d3ddev->StretchRect(options, NULL, backbuffer, NULL, D3DTEXF_NONE);

					sprite_handler->Begin(D3DXSPRITE_ALPHABLEND);

					sprite_handler->End();

					//stop rendering
					d3ddev->EndScene();
				}
				break;
			}
		case 9:
			{
				CreditScreen();

				break;
			}
		case 10:
			{
				if (d3ddev->BeginScene())
				{
					//erase the entire background
					d3ddev->StretchRect(story, NULL, backbuffer, NULL, D3DTEXF_NONE);

					sprite_handler->Begin(D3DXSPRITE_ALPHABLEND);

					sprite_handler->End();

					//stop rendering
					d3ddev->EndScene();
				}

				d3ddev->Present(NULL, NULL, NULL, NULL);
				break;
			}
		case 11:
			{
				if (d3ddev->BeginScene())
				{
					//erase the entire background
					d3ddev->StretchRect(winning, NULL, backbuffer, NULL, D3DTEXF_NONE);

					sprite_handler->Begin(D3DXSPRITE_ALPHABLEND);

					sprite_handler->End();

					//stop rendering
					d3ddev->EndScene();
				}

				d3ddev->Present(NULL, NULL, NULL, NULL);
				break;
			}
		case 12:
			{
				if (d3ddev->BeginScene())
				{
					//erase the entire background
					d3ddev->StretchRect(losing, NULL, backbuffer, NULL, D3DTEXF_NONE);

					sprite_handler->Begin(D3DXSPRITE_ALPHABLEND);

					sprite_handler->End();

					//stop rendering
					d3ddev->EndScene();
				}

				d3ddev->Present(NULL, NULL, NULL, NULL);
				break;
			}

		}

		//display the back buffer on the screen
		d3ddev->Present(NULL, NULL, NULL, NULL);

		//check for mouse button (to exit program)
		if (Mouse_Button(0))
			SendMessage(hwnd,WM_CLOSE,0,0);

		//check for escape key (to exit program)
		if (Key_Down(DIK_ESCAPE))
			SendMessage(hwnd,WM_CLOSE,0,0);
	}
}

//frees memory and cleans up before the game ends
void Game_End(HWND hwnd)
{

	if (titlescreen != NULL)
		titlescreen->Release();

	if (creditscreen != NULL)
		creditscreen->Release();

	if (story != NULL)
		story->Release();

	if (winning != NULL)
		winning->Release();

	if (options != NULL)
		options->Release();

	if (losing != NULL)
		losing->Release();

	if (levelback1_image != NULL)
		levelback1_image->Release();

	if (levelback2_image != NULL)
		levelback2_image->Release();

	if (levelback3_image != NULL)
		levelback3_image->Release();

	if (leveltran1 != NULL)
		leveltran1->Release();

	if (leveltran2 != NULL)
		leveltran2->Release();

	if (leveltran3 != NULL)
		leveltran3->Release();

	if (level1char_image != NULL)
		level1char_image->Release();

	if (level2char_image != NULL)
		level2char_image->Release();

	if (level3char_image != NULL)
		level3char_image->Release();

	if (level1enemy_image != NULL)
		level1enemy_image->Release();

	if (level2enemy_image != NULL)
		level2enemy_image->Release();

	if (level3enemy_image != NULL)
		level3enemy_image->Release();

	if (level1bullet_image != NULL)
		level1bullet_image->Release();

	if (level2bullet_image != NULL)
		level2bullet_image->Release();

	if (level3bullet_image != NULL)
		level3bullet_image->Release();

	if (boss1_bullet_image != NULL)
		boss1_bullet_image->Release();

	if (boss2_bullet1_image != NULL)
		boss2_bullet1_image->Release();

	if (boss2_bullet2_image != NULL)
		boss2_bullet2_image->Release();

	if (boss3_bullet_image != NULL)
		boss3_bullet_image->Release();

	if (level1blood_image != NULL)
		level1blood_image->Release();


	if (boss1_image != NULL)
		boss1_image->Release();

	if (boss2_image != NULL)
		boss2_image->Release();

	if (boss3_image != NULL)
		boss3_image->Release();


	if (sprite_handler != NULL)
		sprite_handler->Release();

	if (sound_hit != NULL)
		delete sound_hit;

	if (sound_bullet1shoot != NULL)
		delete sound_bullet1shoot;
	if (sound_bullet2shoot != NULL)
		delete sound_bullet2shoot;
	if (sound_bullet3shoot != NULL)
		delete sound_bullet3shoot;

	if (sound_background1 != NULL)
		delete sound_background1;

	if (sound_background2 != NULL)
		delete sound_background2;

	if (sound_background3 != NULL)
		delete sound_background3;
	
}

void TitleScreen()
{

	if (d3ddev->BeginScene())
	{
		//erase the entire background
		d3ddev->StretchRect(titlescreen, NULL, backbuffer, NULL, D3DTEXF_NONE);

		sprite_handler->Begin(D3DXSPRITE_ALPHABLEND);

		sprite_handler->End();

		//stop rendering
		d3ddev->EndScene();
	}
}

void CreditScreen()
{

	if (d3ddev->BeginScene())
	{

		D3DXVECTOR3 position(0,0,0);

		//erase the entire background
		d3ddev->StretchRect(creditscreen, NULL, backbuffer, NULL, D3DTEXF_NONE);

		sprite_handler->Begin(D3DXSPRITE_ALPHABLEND);

		sprite_handler->End();

		//stop rendering
		d3ddev->EndScene();
	}
}

void player_shoot()
{

	if(levelTrack == 1)
	{
		//limit firing rate
		if ((int)timeGetTime() < player_shoot_timer + 1000) return;
		player_shoot_timer = timeGetTime();

		//create a bullet
		int b1 = find_bullet();
		if (b1 == -1) return;

		PlaySound(sound_bullet1shoot);

		bullets1[b1].alive = true;
		bullets1[b1].rotation = 0.0;
		bullets1[b1].movex = 0.0f;
		bullets1[b1].movey = 5.0f;
		bullets1[b1].x = level1char.x + level1char.width/2;
		bullets1[b1].y = level1char.y + level1char.height/2 
			- bullets1[b1].height/2;
	}
	else if(levelTrack == 2)
	{
		//limit firing rate
		if ((int)timeGetTime() < player_shoot_timer + 800) return;
		player_shoot_timer = timeGetTime();

		//create a bullet
		int b1 = find_bullet();
		if (b1 == -1) return;

		PlaySound(sound_bullet2shoot);

		bullets2[b1].alive = true;
		bullets2[b1].rotation = 3.0;
		bullets2[b1].movex = 0.0f;
		bullets2[b1].movey = 5.0f;
		bullets2[b1].x = level2char.x + level2char.width/2 - 8;
		bullets2[b1].y = level2char.y;
	}
	else if(levelTrack == 3)
	{
		//limit firing rate
		if ((int)timeGetTime() < player_shoot_timer + 1000) return;
		player_shoot_timer = timeGetTime();

		//create a bullet
		int b1 = find_bullet();
		if (b1 == -1) return;

		PlaySound(sound_bullet3shoot);

		bullets3[b1].alive = true;
		bullets3[b1].rotation = 0.0;
		bullets3[b1].movex = 0.0f;
		bullets3[b1].movey = 5.0f;
		bullets3[b1].x = level3char.x + level3char.width/2;
		bullets3[b1].y = level3char.y + level3char.height/2 
			- bullets3[b1].height/2;
	}

}

void player_bomb()
{
	if ((int)timeGetTime() < player_bomb_timer + 2000) return;
	player_bomb_timer = timeGetTime();

	if(levelTrack == 1)
	{
		if(bomb1Count > 2)
			return;

		bomb1Count++;
		increment2++;
		bomb1.alive = true;
		bomb1.rotation = 0.0;
		bomb1.movex = 0.0f;
		bomb1.movey = 5.0f;
		bomb1.x = level1char.x + level1char.width/2;
		bomb1.y = level1char.y + level1char.height/2 
			- bomb1.height/2;

	}
	else if(levelTrack == 2)
	{
		if(bomb2Count > 2)
			return;

		bomb2Count++;
		increment2++;
		bomb2.alive = true;
		bomb2.rotation = 0.0;
		bomb2.movex = 0.0f;
		bomb2.movey = 5.0f;
		bomb2.x = level2char.x + level2char.width/2;
		bomb2.y = level2char.y + level2char.height/2 
			- bomb2.height/2;
	}
	else if(levelTrack == 3)
	{
		if(bomb3Count > 2)
			return;

		bomb3Count++;
		increment2++;
		bomb3.alive = true;
		bomb3.rotation = 0.0;
		bomb3.movex = 0.0f;
		bomb3.movey = 5.0f;
		bomb3.x = level3char.x + level3char.width/2;
		bomb3.y = level3char.y + level3char.height/2 
			- bomb3.height/2;
	}

}

void boss_shoot()
{
	//limit firing rate
	if ((int)timeGetTime() < boss_shoot_timer1 + 1000) return;
	boss_shoot_timer1 = timeGetTime();

	if ((int)timeGetTime() < boss_shoot_timer2 + 750) return;
	boss_shoot_timer2 = timeGetTime();

	if ((int)timeGetTime() < boss_shoot_timer3 + 500) return;
	boss_shoot_timer3 = timeGetTime();

	//create a bullet
	int c1 = find_boss_bullet();
	if (c1 == -1) return;
	if(levelTrack == 1)
	{
		PlaySound(sound_boss1shoot);

		boss1_bullet[c1].alive = true;
		boss1_bullet[c1].rotation = 0.0;
		boss1_bullet[c1].movex = rand() % 5;
		boss1_bullet[c1].movey = 5.0f;
		boss1_bullet[c1].x = boss1.x + boss1.width/2;
		boss1_bullet[c1].y = boss1.y + boss1.height/2 
			- boss1_bullet[c1].height/2;
	}
	if(levelTrack == 2)
	{
		PlaySound(sound_boss2shoot);

		boss2_bullet1[c1].alive = true;
		boss2_bullet1[c1].rotation = 0.0;
		boss2_bullet1[c1].movex = rand() % 5;
		boss2_bullet1[c1].movey = 5.0f;
		boss2_bullet1[c1].x = boss2.x + 25;
		boss2_bullet1[c1].y = boss2.y + 100;

		boss2_bullet2[c1].alive = true;
		boss2_bullet2[c1].rotation = 0.0;
		boss2_bullet2[c1].movex = rand() % 5;
		boss2_bullet2[c1].movey = 5.0f;
		boss2_bullet2[c1].x = boss2.x + 110;
		boss2_bullet2[c1].y = boss2.y + 100;
	}
	if(levelTrack == 3)
	{
		PlaySound(sound_boss3shoot);

		boss3_bullet[c1].alive = true;
		boss3_bullet[c1].rotation = 0.0;
		boss3_bullet[c1].movex = rand() % 5;
		boss3_bullet[c1].movey = 5.0f;
		boss3_bullet[c1].x = boss3.x + boss3.width/2;
		boss3_bullet[c1].y = boss3.y + boss3.height/2 
			- boss3_bullet[c1].height/2;
	}

}

int find_bullet()
{
	int bullet = -1;
	if(levelTrack == 1)
	{
		for (int n=0; n<BULLETS; n++)
		{
			if (!bullets1[n].alive) 
			{
				bullet = n;
				break;
			}		
		}
	}
	else if(levelTrack == 2)
	{
		for (int n=0; n<BULLETS; n++)
		{
			if (!bullets2[n].alive) 
			{
				bullet = n;
				break;
			}		
		}
	}
	else if(levelTrack == 3)
	{
		for (int n=0; n<BULLETS; n++)
		{
			if (!bullets3[n].alive) 
			{
				bullet = n;
				break;
			}		
		}
	}
	return bullet;
}

int find_boss_bullet()
{
	int bossBullet = -1;
	if(levelTrack ==1)
	{
		for (int n=0; n<BULLETS; n++)
		{
			if (!boss1_bullet[n].alive) 
			{
				bossBullet = n;
				break;
			}
		}
	}
	else if(levelTrack == 2)
	{
		for (int n=0; n<BULLETS; n++)
		{
			if (!boss2_bullet1[n].alive) 
			{
				bossBullet = n;
				break;
			}
		}
	}
	else if(levelTrack == 3)
	{
		for (int n=0; n<BULLETS; n++)
		{
			if (!boss3_bullet[n].alive) 
			{
				bossBullet = n;
				break;
			}
		}
	}
	return bossBullet;
}
