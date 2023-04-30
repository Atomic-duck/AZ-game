#include "Core.h"
#include "CFG.h"

/* ******************************************** */

bool CCore::mouseLeftPressed = false;
bool CCore::mouseRightPressed = false;
int CCore::mouseX = 0;
int CCore::mouseY = 0;
bool CCore::quitGame = false;

bool CCore::movePressed = false;
bool CCore::keyMenuPressed = false;
bool CCore::keyS = false;
bool CCore::keyW = false;
bool CCore::keyA = false;
bool CCore::keyD = false;
bool CCore::keyShift = false;
bool CCore::keyAPressed = false;
bool CCore::keyDPressed = false;
const int INTERVAL = 300;

CCore::CCore(void) {
	loadMap = true;
	this->quitGame = false;
	this->iFPS = 0;
	this->iNumOfFPS = 0;
	this->lFPSTime = 0;

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO);

	window = SDL_CreateWindow("uMario - www.LukaszJakowski.pl", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, CCFG::GAME_WIDTH, CCFG::GAME_HEIGHT, SDL_WINDOW_SHOWN);

	if (window == NULL) {
		quitGame = true;
	}

	rR = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
	}

	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}

	//Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
	}
	// Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	// CCFG::getSMBLOGO()->setIMG("super_mario_bros", rR);

	//CCFG::getMusic()->PlayMusic();

	this->keyMenuPressed = this->movePressed = this->keyS = this->keyW = this->keyA = this->keyD = this->keyShift = false;

	this->keyAPressed = this->keyDPressed = this->firstDir = this->pull = false;

	this->mouseX = this->mouseY = 0;

	p1 = new Tank("images/t-red-norm.png", 1);
	p2 = new Tank("images/t-green-norm.png", 2);
	explo_texture = new LTexture();
	laser_p = new LTexture();
	explo_sound = nullptr;
	p1_alive = p2_alive = true;
	counddown = false;
	interval = INTERVAL;
	p1_score = p2_score;
}

CCore::~CCore(void) {
	delete p1;
	delete p2;
	delete explo_texture;
	delete laser_p;
	Mix_FreeChunk(explo_sound);
	
	//Destroy window	
	SDL_DestroyRenderer(rR);
	SDL_DestroyWindow(window);
	window = NULL;
	rR = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
	Mix_Quit();
}

/* ******************************************** */

void CCore::Update() {
	for (int i = 0; i < explos.size(); i++) {
		explos[i]->update();
		if (explos[i]->isFinish()) {
			delete explos[i];
			explos.erase(explos.begin() + i);
		}
	}
}


void CCore::Draw() {
	map.render(rR, laser_p);
	p1->renderBullet(rR);
	p2->renderBullet(rR);
	if(p1_alive) p1->render(rR);
	if(p2_alive) p2->render(rR);
	for (int i = 0; i < explos.size(); i++) {
		explos[i]->render(rR, explo_texture);
	}
}

