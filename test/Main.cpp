#include "Core.h"
#include "Sprite.h"
#include "Vector2.h"
#include <time.h>
#include <list>

#include "hge.h"
#include "hgefont.h"
#include <hgesprite.h>
#include "hgegui.h"

#include "menuitem.h"

#include <math.h>


// Some resource handles
HEFFECT				snd;
HTEXTURE			tex, tex1;
hgeQuad				quad;


// Pointers to the HGE objects we will use
hgeGUI				*gui;
hgeFont				*fnt;
hgeSprite			*spr;
hgeSprite			*spr1;


HMUSIC music;
HEFFECT fallEffect, lineEffect, gameoverEffect;
//HTEXTURE tex;
float dx = 0.0f;
//hgeSprite* spr;

const int height = 36; // 648 / 18;
const int width = 18; // 324 / 18;

bool rotate = 0; int colorNum = 1, gameRunning = true;

float hDelayBase = 0.1, delayBase = 0.3;
float hDelay = 0.1, delay = 0.3;
float timer = 0, hTimer = 0;
int linesHit = 0;

int field[height][width] = { 0 };

struct Point
{
	int x, y;
};

Point a[4], b[4];

int blockShapes[7][4] =
{
	1,3,5,7, // I
	2,4,5,7, // Z
	3,5,4,6, // S
	3,5,4,7, // T
	2,3,5,7, // L
	3,5,7,6, // J
	2,3,4,5, // O
	//1,2,5,6, // S
};

void PlaySoundEffect(HEFFECT effect)
{
	g_hge->Effect_PlayEx(effect, 35, 1, 1);
}

void GameOver()
{
	if (gameRunning)
	{
		PlaySoundEffect(gameoverEffect);
		gameRunning = false;
	}
}

void RegenerateFallingBlock() 
{
	for (int i = 0; i < 4; i++)
	{
		field[b[i].y][b[i].x] = colorNum;
	}

	colorNum = 1 + rand() % 7;
	int n = rand() % 7;
	for (int i = 0; i < 4; i++)
	{
		a[i].x = blockShapes[n][i] % 2;
		a[i].y = blockShapes[n][i] / 2;
	}
}

bool CheckBounds()
{
	for (int i = 0; i < 4; i++) 
	{
		if (a[i].x < 0 || a[i].x >= width || a[i].y >= height) 
		{
			return 0;
		}
		else if (field[a[i].y][a[i].x])
		{
			return 0;
		}
	}
		
	return 1;
};

void ProcessMovementInput()
{
	if (g_hge->Input_GetKeyState(HGEK_LEFT))
	{
		hDelay = hDelayBase;
		dx = -1;
	}

	if (g_hge->Input_GetKeyState(HGEK_RIGHT))
	{
		hDelay = hDelayBase;
		dx = +1;
	}

	if (g_hge->Input_KeyDown(HGEK_UP))
	{
		rotate = true;
	}

	if (g_hge->Input_GetKeyState(HGEK_DOWN))
	{
		delay = 0.05;
	}
}

void Rotate()
{
	if (rotate)
	{
		Point p = a[1]; //center of rotation
		for (int i = 0; i < 4; i++)
		{
			int x = a[i].y - p.y;
			int y = a[i].x - p.x;
			a[i].x = p.x - x;
			a[i].y = p.y + y;
		}

		if (!CheckBounds())
		{
			for (int i = 0; i < 4; i++) a[i] = b[i];
		}

		rotate = false;
	}
}

void MoveSpriteXY()
{
	if (hTimer > hDelay)
	{
		for (int i = 0; i < 4; i++)
		{
			b[i] = a[i];
			a[i].x += dx;
		}

		if (!CheckBounds())
		{
			for (int i = 0; i < 4; i++)
			{
				if (a[i].y <= 0)
				{
					GameOver();
				}

				a[i] = b[i];
			}
		}
		hTimer = 0;
	}

	if (timer > delay)
	{
		for (int i = 0; i < 4; i++)
		{
			b[i] = a[i];
			a[i].y += 1;
		}

		if (!CheckBounds())
		{
			PlaySoundEffect(fallEffect);

			RegenerateFallingBlock();
		}

		timer = 0;
	}
}

