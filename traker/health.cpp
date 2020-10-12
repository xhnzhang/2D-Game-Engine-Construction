/***********
*Xiaohong Zhang
*CPSC 6160, S19
*xiaohoz@g.clemson.edu
*Asg #6
*Menu and restart
************/
#include "health.h"
#include "renderContext.h"
#include "gameData.h"
#include "imageFactory.h"

Health::Health():
    renderer(RenderContext::getInstance().getRenderer()),
    io(IoMod::getInstance()),
    heart(ImageFactory::getInstance().getImage("Heart")),
    lives(Gamedata::getInstance().getXmlInt("Alien/initLives")),
    x(Gamedata::getInstance().getXmlInt("Heart/startLoc/x")), 
    y(Gamedata::getInstance().getXmlInt("Heart/startLoc/y"))
{}

// Health::~Health() {
//     delete heart;
// }

void Health::drawLives() const{
	SDL_Color c = {255,255,0,0};
	io.writeText("Lives: ", 30, 30, c);

    for (int i = 0; i < lives; ++i){
        //must use this draw() function
        heart->draw(0, 0, x+i*heart->getWidth()+5,y);
    }
}

void Health::showLose() const {

    // std::string fontFile = "./fonts/game.ttf";
    // int fontSize = 100;
    SDL_Color c = {255,0,0,0};
    // drawLives(120,20);
    io.writeTextGame("You Lose!", 100, 200, c);
    io.writeTextGame("Press R to restart or",  100, 300, c);
    io.writeTextGame("Esc to quit",  300, 400, c);
}


void Health::showWin() const {

    SDL_Color c = {255,0,0,0};
    // drawLives(120,20);
    io.writeTextGame("You Win!", 100, 200, c);
    io.writeTextGame("Press R to restart or",  100, 300, c);
    io.writeTextGame("Esc to quit",  300, 400, c);
}


















