#include "Vector2.h"
#include <math.h>

/* ******************************************** */

Vector2::Vector2(void) {
	this->X = 0;
	this->Y = 0;
}

Vector2::Vector2(float X, float Y) {
	this->X = X;
	this->Y = Y;
}

Vector2::~Vector2(void) { }

/* ******************************************** */

float Vector2::getX() const {
	return X;
}

void Vector2::setX(float X) {
	this->X = X;
}

float Vector2::getY() const {
	return Y;
}

void Vector2::setY(float Y) {
	this->Y = Y;
}

float Vector2::length() {
	return sqrt(X * X + Y * Y);
}

void Vector2::set(float X, float Y) {
	this->X = X;
	this->Y = Y;
}

Vector2 Vector2::normalize() {
	if (X == 0 && Y == 0) return Vector2(0, 0);

	float length = sqrt(X * X + Y * Y);
	return Vector2(X / length, Y / length);
}

float Vector2::dotVector(Vector2& v) {
	return v.getX() * X + v.getY() * Y;
}

void Vector2::floor() {
	X = floorf(X);
	Y = floorf(Y);
}

Vector2 Vector2::operator+ (const Vector2& v) {
	return Vector2(X + v.getX(), Y + v.getY());
}

Vector2 Vector2::operator- (const Vector2& v) {
	return Vector2(X - v.getX(), Y - v.getY());
}

Vector2 operator* (float val, const Vector2& v) {
	return Vector2(val * v.getX(), val * v.getY());
}

Vector2 operator* (const Vector2& v, float val) {
	return Vector2(val * v.getX(), val * v.getY());
}