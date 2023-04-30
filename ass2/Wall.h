#pragma once

#include "Vector2.h"
#include "header.h"
#include "BulletNorm.h"
#include "Tank.h"
#include "Laser.h"
#include <vector>

class Wall
{
public:
	static const int w = 4;
	Wall(int x, int y, int x2, int y2);

	int checkBulletCollision(BulletNorm* ball, float& t);
	int checkLaser(Laser* laser, float& t, Vector2& l_vel, Vector2& l_end);
	void checkTankCollision(Tank* tank);
	void render(SDL_Renderer* rR);
private:
	std::vector<Vector2> points;
	SDL_Rect rect;
};


