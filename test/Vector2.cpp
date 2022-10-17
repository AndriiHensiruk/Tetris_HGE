#include "Vector2.h"


Vector2::Vector2()
{
	m_hgeVector = hgeVector(0.0f,0.0f);
}

Vector2::Vector2(float x, float y)
{
	m_hgeVector = hgeVector(x,y);
}

Vector2::Vector2(const Vector2& vec)
{
	m_hgeVector = hgeVector(vec.X(), vec.Y());
}

float Vector2::Dot(Vector2& vec)
{
	return m_hgeVector.Dot(&vec.m_hgeVector);
}

float Vector2::Length()
{
	return m_hgeVector.Length();
}

float Vector2::SquaredLength()
{
	return m_hgeVector.Dot(&m_hgeVector);
}

float Vector2::Angle(Vector2& vec)
{
	return m_hgeVector.Angle(&vec.m_hgeVector);
}

float Vector2::Angle()
{
	return m_hgeVector.Angle();
}

void Vector2::Normalize()
{
	m_hgeVector = *m_hgeVector.Normalize();
}

void Vector2::Rotate(float angle)
{
	m_hgeVector.Rotate(angle);
}