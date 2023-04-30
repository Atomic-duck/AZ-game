#pragma once

#ifndef CFG_H
#define CFG_H

#include "header.h"
//#include "Music.h"

class CCFG
{
private:
	//static Music* oMusic;
public:
	CCFG(void);
	~CCFG(void);

	static int GAME_WIDTH, GAME_HEIGHT;

	static bool keySpace;

	static int keyIDA, keyIDS, keyIDD, keyIDSpace, keyIDShift;

	static std::string getKeyString(int keyID);

	//static Music* getMusic();

	static bool canMoveBackward;
};

#endif

