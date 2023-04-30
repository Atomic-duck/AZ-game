#pragma once

#include "header.h"
#include "Texture.h"
#include <vector>

class Explosion
{
public:
	static const int FRAMES = 12;
	Explosion(float x, float y, float scale);
	~Explosion();

	void render(SDL_Renderer* rR, LTexture* texture);
	void update();
	bool isFinish();
private:
	std::vector<SDL_Rect> clips;
	int width, height;
	float cur_frame;
	float x, y, scale;
	bool finish;
};

