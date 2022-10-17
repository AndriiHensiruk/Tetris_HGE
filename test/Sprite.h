#ifndef SPRITE_H
#define SPRITE_H
#include "hgesprite.h"
#include "Vector2.h"
#include "Core.h"

class Sprite
{
public:

	//textureName includes filename extension (.png)
	//positionOnTexture is start pixel of the sprite (upper left corner)
	//dimensionsOnTexture is the (width,height) of the sprite on the texture
	Sprite(char* textureName, Vector2 positionOnTexture, Vector2 dimensionsOnTexture);
	Sprite(const Sprite& sprite);	//copy constructor
	~Sprite(){delete m_hgeSprite;}

	//Renders the sprite at the given screen position.  Must be called between RenderBegin and RenderEnd in the Render loop.
	void Render(Vector2 position, float rotation = 0.0f) const;

	//Sets the anchor point for position and orietation operations
	//This is in the sprites "local space"
	//Defaults to (0,0)
	void SetAnchorPoint(Vector2 newAnchorPoint);
	Vector2 GetAnchorPoint();

	void SetColor(byte red, byte green, byte blue, byte alpha);
	byte GetColorRed(){return (((m_hgeSprite->GetColor())>>16) & 0xFF);}
	byte GetColorGreen(){return (((m_hgeSprite->GetColor())>>8) & 0xFF);}
	byte GetColorBlue(){return ((m_hgeSprite->GetColor()) & 0xFF);}
	byte GetColorAlpha(){return (m_hgeSprite->GetColor() >> 24);}

	//Width of the sprite in pixels
	float GetWidth(){return m_dimensionsOnTexture.X();}

	//Height of the sprite in pixels
	float GetHeight(){return m_dimensionsOnTexture.Y();}

protected:
	hgeSprite* m_hgeSprite;
	const HTEXTURE m_texture;

	//These are the position(upper left corner) and dimensions (width,height) of the sprite
	//on the source texture;
	const Vector2 m_positionOnTexture;
	const Vector2 m_dimensionsOnTexture;
};

#endif