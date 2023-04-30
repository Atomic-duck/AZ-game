#include "Tank.h"
#include "CFG.h"

Tank::Tank(std::string src, int p)
{
	//Initialize the offsets
	this->src = src;
	this->p = p;

	tankTexture = new LTexture();
	mPosX = 200;
	mPosY = 200;
	width = WIDTH;
	height = HEIGHT;
	b1 = 11;
	b2 = 19;
	head = 7;

	alpha = 0;
	speed = 0;
	scale = 1;
	turn_speed = 3;
	turn_left = false;
	turn_right = false;
	center.set(15, 21 + 7);

	bnum = 3;
	bullet_texture = new LTexture();
	laser = nullptr;
}

Tank::~Tank()
{
	tankTexture->free();
	bullet_texture->free();
	delete tankTexture;
	delete bullet_texture;
	delete laser;
}
std::vector<BulletNorm*>& Tank::getBullet() {
	return bullets;
}

void Tank::initLaser() {
	if (!laser) {
		laser = new Laser();
	}
}

void Tank::destroyLaser() {
	delete laser;
	laser = nullptr;
}

bool Tank::checkBulletCollision(BulletNorm* bullet) {
	if (!bullet) return false;
	Vector2 bcenter = bullet->getCenter();
	float ra = bullet->getRadious();
	Vector2 v = bcenter - cenPos;
	bcenter = cenPos + Vector2(v.getX() * cos(-alpha * M_PI / 180) - v.getY() * sin(-alpha * M_PI / 180), v.getX() * sin(-alpha * M_PI / 180) + v.getY() * cos(-alpha * M_PI / 180));
	
	float dx = abs(bcenter.getX() - cenPos.getX());
	float dy = abs(bcenter.getY() - cenPos.getY());
	float hw = width * scale / 2;
	float hh = height * scale / 2;

	if (dx > hw + ra || dy > hh + ra) return false;
	if (dx <= hw || dy <= hh || (pow(dx - hw, 2) + pow(dy - hh, 2)) <= pow(ra, 2)) {
		return true;
	}
}

void Tank::update() {
	move();
	updateBullet();
	if (laser) {
		laser->update();
		if (laser->isFinish()) {
			destroyLaser();
		}
	}
}

void Tank::updateBullet() {
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i]->getTimer() <= 0) {
			destroyBullet(i);
		}
		else {
			bullets[i]->move();
		}
	}
}

void Tank::destroyBullet(int i) {
	delete bullets[i];
	bullets.erase(bullets.begin() + i);
}

void Tank::fire() {
	if (laser) {
		laser->fire();
	}
	else if (bullets.size() < bnum) {
		BulletNorm* bullet = new BulletNorm();
		bullet->loadTexture(bullet_texture);
		bullet->setScale(scale);
		Vector2 v(0, -1);
		bullet->setDirect(Vector2(v.getX() * cos(alpha * M_PI / 180) - v.getY() * sin(alpha * M_PI / 180), v.getX() * sin(alpha * M_PI / 180) + v.getY() * cos(alpha * M_PI / 180)).normalize());
		
		if (abs(alpha) <= 100) {
			v = Vector2(mPosX + b1 * scale, mPosY - bullet->getRadious() - 1) - cenPos;
		}
		else {
			v = Vector2(mPosX + b2 * scale, mPosY - bullet->getRadious() - 1) - cenPos;
		}
		v = cenPos + Vector2(v.getX() * cos(alpha * M_PI / 180) - v.getY() * sin(alpha * M_PI / 180), v.getX() * sin(alpha * M_PI / 180) + v.getY() * cos(alpha * M_PI / 180));
		bullet->setPosX(v.getX());
		bullet->setPosY(v.getY());
		//printf("%f, %f\n", v.getX(), v.getX());
		bullets.push_back(bullet);
	}
}

void Tank::setScale(float s) {
	scale = s;
	int x = width * s;
	int y = height * s;
	while (x % 2 == 0 && y % 2 == 0) {
		x++;
		s = (float)x / width;
		y = height * s;
	}

	printf("%d, %d",x, y);
	printf("WH: %f\n",scale);
	setScaleTexture();
}

void Tank::setScaleTexture() {
	tankTexture->setWidth((float)width * scale);
	tankTexture->setHeight((float)height * scale);
	bullet_texture->setWidth((float)BulletNorm::WIDTH * scale);
	bullet_texture->setHeight((float)BulletNorm::HEIGHT * scale);
}

void Tank::loadTexture(SDL_Renderer* rR) {
	tankTexture->loadFromFile(rR, src);
	bullet_texture->loadFromFile(rR, "images/dot.bmp");
	setScaleTexture();
}

