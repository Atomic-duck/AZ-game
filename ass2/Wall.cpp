
#include "Wall.h"
#include <algorithm>

Wall::Wall(int x1, int y1, int x2, int y2) {
	points.push_back(Vector2());
	points.push_back(Vector2());
	points.push_back(Vector2());
	points.push_back(Vector2());

	points[0].set(x1, y1);
	points[3].set(x2, y2);
	float radian_d = M_PI / 2;
	Vector2 v = (points[3] - points[0]).normalize();
	Vector2 v2 = Vector2(v.getX() * cos(radian_d) - v.getY() * sin(radian_d), v.getX() * sin(radian_d) + v.getY() * cos(radian_d));
	points[1] = points[0] + v2 * w;
	points[2] = points[3] + v2 * w;
	points[1].floor();
	points[2].floor();

	if (x1 == x2) {
		rect.x = x1 - w;
		rect.y = y1;
		rect.w = w;
		rect.h = y2 - y1;
	}
	else {
		rect.x = x1;
		rect.y = y1;
		rect.w = x2 - x1;
		rect.h = w;
	}
}

int Wall::checkLaser(Laser* laser, float& t, Vector2& l_vel, Vector2& l_end) {
	Vector2 vel_n = *(laser->getLastDirect());
	Vector2 vel = vel_n * 5;
	Vector2 start = *(laser->getLastPoint());
	Vector2 r_vel;
	Vector2 end;
	float min_t = -1;
	int res = 0;

	for (int i = 0; i < 4; i++) {
		Vector2 p1 = points[i];
		Vector2 p2 = points[(i + 1) % 4];
		Vector2 n1 = Vector2(p2.getY() - p1.getY(), -(p2.getX() - p1.getX())).normalize();
		float alpha = acosf(vel_n.dotVector(n1));
		if (alpha > M_PI / 2) {
			n1 = -1 * n1;
		}

		float tr = 1 / (vel.length() * vel_n.dotVector(n1));
		Vector2 edge = start + vel * tr;

		float t = (-n1.getY() * edge.getY() - n1.getX() * edge.getX() - (-n1.getY() * p1.getY() - n1.getX() * p1.getX())) / (n1.getX() * vel.getX() + n1.getY() * vel.getY());
		//
		if (t > 0) {
			Vector2 cp = (edge + vel * t);
			cp.floor();
			float xmin = std::min(p1.getX(), p2.getX());
			float xmax = std::max(p1.getX(), p2.getX());
			float ymin = std::min(p1.getY(), p2.getY());
			float ymax = std::max(p1.getY(), p2.getY());

			int typ = 0;
			if (p1.getY() == p2.getY() && cp.getX() >= xmin && cp.getX() <= xmax) {
				typ = 1;
			}
			else if (p1.getX() == p2.getX() && cp.getY() >= ymin && cp.getY() <= ymax) {
				typ = 1;
			}
			else if (p1.getY() != p2.getY() && p1.getX() != p2.getX() && cp.getX() >= xmin && cp.getX() <= xmax && cp.getY() >= ymin && cp.getY() <= ymax) {
				typ = 1;
			}
			else {
				Vector2 x;
				if ((p1 - start).length() > (p2 - start).length())
					x = p2 - start;
				else
					x = p1 - start;


				float delta = powf(2 * vel.dotVector(x), 2) - 4 * vel.dotVector(vel) * (x.dotVector(x));

				if (delta >= 0) {
					float t1 = (2 * vel.dotVector(x) + sqrtf(delta)) / (2 * vel.dotVector(vel));
					float t2 = (2 * vel.dotVector(x) - sqrtf(delta)) / (2 * vel.dotVector(vel));
					t = std::min(t1, t2);

					n1 = (start - (x - vel * t)).normalize();
					typ = 2;
				}
			}

			if (typ != 0 && (min_t == -1 || t < min_t)) {
				res = typ;
				min_t = t - 0.005;
				r_vel = vel - 2 * n1 * vel.dotVector(n1);
				end = start + vel * t;
				end.floor();
			}
		}
	}

	if (res != 0) {
		l_vel = r_vel.normalize();
		l_end = end;
		t = min_t;
	}

	return res;
}

