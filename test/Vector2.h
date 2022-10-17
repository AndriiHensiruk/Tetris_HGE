#ifndef VECTOR_2_H
#define VECTOR_2_H

#include "hgevector.h"

class Vector2
{
public:
	Vector2();	//default constructor
	Vector2(float x, float y); //initializing constructor
	Vector2(const Vector2& vec);	//copy constructor

	~Vector2(){}

	float X() const{return m_hgeVector.x;}
	float Y() const {return m_hgeVector.y;}

	//Returns the dot product between the two vectors
	float Dot(Vector2& vec);

	//Returns the length of this vector
	float Length();

	//Returns the squared length of this vector
	float SquaredLength();

	//Returns the angle between the two vectors
	float Angle(Vector2& vec);

	//Returns the angle between this vector and the X axis
	float Angle();

	//Normalizes this vector
	void Normalize();

	//Rotates this vector to the specified angle relative to the X axis
	void Rotate(float angle);

	//Operators
	Vector2 operator+(const Vector2& v) const
	{
		return Vector2(m_hgeVector.x + v.m_hgeVector.x, m_hgeVector.y + v.m_hgeVector.y);
	}
	Vector2 operator-(const Vector2& v) const
	{
		return Vector2(m_hgeVector.x - v.m_hgeVector.x, m_hgeVector.y - v.m_hgeVector.y);
	}
    Vector2 operator-() const
    {
        return Vector2(-m_hgeVector.x, -m_hgeVector.y);
    }
    void operator+=(const Vector2& v)
    {
		m_hgeVector += v.m_hgeVector;
    }
    void operator-=(const Vector2& v)
	{
		m_hgeVector -= v.m_hgeVector;
    }
    Vector2 operator/(const float s) const
    {
         float r = 1.f / s;
         return Vector2(m_hgeVector.x * r, m_hgeVector.y * r);
    }
    Vector2 operator*(const float s) const
    {
         return Vector2(m_hgeVector.x * s, m_hgeVector.y * s);
    }
    void operator*=(const float s)
    {
        m_hgeVector*= s;
    }
    bool operator==(const Vector2& v) const
    {
         return (m_hgeVector == v.m_hgeVector);
    }
    bool operator!=(const Vector2& v) const
    {
         return (m_hgeVector != v.m_hgeVector);
    }

protected:

	//Internal representation of the vector.  Shouldn't need to touch this.
	hgeVector m_hgeVector;
};

#endif