void CCore::mainLoop() {
	lFPSTime = SDL_GetTicks();
	if (!loadMedia())
	{
		printf("Failed to load media!\n");
	}
	else
	{
		while (!quitGame) {
			frameTime = SDL_GetTicks();
			if (loadMap) {
				map.createMap(p1, p2);
				loadMap = false;
			}
			//Handle events on queue
			while (SDL_PollEvent(&e) != 0)
			{
				//User requests quit
				if (e.type == SDL_QUIT)
				{
					quitGame = true;
				}
				if(p1_alive) p1->handleEvent(e);
				if(p2_alive) p2->handleEvent(e);
				//MouseInput();
			}

			//
			p1->update();
			p2->update();
			map.update();
			map.checkTankCollision(p1);
			map.checkTankCollision(p2);
			map.checkLaser(p1->getLaser());
			map.checkLaser(p2->getLaser());

			std::vector<BulletNorm*> bullets_p[] = {p1->getBullet(), p2->getBullet()};
			for (int i = 0; i < 2; i++) {
				if (counddown) break;
				for (int j = 0; j < bullets_p[i].size(); j++) {
					map.checkBullet(bullets_p[i][j]);
					p1_alive = !p1->checkBulletCollision(bullets_p[i][j]);
					p2_alive = !p2->checkBulletCollision(bullets_p[i][j]);
					if (!p1_alive || !p2_alive) {
						if (i == 0) {
							p1->destroyBullet(j);
						}
						if (i == 1) {
							p2->destroyBullet(j);
						}
						
						if (!p1_alive) {
							explos.push_back(new Explosion(p1->getCenPosX(), p1->getCenPosY(), map.getScale()));
							Mix_PlayChannel(-1, explo_sound, 0);
						}
						if (!p2_alive) {
							explos.push_back(new Explosion(p2->getCenPosX(), p2->getCenPosY(), map.getScale()));
							Mix_PlayChannel(-1, explo_sound, 0);
						}

						counddown = true;
						break;
					}
				}
			}
			
			Update();
			//Move the dot

			//Clear screen
			SDL_SetRenderDrawColor(rR, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(rR);

			//Render objects
			Draw();
			p1_logo.render(rR, CCFG::GAME_WIDTH/2 - 150-142, CCFG::GAME_HEIGHT - 100);
			p1_score_tx.render(rR, CCFG::GAME_WIDTH / 2 - 130, CCFG::GAME_HEIGHT - 70);
			p2_logo.render(rR, CCFG::GAME_WIDTH/2 + 150, CCFG::GAME_HEIGHT - 95);
			p2_score_tx.render(rR, CCFG::GAME_WIDTH / 2 + 100, CCFG::GAME_HEIGHT - 70);

			//Update screen
			SDL_RenderPresent(rR);

			if (counddown) interval--;
			if (interval == 0) {
				counddown = false;
				SDL_Color textColor = { 0, 0, 0 };
				interval = INTERVAL;
				loadMap = true;
				if (p1_alive) {
					p1_score++; 
					p1_score_tx.loadFromRenderedText(rR, gFont, std::to_string(p1_score), textColor);
				}
				if (p2_alive) { 
					p2_score++;
					p2_score_tx.loadFromRenderedText(rR, gFont, std::to_string(p2_score), textColor);
				}
				p1_alive = true;
				p2_alive = true;
				p1->reset();
				p2->reset();
				map.reset();
			}

			if (SDL_GetTicks() - frameTime < MIN_FRAME_TIME) {
				SDL_Delay(MIN_FRAME_TIME - (SDL_GetTicks() - frameTime));
			}

		}
	}
}

bool CCore::loadMedia() {
	
	//Loading success flag
	bool success = true;
	
	explo_sound = Mix_LoadWAV("sound/explosion.wav");
	if (explo_sound == NULL)
	{
		printf("Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}
	p1_logo.loadFromFile(rR, "images/p1_logo.png");
	p2_logo.loadFromFile(rR, "images/p2_logo.png");
	laser_p->loadFromFile(rR, "images/laser_p.png");

	//Open the font
	gFont = TTF_OpenFont("font/neuropol.otf", 28);
	if (gFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else
	{
		//Render text
		SDL_Color textColor = { 0, 0, 0 };
		if (!p1_score_tx.loadFromRenderedText(rR, gFont, std::to_string(p1_score), textColor))
		{
			printf("Failed to render text texture!\n");
			success = false;
		}
		if (!p2_score_tx.loadFromRenderedText(rR, gFont, std::to_string(p1_score), textColor))
		{
			printf("Failed to render text texture!\n");
			success = false;
		}
	}

	explo_texture->loadFromFile(rR, "images/Explosion2.png");
	p1->loadTexture(rR);
	p2->loadTexture(rR);
	
	return success;
}

void CCore::MouseInput() {
	switch (e.type) {
	case SDL_MOUSEBUTTONDOWN: {
		switch (e.button.button) {
		case SDL_BUTTON_LEFT:
			mouseLeftPressed = true;
			break;
		case SDL_BUTTON_RIGHT:
			mouseRightPressed = true;
			break;
		}

		break;
	}
	case SDL_MOUSEMOTION: {

		SDL_GetMouseState(&mouseX, &mouseY);
		break;
	}
	case SDL_MOUSEBUTTONUP: {
		switch (e.button.button) {
		case SDL_BUTTON_LEFT:
			mouseLeftPressed = false;
			break;
		case SDL_BUTTON_RIGHT:
			mouseRightPressed = false;
			break;
		}
		break;
	}
	case SDL_MOUSEWHEEL:
		if (e.wheel.timestamp > SDL_GetTicks() - 2) {
			//CCFG::getMM()->getLE()->mouseWheel(e.wheel.y);
		}
		break;
	}
}

/* ******************************************** */
void CCore::resetKeys() {
	movePressed = keyMenuPressed = keyS = keyW = keyA = keyD = CCFG::keySpace = keyShift = keyAPressed = keyDPressed = false;
}

void CCore::resetMove() {
	this->keyAPressed = this->keyDPressed = false;
}
