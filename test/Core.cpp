#include "Core.h"
#include "Sprite.h"
#include "Vector2.h"

HGE *g_hge=0;  

const Sprite* g_pSquareSprite;

void RenderBegin()
{
	g_hge->Gfx_BeginScene();
	g_hge->Gfx_Clear(DWORD(0xFFAAAAAA));
}
void RenderEnd()
{
	g_hge->Gfx_EndScene();
}
void Initialize(int screenWidthInPixels, int screenHeightInPixels, hgeCallback updateCallback, hgeCallback renderCallback)
{
	g_hge = hgeCreate(HGE_VERSION);
	g_hge->System_SetState(HGE_FRAMEFUNC, updateCallback);
	g_hge->System_SetState(HGE_RENDERFUNC, renderCallback);
	g_hge->System_SetState(HGE_TITLE, "Tetris");
	g_hge->System_SetState(HGE_SHOWSPLASH, false );
	g_hge->System_SetState(HGE_WINDOWED, true);
	g_hge->System_SetState(HGE_SCREENWIDTH, screenWidthInPixels);
	g_hge->System_SetState(HGE_SCREENHEIGHT, screenHeightInPixels);
	g_hge->System_SetState(HGE_SCREENBPP, 32);
}
bool IsInitialized()
{
	return g_hge->System_Initiate();
}
void Run()
{
	g_hge->System_Start();
}
void Shutdown()
{
	g_hge->System_Shutdown();
	g_hge->Release();
}

float GetDeltaTime()
{
	return g_hge->Timer_GetDelta();
}