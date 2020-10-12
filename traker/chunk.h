/***********
*Xiaohong Zhang
*CPSC 6160, S19
*xiaohoz@g.clemson.edu
*Asg #6
*Menu and restart
************/
#ifndef CHUNK__H
#define CHUNK__H
#include <iostream>
#include "sprite.h"
#include "gameData.h"

class Chunk : public Sprite {
public:
  //my chunk class takes one more param: scale
  explicit Chunk( const Vector2f& pos, const Vector2f vel, 
                  const string& name, Image* fm, float scale) :
    Sprite(name, pos, vel, fm, scale), 
    distance(0), 
    maxDistance(Gamedata::getInstance().getXmlInt(name+"/distance")), 
    tooFar(false),
    image(fm)
  { }

  Chunk(const Chunk& )=default;
  Chunk(      Chunk&&)=default;
  Chunk& operator=(const Chunk& )=default;
  Chunk& operator=(      Chunk&&)=default;

  virtual ~Chunk(){
    delete image;
  }
  virtual void update(Uint32 ticks);
  bool goneTooFar() const { return tooFar; }
  void reset() {
    tooFar = false;
    distance = 0;
  }
private:
  float distance;
  float maxDistance;
  bool tooFar;
  Image* image;
};
#endif