void Tank::handleEvent(SDL_Event& e)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		//Adjust the velocity
		if (p == 1) {
			switch (e.key.keysym.sym)
			{
				case SDLK_UP: speed = TANK_VEL + 0.5; break;
				case SDLK_DOWN: speed = -TANK_VEL; break;
				case SDLK_LEFT: turn_left = true; break;
				case SDLK_RIGHT: turn_right = true; break;
				case SDLK_m: fire(); break;
			}
		}
		else if(p == 2) {
			switch (e.key.keysym.sym)
			{
				case SDLK_e: speed = TANK_VEL + 0.5; break;
				case SDLK_d: speed = -TANK_VEL; break;
				case SDLK_s: turn_left = true; break;
				case SDLK_f: turn_right = true; break;
				case SDLK_q: fire(); break;
			}
		}
	}
	//If a key was released
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
	//Adjust the velocity
		if (p == 1) {
			switch (e.key.keysym.sym)
			{
				case SDLK_UP: speed = 0; break;
				case SDLK_DOWN: speed = 0; break;
				case SDLK_LEFT: turn_left = false; break;
				case SDLK_RIGHT: turn_right = false; break;
			}
		}
		else if (p == 2) {
			switch (e.key.keysym.sym)
			{
				case SDLK_e: speed = 0; break;
				case SDLK_d: speed = 0; break;
				case SDLK_s: turn_left = false; break;
				case SDLK_f: turn_right = false; break;
			}
		}
	}
}

void Tank::move()
{
	if (turn_left) {
		alpha -= turn_speed;
		if (alpha < -360) {
			alpha = -360 - alpha;
		}
	}
	if (turn_right) {
		alpha += turn_speed;
		if (alpha > 360) {
			alpha = alpha-360;
		}
	}
	vel.set(speed * sinf(alpha * M_PI / 180), -speed * cosf(alpha * M_PI / 180));
	//Move the dot left or right
	mPosX += vel.getX();
	//Move the dot up or down
	mPosY += vel.getY();
	cenPos.setX(mPosX + center.getX() * scale);
	cenPos.setY(mPosY + center.getY() * scale);
}

void Tank::undoMove() {
	if (turn_left) {
		alpha += turn_speed;
	}
	if (turn_right) {
		alpha -= turn_speed;
	}
	//Move the dot left or right
	mPosX -= vel.getX();
	//Move the dot up or down
	mPosY -= vel.getY();
	cenPos.setX(mPosX + center.getX() * scale);
	cenPos.setY(mPosY + center.getY() * scale);
}

void Tank::reset() {
	for (int i = 0; i < bullets.size(); i++) {
		delete bullets[i];
	}
	bullets.clear();
	alpha = 0;
}

Laser* Tank::getLaser() {
	if (laser) {
		if (!laser->isFired()) {
			Vector2 v(0, -1);
			Vector2 vel(v.getX() * cos(alpha * M_PI / 180) - v.getY() * sin(alpha * M_PI / 180), v.getX() * sin(alpha * M_PI / 180) + v.getY() * cos(alpha * M_PI / 180));
			vel = vel.normalize();
			laser->addStartPoint(cenPos, vel);
		}
	}
	return laser;
}

void Tank::renderBullet(SDL_Renderer* rR) {
	for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->render(rR);
	}
	renderLaser(rR);
}

void Tank::renderLaser(SDL_Renderer* rR) {
	if(laser) laser->render(rR);
}

void Tank::render(SDL_Renderer* rR)
{
	SDL_Point c = { center.getX() * scale, center.getY() * scale};
	tankTexture->render(rR, mPosX, mPosY, NULL, alpha, &c);

	/*
	for (int i = 0; i < corners.size(); i++) {
		Vector2 v = corners[i] - cenPos;
		v = cenPos + Vector2(v.getX() * cos(alpha * M_PI / 180) - v.getY() * sin(alpha * M_PI / 180), v.getX() * sin(alpha * M_PI / 180) + v.getY() * cos(alpha * M_PI / 180));
		SDL_RenderDrawPoint(rR, v.getX(), v.getY());
	}
	SDL_RenderDrawPoint(rR, cenPos.getX(), cenPos.getY());
	*/
}

std::vector<Vector2> Tank::getCorners() {
	corners.clear();
	corners.push_back(Vector2(mPosX, mPosY + head *scale));
	corners.push_back(Vector2(mPosX, mPosY + (head+20) *scale));
	corners.push_back(Vector2(mPosX + width * scale, mPosY + head*scale));
	corners.push_back(Vector2(mPosX + width * scale, mPosY + (head + 20) * scale));
	corners.push_back(Vector2(mPosX + width* scale, mPosY + height* scale));
	corners.push_back(Vector2(mPosX, mPosY + height* scale));
	corners.push_back(Vector2(mPosX + b1 * scale, mPosY + head * scale));
	corners.push_back(Vector2(mPosX + b2 * scale, mPosY + head * scale));
	corners.push_back(Vector2(mPosX + b1 * scale, mPosY));
	corners.push_back(Vector2(mPosX + b2 * scale, mPosY));

	std::vector<Vector2> r_corners;
	for (int i = 0; i < corners.size(); i++) {
		Vector2 v = corners[i] - cenPos;
		r_corners.push_back(cenPos + Vector2(v.getX() * cos(alpha * M_PI / 180) - v.getY() * sin(alpha * M_PI / 180), v.getX() * sin(alpha * M_PI / 180) + v.getY() * cos(alpha * M_PI / 180)));
	}

	return r_corners;
}


