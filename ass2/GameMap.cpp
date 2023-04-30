#include "GameMap.h"
#include "CFG.h"
#include <time.h>

GameMap::GameMap() {
	srand(time(0));
	p1_initX = p1_initY = p2_initX = p2_initY = -1;
	tile = TILE_SIZE;
	offsetX = 10;
	offsetY = 20;
	interval = rand() % 500 + 100;
	wl = 45;
	hl = 40;
}

void GameMap::render(SDL_Renderer* rR, LTexture* texture) {
	if (texture->getHeight() != (int)hl*scale) {
		texture->setWidth((float)wl * scale);
		texture->setHeight((float)hl * scale);
	}

	for (int i = 0; i < walls.size(); i++) {
		walls[i].render(rR);
	}
	for (int i = 0; i < laser_pos.size(); i++) {
		bool ig = false;
		for (int j = 0; j < ignore.size(); j++) {
			if (i == ignore[j]) ig = true;
		}
		if (ig) continue;

		texture->render(rR, laser_pos[i].first, laser_pos[i].second);
	}
}

void GameMap::checkGetLaser(Tank* tank) {
	std::vector<Vector2> corners = tank->getCorners();
	
	for (int k = 0; k < laser_pos.size(); k++) {
		for (int i = 0; i < corners.size(); i++) {
			bool ig = false;
			for (int j = 0; j < ignore.size(); j++) {
				if (k == ignore[j]) ig = true;
			}
			if (ig) continue;

			float dx = corners[i].getX() - (laser_pos[k].first + wl * scale / 2);
			float dy = corners[i].getY() - (laser_pos[k].second + hl * scale / 2);
			//printf("%f, %f\n", pow(dx, 2) + pow(dy, 2), pow(hl / 2 * scale, 2));
			if (pow(dx, 2) + pow(dy, 2) <= pow(hl / 2 * scale, 2)) {
				tank->initLaser();
				ignore.push_back(k);
			}
		}
	}
}

void GameMap::update() {
	if (interval-- <= 0) {
		laser_pos.push_back(std::pair<int, int>(rand() % width * tile + offsetX + 10*scale, rand() % height*tile + offsetY + 10*scale));
		interval = rand() % 500 + 100;
	}
}

void GameMap::reset() {
	map.clear();
	walls.clear();
	laser_pos.clear();
	ignore.clear();
	p1_initX = p1_initY = p2_initX = p2_initY = -1;
	tile = TILE_SIZE;
	offsetX = 10;
	offsetY = 20;
}

void GameMap::initializeTank(Tank* tank1, Tank* tank2) {
	tank1->setPos(p1_initX + 10, p1_initY + 10);
	tank1->setScale(scale);
	tank2->setPos(p2_initX + 10, p2_initY + 10);
	tank2->setScale(scale);
}

void GameMap::checkLaser(Laser* laser){
	if (!laser) return;
	int count = 100;
	while (!laser->isLongEnought() && count > 0) {
		float t_min = -1;
		int cur_typ = 0;
		Vector2 r_vel;
		Vector2 end;

		for (int i = 0; i < walls.size(); i++) {
			Vector2 l_vel;
			Vector2 l_end;
			float t = -1;
			int typ = walls[i].checkLaser(laser, t, l_vel, l_end);

			laser->getSize();
			//printf("S: %f\n", laser->getSize());
			//printf("L: %f\n", laser->getLength());
			if (typ != 0 && (t_min == -1 || t < t_min)) {
				if (typ == 2 && cur_typ == 1) continue;
				t_min = t;
				r_vel = l_vel;
				end = l_end;
				cur_typ = typ;
			}
		}

		if (t_min != -1) {
			laser->addReflexPoint(end, r_vel);
		}
		count--;
	}
}

void GameMap::checkTankCollision(Tank* tank) {
	for (int i = 0; i < walls.size(); i++) {
		walls[i].checkTankCollision(tank);
	}
	checkGetLaser(tank);
}

void GameMap::checkBullet(BulletNorm* bullet) {
	if(bullet) checkWallCollision(bullet);
}

void GameMap::checkWallCollision(BulletNorm* ball) {
	float t_min = -1;
	int cur_type = 0;
	Vector2 v = ball->getVel();
	Vector2 vtemp;
	for (int i = 0; i < walls.size(); i++) {
		ball->setVel(v);
		float t = 0;
		int type = walls[i].checkBulletCollision(ball, t);	// may need to optimize
		if (type != 0 && (t_min == -1 || t < t_min)) {
			if (type == 2 && cur_type == 1) continue;
			t_min = t;
			vtemp = ball->getTempVel();
			cur_type = type;
		}
	}

	if (t_min != -1) ball->setVelTemp(vtemp);
}


void GameMap::createWalls() {
	std::bitset<4> edge[4] = { std::bitset<4>{0b1000}, std::bitset<4>{0b0100}, std::bitset<4>{0b0010}, std::bitset<4>{0b0001} };

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			std::bitset<4> cell = map[i][j];
			int x = j * tile + offsetX;
			int y = i * tile + offsetY;

			if ((cell & edge[0]) != 0) {
				walls.push_back(Wall(x, y - Wall::w / 2, x + tile, y - Wall::w / 2));
			}
			if ((cell & edge[1]) != 0) {
				walls.push_back(Wall(x + tile + Wall::w / 2, y - Wall::w / 2, x + tile + Wall::w / 2, y + tile + Wall::w / 2));
			}
			if ((cell & edge[2]) != 0) {
				walls.push_back(Wall(x, y + tile - Wall::w / 2, x + tile, y + tile - Wall::w / 2));
			}
			if ((cell & edge[3]) != 0) {
				walls.push_back(Wall(x + Wall::w / 2, y - Wall::w / 2, x + Wall::w / 2, y + tile + Wall::w / 2));
			}
		}
	}
}

