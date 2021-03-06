/***********
*Xiaohong Zhang
*CPSC 6160, S19
*xiaohoz@g.clemson.edu
*Asg #6
*Menu and restart
************/
#ifndef TWOWAYMULTISPRITE__H
#define TWOWAYMULTISPRITE__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class ExplodingSprite;

class TwoWayMultiSprite : public Drawable {
public:
  TwoWayMultiSprite(const std::string&);
  TwoWayMultiSprite(const TwoWayMultiSprite&);
  TwoWayMultiSprite& operator=(const TwoWayMultiSprite&);
  //proj5, modify destructor b/c added explostion
  virtual ~TwoWayMultiSprite();

  virtual void draw() const override;
  virtual void update(Uint32 ticks) override;
  //add collide for proj5
  virtual bool collidedWith(const Drawable*) override {
    throw std::string("I don't collide: ")+getName();
  }
  virtual void explode() override;
  virtual bool explosionDone() const override;
  //end add proj5
  
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

protected:
  std::vector<Image *> images;
  ExplodingSprite* explosion;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  void advanceFrame(Uint32 ticks);
  Vector2f makeVelocity(int, int) const;
};
#endif
