#include "BulletNorm.h"
#include "CFG.h"

BulletNorm::BulletNorm(float x, float y)
{
	//Initialize the offsets
	texture = nullptr;
	mPosX = x;
	mPosY = y;
	width = 8;
	height = 8;
	radious = 4;
	timer = 1000;
	scale = 1;
}

BulletNorm::~BulletNorm()
{
}

void BulletNorm::loadTexture(LTexture* texture) {
	this->texture = texture;
}

void BulletNorm::move()
{
	//Move the dot left or right
	mPosX += vel.getX();
	//Move the dot up or down
	mPosY += vel.getY();

	if (vel_temp.getX() != 0 || vel_temp.getY() != 0) {
		vel.set(vel_temp.getX(), vel_temp.getY());
		vel_temp.set(0, 0);
	}

	timer--;
}

void BulletNorm::render(SDL_Renderer* rR)
{
	//Show the dot
	texture->render(rR, mPosX, mPosY);
}

Vector2 BulletNorm::getCenter() {
	return Vector2(mPosX + width / 2 * scale, mPosY + height / 2 * scale);
}

void BulletNorm::setCenter(Vector2 c) {
	mPosX = c.getX() - width / 2 * scale;
	mPosY = c.getY() - height / 2 * scale;
}
