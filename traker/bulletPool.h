/***********
*Xiaohong Zhang
*CPSC 6160, S19
*xiaohoz@g.clemson.edu
*Asg #6
*Menu and restart
************/
#ifndef BULLETPOOL__H
#define BULLETPOOL__H

#include <list>
#include "vector2f.h"
#include "drawable.h"
#include "bullet.h"

class CollisionStrategy;

class BulletPool {
public:
  BulletPool(const std::string&);
  BulletPool(const BulletPool&) = default;
  BulletPool& operator=(const BulletPool&) = default;
  ~BulletPool() = default;
  void draw() const;
  void update(Uint32 ticks);
  void shoot(const Vector2f& pos, const Vector2f& objVel);

  unsigned int bulletCount() const { return bulletList.size(); }
  unsigned int freeCount()   const { return freeList.size(); }

  bool shooting() const { return !bulletList.empty(); }
  bool collidedWith(const Drawable*) const;
private:
  std::string name;
  Vector2f myVelocity;
  std::list<Bullet> bulletList;
  std::list<Bullet> freeList;
  CollisionStrategy* strategy;
};
#endif
