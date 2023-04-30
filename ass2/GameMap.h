
#pragma once

#ifndef WALLS_H
#define WALLS_H

#include "header.h"
#include "Wall.h"
#include "CFG.h"
#include "Tank.h"
#include "BulletNorm.h"
#include "Laser.h"
#include <vector>
#include <bitset>

class GameMap
{
private:
	static const int TILE_SIZE = 80;
	std::vector<Wall> walls;
	std::vector<std::vector<std::bitset<4>>> map;
	std::vector<std::pair<int, int>> laser_pos;
	std::vector<int> ignore;
	int width, height, offsetX, offsetY;
	float p1_initX, p1_initY, p2_initX, p2_initY, scale;
	int tile;
	float interval;
	float wl, hl;

	void dfsPath(std::vector<std::vector<bool>>& visited, int& num, int& total, int r, int c, int& p1_num, int& p2_num);
	void createPath();
	void createWalls();
	void initializeTank(Tank* tank1, Tank* tank2);
public:
	GameMap();
	void reset();
	void render(SDL_Renderer* rR, LTexture* texture);
	void update();

	void checkBullet(BulletNorm* bullet);
	void checkWallCollision(BulletNorm* bullet);
	void checkTankCollision(Tank* tank);
	void checkLaser(Laser* laser);
	void checkGetLaser(Tank* tank);

	void createMap(Tank* tank1, Tank* tank2);
	float getScale() { return scale; }
};

#endif
