#include "Sprite.h"

Sprite::Sprite(char* textureName, Vector2 positionOnTexture, Vector2 dimensionsOnTexture)
: m_texture(g_hge->Texture_Load(textureName))
, m_positionOnTexture(positionOnTexture)
, m_dimensionsOnTexture(dimensionsOnTexture)
{
	m_hgeSprite = new hgeSprite(m_texture, positionOnTexture.X(), positionOnTexture.Y(), dimensionsOnTexture.X(), dimensionsOnTexture.Y());
}

Sprite::Sprite(const Sprite& sprite)
: m_texture(sprite.m_texture)
, m_positionOnTexture(sprite.m_positionOnTexture)
, m_dimensionsOnTexture(sprite.m_dimensionsOnTexture)
{
	m_hgeSprite = new hgeSprite(m_texture,m_positionOnTexture.X(), m_positionOnTexture.Y(), m_dimensionsOnTexture.X(), m_dimensionsOnTexture.Y());
}

void Sprite::Render(Vector2 position, float rotation) const
{
	m_hgeSprite->RenderEx(position.X(), position.Y(), rotation);
}

void Sprite::SetAnchorPoint(Vector2 newAnchorPoint)
{
	m_hgeSprite->SetHotSpot(newAnchorPoint.X(), newAnchorPoint.Y());
}

Vector2 Sprite::GetAnchorPoint()
{
	float x, y;
	m_hgeSprite->GetHotSpot(&x,&y);

	return Vector2(x,y);
}

void Sprite::SetColor(byte red, byte green, byte blue, byte alpha)
{
	DWORD color = ((DWORD(alpha)<<24) + (DWORD(red)<<16) + (DWORD(green)<<8) + DWORD(blue));
	m_hgeSprite->SetColor(color);
}