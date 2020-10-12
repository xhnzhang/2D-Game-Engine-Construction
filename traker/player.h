/***********
*Xiaohong Zhang
*CPSC 6160, S19
*xiaohoz@g.clemson.edu
*Asg #6
*Menu and restart
************/
#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <vector>
#include <cmath>
#include <list>
#include "drawable.h"
#include "smartSprite.h"
#include "bulletPool.h"

class CollisionStrategy;
class ExplodingSprite;

class Player : public Drawable {
public:
  Player(const std::string&);  //convert constructor
  Player(const Player&) = default;	// copy constructor
  Player& operator=(const Player&) = default; //assignment operator
  virtual ~Player(); //destructor

  virtual void draw() const override;
  virtual void update(Uint32 ticks) override;

  virtual const Image* getImage() const override { 
    return images[currentFrame]; 
  }
  virtual int getScaledWidth()  const override { 
    return getScale()*images[currentFrame]->getWidth(); 
  } 
  virtual int getScaledHeight()  const override { 
    return getScale()*images[currentFrame]->getHeight(); 
  } 
  virtual const SDL_Surface* getSurface() const override { 
    return images[currentFrame]->getSurface();
  }

  void right();
  void left();
  void up();
  void down();
  void stop();

  void attach(SmartSprite* o) { observers.push_back(o); }
  void detach(SmartSprite* o);
  void notify();

  // add new functions for proj 5
  virtual void shoot() override;
  virtual bool shot(const Drawable*) override;
  virtual void explode() override;
  virtual bool explosionDone() const override;
  virtual bool collidedWith(const Drawable*) override;
  unsigned int getBulletCount() const { return bullets.bulletCount(); }
  unsigned int getFreeCount()   const { return bullets.freeCount(); }

  // proj6
  int getLives() { return lives; }
  void reduceLives() { if(lives>0) --lives; }
  bool getGodMode() { return godMode; }
  void toggleGodMode() { godMode = ! godMode; }

private:
	//vector of smart sprite
  std::list<SmartSprite*> observers;
  std::vector<Image *> images;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  Vector2f initialVelocity;

  // add new data member for proj 5
  CollisionStrategy* collisionStrategy;
  ExplodingSprite* explosion;

  std::string bulletName;
  BulletPool bullets;
  float bulletSpeed;
  int bulletInterval;
  int timeSinceLastBullet;
  ////////////////

  //proj6
  bool godMode;
  int initLives;
  int lives;

  void advanceFrame(Uint32 ticks);
  Vector2f makeVelocity(int, int) const;
};
#endif












