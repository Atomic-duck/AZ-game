#include "Explosion.h"

Explosion::Explosion(float x, float y, float scale) {
	width = 70;
	height = 70;
	this->scale = scale;
	this->x = x - (float)width*scale/2;
	this->y = y - (float)height*scale/2;
	cur_frame = 0;
	finish = false;
	for (int i = 0; i < FRAMES; i++) {
		SDL_Rect r = { i * width, 0, width, height };
		clips.push_back(r);
	}
}

Explosion::~Explosion() {

}

bool Explosion::isFinish() {
	return finish;
}

void Explosion::render(SDL_Renderer* rR, LTexture* texture) {
	texture->setWidth((int)width * scale);
	texture->setHeight((int)height * scale);

	texture->render2(rR, x, y, &clips[(int)cur_frame]);
}
void Explosion::update() {
	cur_frame += 0.2;
	if (cur_frame >= FRAMES) {
		finish = true;
	}
}