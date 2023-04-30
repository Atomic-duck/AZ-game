#pragma once
#include "header.h"
#include "Vector2.h"
#include <vector>

class Laser
{
public:
	Laser();
	~Laser();
	float length = 200;

	void render(SDL_Renderer* rR);
	void update();
	void addStartPoint(Vector2& p, Vector2& direct);
	void addReflexPoint(Vector2& p, Vector2& direct);
	void clear();
	Vector2* getStartDirect() { return directs[0]; }
	Vector2* getStartPoint() { return points[0]; }
	Vector2* getLastDirect() { return directs.back(); }
	Vector2* getLastPoint() { return points.back(); }
	bool isLongEnought();
	bool isFinish() { return finish; }
	void fire() { isFire = true;}
	bool isFired() { return isFire; }
	float getLength() { return l; }
	std::vector<Vector2*> getSize() { return points; }
private:
	std::vector<Vector2*> directs;
	std::vector<Vector2*> points;
	std::vector<Vector2*> p_between;
	Vector2 head, tail;
	int p_t, p_h;
	bool flag;

	int cp;
	float vel;
	float l, l_fire;
	bool isFire;
	bool finish;

};

