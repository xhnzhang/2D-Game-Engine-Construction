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
#include "sprite.h"
#include "gameData.h"
#include "imageFactory.h"
#include "explodingSprite.h"

//add for proj5

Sprite::~Sprite() { if ( explosion ) delete explosion; } 

void Sprite::explode() {
  if ( !explosion ) explosion = new ExplodingSprite(*this);
}

bool Sprite::explosionDone() const {
  if ( explosion && explosion->chunkCount() == 0 ) {
    return true;
  }
  else return false;
}



Vector2f Sprite::makeVelocity(int vx, int vy) const {
  float newvx = Gamedata::getInstance().getRandFloat(vx-50,vx+50);;
  float newvy = Gamedata::getInstance().getRandFloat(vy-50,vy+50);;
  newvx *= [](){ if(rand()%2) return -1; else return 1; }();
  newvy *= [](){ if(rand()%2) return -1; else return 1; }();

  return Vector2f(newvx, newvy);
}

Sprite::Sprite(const string& n, const Vector2f& pos, const Vector2f& vel,
               const Image* img, float scale):
  Drawable(n, pos, vel, scale), 
  image( img ),
  explosion(nullptr),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

Sprite::Sprite(const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           makeVelocity(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                        Gamedata::getInstance().getXmlInt(name+"/speedY")),
           Gamedata::getInstance().getXmlFloat(name+"/scale")
           ),
  image( ImageFactory::getInstance().getImage(name) ),
  explosion(nullptr),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

Sprite::Sprite(const Sprite& s) : 
  Drawable(s),//why? use derived to inilizae base?
  image(s.image),
  explosion(s.explosion),
  worldWidth(s.worldWidth),
  worldHeight(s.worldHeight)
{ }

Sprite& Sprite::operator=(const Sprite& rhs) {
  if (this == & rhs) { return *this; }
  Drawable::operator=(rhs);
  image = rhs.image;
  explosion = rhs.explosion;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  return *this;  
}

inline namespace{
  constexpr float SCALE_EPSILON = 2e-7;
}

void Sprite::draw() const { 
  if(getScale() < SCALE_EPSILON) return;
  if ( explosion && explosion->chunkCount() > 0 ) explosion->draw();
  else if ( explosionDone() ) return;
  else image->draw(getX(), getY(), getScale()); 
}

void Sprite::update(Uint32 ticks) { 
  if ( explosion && explosion->chunkCount() > 0 ) {
    explosion->update(ticks);
    return;
  }
  else if ( explosionDone() ) {
    return;
  }

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( std::abs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight()-100) {
    setVelocityY( -std::abs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( std::abs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -std::abs( getVelocityX() ) );
  }  
}
