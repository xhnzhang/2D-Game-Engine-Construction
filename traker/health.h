/***********
*Xiaohong Zhang
*CPSC 6160, S19
*xiaohoz@g.clemson.edu
*Asg #6
*Menu and restart
************/
#ifndef HEALTH__H
#define HEALTH__H

#include "ioMod.h"
#include "image.h"
#include <SDL2/SDL.h>


class Health {
public:
	static Health& getInstance(){
		static Health instance;
		return instance;
	}
	~Health() {};

	void setLives(int n){ lives = n; }
	void drawLives() const;
	void showLose() const;
	void showWin() const;

	Health(const Health&) = delete;
	Health& operator=(const Health&) = delete;
	
private:
	Health();
	SDL_Renderer* renderer;
	IoMod& io;
	Image* heart;
	int lives;
	int x;
	int y;
};

#endif