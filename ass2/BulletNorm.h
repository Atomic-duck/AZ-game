#pragma once
#include "header.h"
#include "Texture.h"
#include "Vector2.h"

class BulletNorm
{
public:
	static const int WIDTH = 8;
	static const int HEIGHT = 8;
	//Initializes the variables and allocates particles
	BulletNorm(float x = 0, float y = 0);

	//Deallocates particles
	~BulletNorm();

	//Moves the dot
	void move();

	//Shows the dot on the screen
	void render(SDL_Renderer* rR);

	void loadTexture(LTexture* texture);

	void setVel(Vector2 v) { vel = v; }
	void setDirect(Vector2 v) { vel = v*VEL; }
	void setVelTemp(Vector2 v) { vel_temp = v; }
	void setPosX(float x) { mPosX = x; }
	void setPosY(float y) { mPosY = y; }
	void setCenter(Vector2 c);
	void setScale(float s) { scale = s; }

	Vector2 getCenter();
	Vector2 getVel() { return vel; }
	Vector2 getTempVel() { return vel_temp; }
	float getPosX() { return mPosX; }
	float getPosY() { return mPosY; }
	int getTimer() {return timer;}
	float getRadious() { return radious * scale; }
	float getWidth() { return width * scale; }
	float getHeight() { return height * scale; }
private:
	//The X and Y offsets of the dot
	LTexture* texture;
	float mPosX, mPosY;
	int timer;
	float scale;
	float VEL = 1.8;
	float width = 8;
	float height = 8;
	float radious = 4;

	//The velocity of the dot
	Vector2 vel;
	Vector2 vel_temp;
};