int Wall::checkBulletCollision(BulletNorm* ball, float& t) {
	Vector2 center = ball->getCenter();
	Vector2 vel = ball->getVel();
	Vector2 vel_n = vel.normalize();
	float radious = ball->getRadious();
	float min_t = 10000;
	Vector2 r_vel;
	Vector2 r_center;
	int res = 0;

	if (vel.getX() == 0 && vel.getY() == 0) return 0;
	//printf("#####\n");
	for (int i = 0; i < 4; i++) {
		Vector2 p1 = points[i];
		Vector2 p2 = points[(i + 1) % 4];
		Vector2 n1 = Vector2(p2.getY() - p1.getY(), -(p2.getX() - p1.getX())).normalize();
		float alpha = acosf(vel_n.dotVector(n1));
		if (alpha > M_PI / 2) {
			n1 = -1 * n1;
		}

		float tr = radious / (vel.length() * vel_n.dotVector(n1));
		Vector2 edge = center + vel * tr;

		float t = (-n1.getY() * edge.getY() - n1.getX() * edge.getX() - (-n1.getY() * p1.getY() - n1.getX() * p1.getX())) / (n1.getX() * vel.getX() + n1.getY() * vel.getY());

		if (t >= 0 && t <= 1) {
			Vector2 cp = (edge + vel * t);
			cp.floor();
			float xmin = std::min(p1.getX(), p2.getX());
			float xmax = std::max(p1.getX(), p2.getX());
			float ymin = std::min(p1.getY(), p2.getY());
			float ymax = std::max(p1.getY(), p2.getY());

			int typ = 0;
			if (p1.getY() == p2.getY() && cp.getX() >= xmin && cp.getX() <= xmax) {
				typ = 1;
			}
			else if (p1.getX() == p2.getX() && cp.getY() >= ymin && cp.getY() <= ymax) {
				typ = 1;
			}
			else if (p1.getY() != p2.getY() && p1.getX() != p2.getX() && cp.getX() >= xmin && cp.getX() <= xmax && cp.getY() >= ymin && cp.getY() <= ymax) {
				typ = 1;
			}
			else {
				Vector2 x;
				if ((p1 - center).length() > (p2 - center).length())
					x = p2 - center;
				else
					x = p1 - center;


				float delta = powf(2 * vel.dotVector(x), 2) - 4 * vel.dotVector(vel) * (x.dotVector(x) - powf(radious, 2));

				if (delta >= 0) {
					float t1 = (2 * vel.dotVector(x) + sqrtf(delta)) / (2 * vel.dotVector(vel));
					float t2 = (2 * vel.dotVector(x) - sqrtf(delta)) / (2 * vel.dotVector(vel));
					t = std::min(t1, t2);

					if (t >= 0 && t <= 1) {
						n1 = (center - (x - vel * t)).normalize();
						typ = 2;
						//printf("%f\n", t);
					}
				}
			}
			//printf("CP: %f, %f\n", cp.getX(), cp.getY());
			//printf("P1: %f, %f | P2: %f, %f\n", p1.getX(), p1.getY(), p2.getX(), p2.getY());
			//printf("Reflex: %d\n", reflex);

			if (typ != 0 && (min_t == -1 || t < min_t)) {
				res = typ;
				min_t = t;
				r_vel = vel - 2 * n1 * vel.dotVector(n1);
				r_center = center + vel * t;
				r_center.floor();
			}
		}
	}

	if (res != 0) {
		ball->setCenter(r_center);
		ball->setVel(Vector2(0, 0));
		ball->setVelTemp(r_vel);
		t = min_t;
	}

	return res;
}

void Wall::render(SDL_Renderer* rR) {
	SDL_SetRenderDrawColor(rR, 0, 0, 0, 255);
	for (int i = 0; i < 4; i++) {
		SDL_RenderDrawLine(rR, points[(i + 1)%4].getX(), points[(i + 1)%4].getY(), points[i].getX(), points[i].getY());
	}
	SDL_RenderFillRect(rR, &rect);
}

void Wall::checkTankCollision(Tank* tank) {
	std::vector<Vector2> corners = tank->getCorners();
	for (int i = 0; i < corners.size(); i++) {
		float x = corners[i].getX();
		float y = corners[i].getY();
		if (!(x < rect.x || x > rect.x + rect.w || y < rect.y || y > rect.y + rect.h)) {
			tank->undoMove();
			return;
		}
	}
}