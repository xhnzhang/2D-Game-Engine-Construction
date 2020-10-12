/***********
*Xiaohong Zhang
*CPSC 6160, S19
*xiaohoz@g.clemson.edu
*Asg #6
*Menu and restart
************/
#include <iostream>
#include <cmath>
#include "bulletPool.h"
#include "imageFactory.h"
#include "collisionStrategy.h"
#include "gameData.h"

BulletPool::BulletPool(const std::string& n) :
  name(n),
  myVelocity(
    Gamedata::getInstance().getXmlInt(name+"/speed/x"), 
    Gamedata::getInstance().getXmlInt(name+"/speed/y")
  ),
  bulletList(),
  freeList(),
  strategy(NULL)
{ 
  const string thisStrategy = 
    Gamedata::getInstance().getXmlStr(name+"/collisionStrategy");
  if ( thisStrategy == "PerPixel" ) {
    strategy = new PerPixelCollisionStrategy;
  }
  else if ( thisStrategy == "Rectangular" ) {
    strategy = new RectangularCollisionStrategy;
  }
  else if ( thisStrategy == "MidPoint" ) {
    strategy = new MidPointCollisionStrategy;
  }
}
/*implement bullet pool here*/
void BulletPool::shoot(const Vector2f& pos, const Vector2f& objVel) {
  //if pool is empty, make a bullet
  if (freeList.empty()) {
    Bullet b (name, pos, objVel, Gamedata::getInstance().getXmlFloat(name+"/scale")); 
    bulletList.emplace_back(b);
  }
  //if pool already has bullet
  else {
    Bullet b = freeList.front();
    freeList.pop_front();
    b.reset();
    b.setVelocity(objVel);
    b.setPosition(pos);
    bulletList.emplace_back(b); //used in player.cpp
  }
}

bool BulletPool::collidedWith(const Drawable* obj) const { 
  for ( const auto & bullet : bulletList ) {
    if ( strategy->execute(bullet, *obj) ) {
      return true;
    }
  }
  return false;
}

void BulletPool::draw() const { 
  for ( const auto& bullet : bulletList ) {
    bullet.draw();
  }
}

void BulletPool::update(Uint32 ticks) { 
  std::list<Bullet>::iterator ptr = bulletList.begin();
  while (ptr != bulletList.end()) {
    ptr->update(ticks);
    if (ptr->goneTooFar()) {  // Check to see if we should free a chunk
      //proj5, pool
      freeList.push_back(*ptr);
      ptr = bulletList.erase(ptr);
    }   
    else ++ptr;
  }
}
