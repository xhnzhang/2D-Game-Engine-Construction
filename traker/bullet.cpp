/***********
*Xiaohong Zhang
*CPSC 6160, S19
*xiaohoz@g.clemson.edu
*Asg #6
*Menu and restart
************/
#include <iostream>
#include <cmath>
#include "bullet.h"
#include "gameData.h"
#include "imageFactory.h"

Bullet::Bullet( const string& name, const Vector2f&  pos,
                          const Vector2f& vel, float scale) :
   Sprite(name, pos, vel, ImageFactory::getInstance().getImage(name), scale),
   distance(0), 
   maxDistance(Gamedata::getInstance().getXmlInt(name+"/distance")), 
   tooFar(false) 
{ }

void Bullet::reset() {
  tooFar = false;
  distance = 0;
}

void Bullet::update(Uint32 ticks) { 
  Vector2f pos = getPosition();
  Sprite::update(ticks);
  if ( getY()+getScaledHeight() < 0 || getY() > worldHeight ) {
    tooFar = true;
  }
  if ( getX() < 0 || getX()+getScaledWidth() > worldWidth ) {
    tooFar = true;
  }  
  distance += ( hypot(getX()-pos[0], getY()-pos[1]) );
  if (distance > maxDistance) tooFar = true;
}

