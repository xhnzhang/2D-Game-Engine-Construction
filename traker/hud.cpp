/***********
*Xiaohong Zhang
*CPSC 6160, S19
*xiaohoz@g.clemson.edu
*Asg #6
*Menu and restart
************/
#include "hud.h"
#include "renderContext.h"
#include "gameData.h"

Hud& Hud::getInstance() {
	static Hud instance;
	return instance;
}

Hud::Hud() :
	renderer(RenderContext::getInstance().getRenderer()),
	io(IoMod::getInstance()),
	x(Gamedata::getInstance().getXmlInt("hud/x")), 
	y(Gamedata::getInstance().getXmlInt("hud/y")),
	width(Gamedata::getInstance().getXmlInt("hud/w")),
	height(Gamedata::getInstance().getXmlInt("hud/h")),
	visible(true)
{}

void Hud::changeVisible(){
	if (visible) { visible = false; }
	else visible = true;
}

void Hud::draw() {

	if (!visible) return;
	// Now set the color for the hud:
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255/2);

	SDL_Rect r = {x, y, width, height};
	// SDL_Rect r;
	// r.x = x;
	// r.y = y;
	// r.w = width;
	// r.h = height;
	SDL_RenderFillRect(renderer, &r);
	//Now set the color for the outline of the hud:
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255/2);
	SDL_RenderDrawRect(renderer, &r);
	io.writeText("WSAD: Move", x+10, y+10);
	io.writeText("Space: Shoot", x+10, y+35);
	io.writeText("ESC: Quit", x+10, y+60);
	io.writeText("R: Restart", x+150, y+10);
	io.writeText("P: Pause", x+150, y+35);
	io.writeText("G: God mode", x+150, y+60);
	io.writeText("M: Menu", x+300, y+10);	
	io.writeText("F1: Show/Hide Hud", x+300, y+35);
	io.writeText("F4: Make Video", x+300, y+60);
	SDL_RenderPresent(renderer);//must have this, otherwise no show up
}












