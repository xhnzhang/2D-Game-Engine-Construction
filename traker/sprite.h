/***********
*Xiaohong Zhang
*CPSC 6160, S19
*xiaohoz@g.clemson.edu
*Asg #6
*Menu and restart
************/
#ifndef SPRITE__H
#define SPRITE__H
#include <string>
#include "drawable.h"


class ExplodingSprite;

class Sprite : public Drawable {
public:
  Sprite(const std::string&);
  Sprite(const std::string&, const Vector2f& pos, const Vector2f& vel, 
         const Image*, float scale);
  //proj5: modify destructor
  virtual ~Sprite();
  Sprite(const Sprite&);
  Sprite& operator=(const Sprite&);

  virtual void draw() const override;
  virtual void update(Uint32 ticks) override;
  //add collide for proj5
  virtual bool collidedWith(const Drawable*) override {
    throw std::string("I don't collide: ")+getName();
  }
  virtual void explode () override;
  virtual bool explosionDone() const override;
  //end add proj5

  virtual const Image* getImage() const override { return image; }
  virtual const SDL_Surface* getSurface() const override { 
    return image->getSurface();
  }
  virtual int getScaledWidth()  const override { return getScale()*image->getWidth();  } 
  virtual int getScaledHeight() const override { return getScale()*image->getHeight(); } 

private:
  const Image * image;
  //add collide for proj5
  ExplodingSprite* explosion;
  //end add proj5
protected:
  int worldWidth;
  int worldHeight;

  int getDistance(const Sprite*) const;
  Vector2f makeVelocity(int, int) const;
};
#endif
