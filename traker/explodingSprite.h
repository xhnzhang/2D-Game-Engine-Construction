/***********
*Xiaohong Zhang
*CPSC 6160, S19
*xiaohoz@g.clemson.edu
*Asg #6
*Menu and restart
************/
// This is the "pointers" example in the Brian example
#ifndef EXPLODINGSPRITE__H
#define EXPLODINGSPRITE__H
#include <vector>
#include <list>
#include "chunk.h"

class ExplodingSprite : public Sprite {
public:
  ExplodingSprite(const Sprite& s);
  ~ExplodingSprite();
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  unsigned int chunkCount() const { return chunks.size(); }
  unsigned int freeCount()  const { return freeList.size(); }
  ExplodingSprite(const ExplodingSprite&) = delete; // Explicit disallow 
  ExplodingSprite& operator=(const ExplodingSprite&) = delete; //Explicit disallow 
private:
  std::list<Chunk*> chunks; // An ExplodingSprite is a list of sprite chunks
  std::list<Chunk*> freeList; // When a chunk gets out of range it goes here

  void makeChunks(unsigned int);
  Vector2f getNewVelocity() const;

};
#endif