void GameMap::createMap(Tank* tank1, Tank* tank2) {
	width = 5 + (rand() % 10);
	height = 5 + (rand() % 10);
	int w_d = CCFG::GAME_WIDTH - width * tile;
	int h_d = CCFG::GAME_HEIGHT - 100 - height * tile;
	while (w_d < offsetX * 2 || h_d < offsetY * 2) {
		tile -= 1;
		w_d = CCFG::GAME_WIDTH - width * tile;
		h_d = CCFG::GAME_HEIGHT - 100 - height * tile;
	}
	offsetX = w_d / 2;
	offsetY = h_d / 2;
	printf("\n $%d, %d - %d\n", w_d, h_d, tile);

	map.clear();
	std::bitset<4> edge[4] = { std::bitset<4>{0b1000}, std::bitset<4>{0b0100}, std::bitset<4>{0b0010}, std::bitset<4>{0b0001} };
	int r_axis[2] = { -1,0 };
	int c_axis[2] = { 0,-1 };

	for (int i = 0; i < height; i++) {
		std::vector<std::bitset<4>> row;
		for (int j = 0; j < width; j++) {
			int val = rand() % 16;
			std::bitset<4> cell(val);

			if (i == 0) cell |= edge[0];
			if (i == height - 1) cell |= edge[2];
			if (j == 0) cell |= edge[3];
			if (j == width - 1) cell |= edge[1];

			for (int k = 0; k < 2; k++) {
				int r_idx = i + r_axis[k];
				int c_idx = j + c_axis[k];
				if (r_idx < 0 || c_idx < 0) continue;

				if (k == 0 && ((cell & edge[0]) != 0 || (map[r_idx][c_idx] & edge[2]) != 0)) {
					cell |= edge[0];
					map[r_idx][c_idx] |= edge[2];
				}
				if (k == 1 && ((cell & edge[3]) != 0 || (row.back() & edge[1]) != 0)) {
					cell |= edge[3];
					row.back() |= edge[1];
				}
			}

			row.push_back(cell);
		}
		map.push_back(row);
	}

	createPath();
	createWalls();
	scale = (float)tile / TILE_SIZE;
	initializeTank(tank1, tank2);
}

void GameMap::createPath() {
	float num_cell = width * height;
	int path_cell = floor(num_cell * (rand() % 51 + 50) / 100);
	int num = 1;
	int r_idx = rand() % height;
	int c_idx = rand() % width;
	std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));
	int p1_num = rand() % (path_cell - 2) + 2;
	int p2_num = rand() % (path_cell - 4) + 4;

	printf("%d ", path_cell);
	visited[r_idx][c_idx] = true;
	dfsPath(visited, num, path_cell, r_idx, c_idx, p1_num, p2_num);
	printf("%d ", num);
}

void GameMap::dfsPath(std::vector<std::vector<bool>>& visited, int& num, int& total, int r, int c, int& p1_num, int& p2_num) {
	if (num >= total) return;
	if (p1_initX == -1 && --p1_num == 0) {
		p1_initX = c * tile + offsetX;
		p1_initY = r * tile + offsetY;
	}
	if (p2_initX == -1 && --p2_num == 0) {
		p2_initX = c * tile + offsetX;
		p2_initY = r * tile + offsetY;
	}

	//printf("cell: %d, %d\n", r, c);
	//int v_count = 0;
	//int v_able = 4;
	int r_axis[4] = { -1,0,1,0 };
	int c_axis[4] = { 0,1,0,-1 };
	std::bitset<4> cell = map[r][c];
	std::bitset<4> edge[4] = { std::bitset<4>{0b1000}, std::bitset<4>{0b0100}, std::bitset<4>{0b0010}, std::bitset<4>{0b0001} };
	std::vector<int> arr;

	for (int i = 0; i < 4; i++) {
		int r_idx = r + r_axis[i];
		int c_idx = c + c_axis[i];

		if (r_idx < 0 || r_idx >= height || c_idx < 0 || c_idx >= width) {
			//v_able--;
			continue;
		}

		if (visited[r_idx][c_idx]) {
			//v_count++;
			continue;
		}

		//printf("# %d, %d # ", (cell & edge[i]) == 0, (map[r_idx][c_idx] & edge[(i + 2) % 4]) == 0);
		if ((cell & edge[i]) == 0) {
			num++;
			visited[r_idx][c_idx] = true;

			//printf("1 - ");
			dfsPath(visited, num, total, r_idx, c_idx, p1_num, p2_num);
			if (num >= total) return;
		}
		else {
			arr.push_back(i);
		}
	}

	if (arr.size() > 0) {
		int s = rand() % arr.size() + 1;
		for (int k = 0; k < s; k++) {
			int i = arr[k];
			int r_idx = r + r_axis[i];
			int c_idx = c + c_axis[i];
			map[r][c] &= ~edge[i];
			map[r_idx][c_idx] &= ~edge[(i + 2) % 4];

			num++;
			visited[r_idx][c_idx] = true;

			//printf("2 - ");
			dfsPath(visited, num, total, r_idx, c_idx, p1_num, p2_num);
		}
	}
}