#pragma once
#include "header.h"
#include "Texture.h"
#include "Vector2.h"

class Ball
{
public:
	//The dimensions of the dot
	static const int DOT_WIDTH = 20;
	static const int DOT_HEIGHT = 20;
	static const int RADIOUS = 10;

	//Maximum axis velocity of the dot
	static const int DOT_VEL = 10;

	//Initializes the variables and allocates particles
	Ball();

	//Deallocates particles
	~Ball();

	//Takes key presses and adjusts the dot's velocity
	void handleEvent(SDL_Event& e);

	//Moves the dot
	void move();

	//Shows the dot on the screen
	void render(SDL_Renderer* rR);

	void loadTexture(LTexture* dotTexture);

	bool checkMouse(int x, int y);

	void setVelByPoint(int x, int y);
	void setVel(Vector2 v) { vel = v; }
	void setVelTemp(Vector2 v) { vel_temp = v; }
	void setPosX(float x) { mPosX = x; }
	void setPosY(float y) { mPosY = y; }
	void setCenter(Vector2 c);

	Vector2 getCenter();
	Vector2 getVel() { return vel; }
	Vector2 getTempVel() { return vel_temp; }
	float getPosX() { return mPosX; }
	float getPosY() { return mPosY; }

private:
	//The X and Y offsets of the dot
	LTexture* dotTexture;
	float mPosX, mPosY;

	//The velocity of the dot
	Vector2 vel;
	Vector2 vel_temp;
};