void RemoveLines()
{
	int k = height - 1;

	for (int i = height - 1; i > 0; i--)
	{
		int count = 0;

		for (int j = 0; j < width; j++)
		{
			if (field[i][j])
			{
				count++;
			}

			field[k][j] = field[i][j];
		}

		if (count < width)
		{
			k--;
		}

		if (count == width)
		{
			PlaySoundEffect(lineEffect);

			//speed up v & h movement
			if (linesHit < 5)
			{
				hDelayBase -= 0.01;
				delayBase -= 0.05;
				linesHit++;
			}
		}
	}
}

void RenderSprites()
{
	//i=1 to remove top single block. Set to 0 to see root block
	for (int i = 1; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (field[i][j] == 0) continue;
			spr1->SetTextureRect(field[i][j] * 18, 0, 18, 18);
			spr1->Render(j * 18, i * 18);
		}
	}

	for (int i = 0; i < 4; i++)
	{
		spr1->SetTextureRect(colorNum * 18, 0, 18, 18);
		spr1->Render(a[i].x * 18, a[i].y * 18);
	}
}

bool Update()
{
	if (!gameRunning) return 0;

	if (g_hge->Input_GetKeyState(HGEK_ESCAPE))
	{
		return 1;
	}

	timer += GetDeltaTime();
	hTimer += GetDeltaTime();

	ProcessMovementInput();

	MoveSpriteXY();

	Rotate();

	RemoveLines();

	dx = 0; rotate = false; delay = delayBase, hDelay = hDelayBase;

	return 0;
}

