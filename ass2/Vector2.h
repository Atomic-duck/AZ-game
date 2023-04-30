#pragma once

#ifndef VECTORR_H
#define VECTORR_H

class Vector2
{
private:
	float X, Y;

public:
	Vector2(void);
	Vector2(float X, float Y);
	~Vector2(void);

	float getX() const;
	float getY() const;
	void setX(float X);
	void setY(float Y);
	void set(float X, float Y);
	void floor();
	float dotVector(Vector2& v);
	float length();
	Vector2 normalize();
	
	Vector2 operator+ (const Vector2& v);
	Vector2 operator- (const Vector2& v);
	Vector2 operator* (const Vector2& v);
};

Vector2 operator* (float val, const Vector2& v);
Vector2 operator* (const Vector2& v, float val);

#endif