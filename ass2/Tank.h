#pragma once
#include "header.h"
#include "Texture.h"
#include "Vector2.h"
#include "BulletNorm.h"
#include "Laser.h"
#include <vector>
#include <string>

class Tank
{
public:
	//Maximum axis velocity of the dot
	static const int TANK_VEL = 1;
	static const int WIDTH = 31;
	static const int HEIGHT = 49;

	//Initializes the variables and allocates particles
	Tank(std::string src, int p);

	//Deallocates particles
	~Tank();

	//Takes key presses and adjusts the dot's velocity
	void handleEvent(SDL_Event& e);

	//Shows the dot on the screen
	void update();
	void loadTexture(SDL_Renderer* rR);
	void render(SDL_Renderer* rR);
	void renderBullet(SDL_Renderer* rR);
	void renderLaser(SDL_Renderer* rR);
	std::vector<BulletNorm*>& getBullet();
	bool checkBulletCollision(BulletNorm* bullet);
	void destroyBullet(int i);
	void reset();
	void initLaser();
	void destroyLaser();
	Laser* getLaser();

	//set and get
	void setVel(Vector2 v) { vel = v; }
	void undoMove();
	void setPos(float x, float y) {
		setPosX(x);
		setPosY(y);
	}
	void setPosX(float x) { 
		mPosX = x; 
		cenPos.setX(mPosX + center.getX());
	}
	void setPosY(float y) {
		mPosY = y; 
		cenPos.setY(mPosY + center.getY());
	}
	void setScale(float s);

	Vector2 getVel() { return vel; }
	float getPosX() { return mPosX; }
	float getPosY() { return mPosY; }
	float getCenPosX() { return cenPos.getX(); }
	float getCenPosY() { return cenPos.getY(); }
	float getAlpha() { return alpha; }

	std::vector<Vector2> getCorners();

	void fire();
private:
	void move();
	void updateBullet();
	void setScaleTexture();

	// player
	std::string src;
	int p;

	//The X and Y offsets of the dot
	LTexture* tankTexture;
	int width, height;
	float scale;
	Vector2 cenPos;
	float mPosX, mPosY, b1, b2, head;
	Vector2 center;
	std::vector<Vector2> corners;

	//The velocity of the dot
	Vector2 vel;
	float alpha;
	float speed;
	int turn_speed;
	bool turn_left, turn_right;

	//bullet
	int bnum;
	std::vector<BulletNorm*> bullets;
	LTexture* bullet_texture;

	Laser* laser;
};