bool Render()
{	
	g_hge->Gfx_BeginScene();

	g_hge->Gfx_Clear(0);

	RenderSprites();

	g_hge->Gfx_EndScene();

	return 0;
}
////
bool FrameFunc()
{
	float dt=g_hge->Timer_GetDelta();
	static float t=0.0f;
	float tx,ty;
	int id;
	static int lastid=0;

	// If ESCAPE was pressed, tell the GUI to finish
	if(g_hge->Input_GetKeyState(HGEK_ESCAPE)) { lastid=5; gui->Leave(); }
	
	// We update the GUI and take an action if
	// one of the menu items was selected
	id=gui->Update(dt);
	if(id == -1)
	{
		switch(lastid)
		{
			case 1:{
				
				Initialize(324, 648, Update, Render);
				srand(time(0));
				break;
			   }
			case 2:
			case 3:
			case 4:
				gui->SetFocus(1);
				gui->Enter();
				break;

			case 5: return true;
		}
	}
	else if(id) { lastid=id; gui->Leave(); }

	// Here we update our background animation
	t+=dt;
	tx=50*cosf(t/60);
	ty=50*sinf(t/60);

	quad.v[0].tx=tx;        quad.v[0].ty=ty;
	quad.v[1].tx=tx+324/64; quad.v[1].ty=ty;
	quad.v[2].tx=tx+324/64; quad.v[2].ty=ty+640/64;
	quad.v[3].tx=tx;        quad.v[3].ty=ty+640/64;

	return false;
}
bool RenderFunc()
{
	// Render graphics
	g_hge->Gfx_BeginScene();
	g_hge->Gfx_RenderQuad(&quad);
	gui->Render();
	/*fnt->SetColor(0xFFFFFFFF);
	fnt->printf(5, 5, HGETEXT_LEFT, "dt:%.3f\nFPS:%d", hge->Timer_GetDelta(), hge->Timer_GetFPS());*/
	g_hge->Gfx_EndScene();

	return false;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	g_hge = hgeCreate(HGE_VERSION);

	g_hge->System_SetState(HGE_LOGFILE, "hge_tetris.log");
	g_hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	g_hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
	g_hge->System_SetState(HGE_TITLE, "Tetris menus");
	g_hge->System_SetState(HGE_WINDOWED, true);
	g_hge->System_SetState(HGE_SCREENWIDTH, 324);
	g_hge->System_SetState(HGE_SCREENHEIGHT, 648);
	g_hge->System_SetState(HGE_SCREENBPP, 32);

	if(g_hge->System_Initiate())
	{

		// Load sound and texturestetris
		quad.tex=g_hge->Texture_Load("resources/bg.png");
		tex=g_hge->Texture_Load("resources/cursor.png");
		snd=g_hge->Effect_Load("resources/menu.wav");

		
		if(!quad.tex || !tex || !snd)
		{
			// If one of the data files is not found, display
			// an error message and shutdown.
			MessageBox(NULL, "Can't load BG.PNG, CURSOR.PNG or MENU.WAV", "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
			g_hge->System_Shutdown();
			g_hge->Release();
			return 0;
		}

		gameoverEffect = g_hge->Effect_Load("resources/gameover.wav");
		fallEffect = g_hge->Effect_Load("resources/fall.wav");
		lineEffect = g_hge->Effect_Load("resources/line.wav");
		music = g_hge->Music_Load("resources/music.it");
		tex1 = g_hge->Texture_Load("resources/tiles.png");

		spr1 = new hgeSprite(tex1, 0, 0, 18, 18);
		RegenerateFallingBlock();
	
		if (!music || !fallEffect || !lineEffect || !gameoverEffect || !tex1)
		{
			Shutdown();
			return 0;
		}

		g_hge->Music_Play(music, true, 1);

		///
		// Set up the quad we will use for background animation
		quad.blend=BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_NOZWRITE;

		for(int i=0;i<4;i++)
		{
			// Set up z-coordinate of vertices
			quad.v[i].z=0.5f;
			// Set up color. The format of DWORD col is 0xAARRGGBB
			quad.v[i].col=0xFFFFFFFF;
		}

		quad.v[0].x=0; quad.v[0].y=0; 
		quad.v[1].x=324; quad.v[1].y=0; 
		quad.v[2].x=324; quad.v[2].y=648; 
		quad.v[3].x=0; quad.v[3].y=648; 


		// Load the font, create the cursor sprite
		fnt=new hgeFont("resources/font1.fnt");
		spr=new hgeSprite(tex,0,0,32,32);

		// Create and initialize the GUI
		gui=new hgeGUI();

		gui->AddCtrl(new hgeGUIMenuItem(1,fnt,snd,150,200,0.0f,"Play"));
		gui->AddCtrl(new hgeGUIMenuItem(2,fnt,snd,150,240,0.1f,"Options"));
		gui->AddCtrl(new hgeGUIMenuItem(3,fnt,snd,150,280,0.2f,"Instructions"));
		gui->AddCtrl(new hgeGUIMenuItem(4,fnt,snd,150,320,0.3f,"Credits"));
		gui->AddCtrl(new hgeGUIMenuItem(5,fnt,snd,150,360,0.4f,"Exit"));

		gui->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED);
		gui->SetCursor(spr);
		gui->SetFocus(1);
		gui->Enter();

		// Let's rock now!
		g_hge->System_Start();

		// Delete created objects and free loaded resources
		delete gui;
		delete fnt;
		delete spr;
		g_hge->Effect_Free(snd);
		g_hge->Texture_Free(tex);
		g_hge->Texture_Free(quad.tex);
		g_hge->Texture_Free(tex1);
		g_hge->Music_Free(music);
		g_hge->Effect_Free(fallEffect);
		g_hge->Effect_Free(lineEffect);
		g_hge->Effect_Free(gameoverEffect);
	}
	
	// Clean up and shutdown
	Shutdown();
	return 0;
}


