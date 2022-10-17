#ifndef CORE_H
#define CORE_H

#include "hge.h"

class Sprite;
class Vector2;

extern HGE* g_hge;

float GetDeltaTime();

void RenderBegin();
void RenderEnd();
void Initialize(int screenWidthInPixels, int screenHeightInPixels, hgeCallback updateCallback, hgeCallback renderCallback);
bool IsInitialized();
void Run();
void Shutdown();

#endif