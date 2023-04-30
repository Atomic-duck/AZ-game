#include "Ball.h"
#include "CFG.h"

Ball::Ball()
{
	//Initialize the offsets
	dotTexture = nullptr;
	mPosX = 30;
	mPosY = 30;
}

Ball::~Ball()
{
}

void Ball::loadTexture(LTexture* dotTexture) {
	this->dotTexture = dotTexture;
}

void Ball::handleEvent(SDL_Event& e)
{
	
}

void Ball::move()
{
	//Move the dot left or right
	mPosX += vel.getX();
	//Move the dot up or down
	mPosY += vel.getY();

	if (vel_temp.getX() != 0 || vel_temp.getY() != 0) {
		vel.set(vel_temp.getX(), vel_temp.getY());
		vel_temp.set(0,0);
	}
}

void Ball::render(SDL_Renderer* rR)
{
	//Show the dot
	dotTexture->render(rR, mPosX, mPosY);
}

bool Ball::checkMouse(int x, int y) {
	bool res = !(y < mPosY || y > mPosY + DOT_HEIGHT || x < mPosX || x > mPosX + DOT_WIDTH);
	return res;
}

Vector2 Ball::getCenter(){
	return Vector2(mPosX + DOT_WIDTH / 2, mPosY + DOT_HEIGHT / 2);
}

void Ball::setVelByPoint(int x, int y) {
	vel.setX((mPosX - x)/5);
	vel.setY((mPosY - y)/5);
}

void Ball::setCenter(Vector2 c) {
	mPosX = c.getX() - DOT_WIDTH / 2;
	mPosY = c.getY() - DOT_HEIGHT / 2;
}
