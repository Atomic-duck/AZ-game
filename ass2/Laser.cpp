#include "Laser.h"
Laser::Laser(){
	isFire = false;
	vel = 10;
	l = 0;
	flag = false;
}
Laser::~Laser(){}

void Laser::addReflexPoint(Vector2& p, Vector2& direct) {
	directs.push_back(new Vector2(direct));
	points.push_back(new Vector2(p));

	if (points.size() > 1) {
		l += (*points[points.size()-1] - *points[points.size() - 2]).length();
	}
}

void Laser::clear() {
	for (int i = 0; i < points.size(); i++) {
		delete points[i];
		delete directs[i];
		points[i] = nullptr;
		directs[i] = nullptr;
	}
	points.clear();
	directs.clear();
}

void Laser::addStartPoint(Vector2& p, Vector2& direct) {
	clear();
	addReflexPoint(p, direct);
	head = *points[0];
	tail = head;
	cp = 0;
	l = 0;
}

bool Laser::isLongEnought() {
	return l >= length;
}

void Laser::render(SDL_Renderer* rR) {
	if (isFire) {
		SDL_SetRenderDrawColor(rR, 0, 142, 88, 255);
		if (p_between.size() == 0) {
			SDL_RenderDrawLine(rR, tail.getX(), tail.getY(), head.getX(), head.getY());
		}
		else {
			for (int i = 1; i < p_between.size(); i++) {
				if (i == 0) SDL_RenderDrawLine(rR, tail.getX(), tail.getY(), points[i]->getX(), points[i]->getY());
				else if (i == p_between.size() - 1) SDL_RenderDrawLine(rR, head.getX(), head.getY(), points[i]->getX(), points[i]->getY());
				else SDL_RenderDrawLine(rR, points[i]->getX(), points[i]->getY(), points[i - 1]->getX(), points[i - 1]->getY());
			}
		}
	}
	else {
		SDL_SetRenderDrawColor(rR, 100, 142, 88, 255);
		for (int i = 1; i < points.size(); i++) {
			SDL_RenderDrawLine(rR, points[i]->getX(), points[i]->getY(), points[i - 1]->getX(), points[i - 1]->getY());
		}
	}
}
void Laser::update() {
	if (isFire) {
		if(flag) tail = tail + *points[p_t] * vel;
		head = head + *points[p_h] * vel;

		if (l_fire >= 30) {
			flag = true;
		}
		p_between.clear();

		if(p_h + 1 < points.size()) {
			if ((head - *points[p_h]).length() >= (*points[p_h + 1] - *points[p_h]).length()) {
				l_fire += (*points[p_h + 1] - *points[p_h]).length();
				p_between.push_back(points[p_h + 1]);
				head = *points[p_h + 1] + *directs[p_h + 1] * (head - *points[p_h]).length();
				p_h++;
			}
			if ((tail - *points[p_t]).length() >= (*points[p_t + 1] - *points[p_t]).length()) {
				tail = *points[p_t + 1] + *directs[p_t + 1] * (head - *points[p_t]).length();
				p_t++;
			}
		}

		if (l_fire >= length || cp+1 == points.size()) {
			finish = true;
		}
	}
}
