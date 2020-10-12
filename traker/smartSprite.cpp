/***********
*Xiaohong Zhang
*CPSC 6160, S19
*xiaohoz@g.clemson.edu
*Asg #6
*Menu and restart
************/
#include <cmath>
#include <random>
#include <functional>
#include "smartSprite.h"
#include "gameData.h"
#include "renderContext.h"


float distance(float x1, float y1, float x2, float y2) {
  float x = x1-x2;
  float y = y1-y2;
  return hypot(x, y);
}


void SmartSprite::goLeft() { setVelocityX(-abs(getVelocityX())); }
void SmartSprite::goRight() { setVelocityX(fabs(getVelocityX())); }
void SmartSprite::goUp() { setVelocityY(-abs(getVelocityY())); }
void SmartSprite::goDown() { setVelocityY(fabs(getVelocityY())); }

//convert constructor
SmartSprite::SmartSprite(const std::string& name, const Vector2f& pos,
  int w, int h) :
  TwoWayMultiSprite(name),
  playerPos(pos),
  playerWidth(w),
  playerHeight(h),
  currentMode(NORMAL),
  safeDistance(Gamedata::getInstance().getXmlFloat(name+"/safeDistance"))
{}


SmartSprite::SmartSprite(const SmartSprite& s):
  TwoWayMultiSprite(s),
  playerPos(s.playerPos),
  playerWidth(s.playerWidth),
  playerHeight(s.playerHeight),
  currentMode(s.currentMode),
  safeDistance(s.safeDistance)
{}


void SmartSprite::update(Uint32 ticks) {
  TwoWayMultiSprite::update(ticks);
  float x = getX()+getImage()->getWidth()/2; //width of one sprite
  float y = getY()+getImage()->getHeight()/2;
  float ex = playerPos[0]+playerWidth/2;
  float ey = playerPos[1]+playerHeight/2;
  float distanceToEnemy = ::distance(x, y, ex, ey);

  if (currentMode == NORMAL) {
    if (distanceToEnemy < safeDistance) currentMode = EVADE; 
  }
  else if (currentMode == EVADE) {
    if (distanceToEnemy < safeDistance) {
      if (x < ex) goLeft();
      if (x > ex) goRight();
      if (y < ey) goUp();
      if (y > ey) goDown();
    }
  }
}



































































