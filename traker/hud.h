/***********
*Xiaohong Zhang
*CPSC 6160, S19
*xiaohoz@g.clemson.edu
*Asg #6
*Menu and restart
************/
#ifndef HUD_H
#define HUD_H
#include <iostream>
#include <SDL2/SDL.h>
#include "ioMod.h"
// Meyers Singleton
class Hud {
public:
	static Hud& getInstance();
	~Hud() {}
	void draw();
	void changeVisible();
	Hud(const Hud&) = delete;
	Hud& operator=(const Hud&) = delete;

private:
	SDL_Renderer* renderer;
	IoMod& io;
	int x;
	int y;
	int width;
	int height;
	bool visible;
	Hud(); //place constructors in private section


};
#endif