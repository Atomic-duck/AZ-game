#pragma once

#ifndef CORE_H
#define CORE_H

#include "header.h"
#include "Texture.h"
#include "GameMap.h"
#include "Tank.h"
#include "Explosion.h"

class CCore
{
private:
	SDL_Window* window;
	SDL_Renderer* rR;
	TTF_Font* gFont;
	SDL_Event e;
	GameMap map;
	Tank* p1;
	Tank* p2;
	LTexture* explo_texture;
	LTexture* laser_p;
	LTexture p1_logo;
	LTexture p1_score_tx;
	LTexture p2_logo;
	LTexture p2_score_tx;
	int p1_score, p2_score;

	std::vector<Explosion*> explos;
	Mix_Chunk* explo_sound;
	bool p1_alive, p2_alive;
	bool counddown;
	int interval;
	// ----- FPS -----

	long frameTime;
	static const int MIN_FRAME_TIME = 14;

	unsigned long lFPSTime;
	int iNumOfFPS, iFPS;

	// ----- FPS -----

	// ----- INPUT
	static bool movePressed, keyMenuPressed, keyS, keyW, keyA, keyD, keyShift;

	static bool keyAPressed, keyDPressed;
	// ----- true = RIGHT, false = LEFT
	bool firstDir;
	bool pull;
	bool drawing;
	bool loadMap;

	// ----- INPUT

	// ----- Methods
	void MouseInput();
	bool loadMedia();
public:
	CCore(void);
	~CCore(void);

	static bool quitGame;

	void mainLoop();
	
	void Update();
	void Draw();

	void resetMove();
	static void resetKeys();

	static bool mouseLeftPressed, mouseRightPressed;
	static int mouseX, mouseY;

	/* ----- get & set ----- */
};

#endif

