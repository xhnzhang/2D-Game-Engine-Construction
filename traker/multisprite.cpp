/***********
*Xiaohong Zhang
*CPSC 6160, S19
*xiaohoz@g.clemson.edu
*Asg #6
*Menu and restart
************/
#include "multisprite.h"
#include "gameData.h"
#include "imageFactory.h"
#include "explodingSprite.h"

//add for proj5
MultiSprite::~MultiSprite( ) { if (explosion) delete explosion; }

void MultiSprite::explode() {
  //now just use scale 1.0 for explode, otherwise needs to modify a lot of code
  if ( !explosion ) {
    Sprite 
    sprite(getName(), getPosition(), getVelocity(), images[currentFrame], 1.0);
    // sprite.setScale( getScale() );
    explosion = new ExplodingSprite(sprite);
  }
}

bool MultiSprite::explosionDone() const {
  if ( explosion && explosion->chunkCount() == 0 ) {
    return true;
  }
  else return false;
}


void MultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

Vector2f MultiSprite::makeVelocity(int vx, int vy) const {
  float newvx = Gamedata::getInstance().getRandFloat(vx-40,vx+30);;
  float newvy = Gamedata::getInstance().getRandFloat(vy-50,vy+60);;
  newvx *= [](){ if(rand()%2) return -1; else return 1; }();
  newvy *= [](){ if(rand()%2) return -1; else return 1; }();

  return Vector2f(newvx, newvy);
}

MultiSprite::MultiSprite( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           makeVelocity(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                        Gamedata::getInstance().getXmlInt(name+"/speedY")),
           Gamedata::getInstance().getXmlFloat(name+"/scale")
           ),
  images( ImageFactory::getInstance().getImages(name) ),
  explosion(nullptr),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

MultiSprite::MultiSprite(const MultiSprite& s) :
  Drawable(s),
  images(s.images),
  explosion(s.explosion),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight )
{ }

MultiSprite& MultiSprite::operator=(const MultiSprite& rhs) {
  Drawable::operator=(rhs);
  images = rhs.images;
  explosion = rhs.explosion;
  currentFrame = rhs.currentFrame;
  numberOfFrames = rhs.numberOfFrames;
  frameInterval = rhs.frameInterval;
  timeSinceLastFrame = rhs.timeSinceLastFrame;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  return *this;  
}



void MultiSprite::draw() const { 
  if ( explosion && explosion->chunkCount() > 0 ) explosion->draw();
  else if ( explosionDone() ) return;
  else images[currentFrame]->draw(getX(), getY(), getScale());
}

void MultiSprite::update(Uint32 ticks) { 
  if ( explosion && explosion->chunkCount() > 0 ) {
    explosion->update(ticks);
    return;
  }
  if ( explosion && explosion->chunkCount() == 0 ) {
    return;
  }
  
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() + getScaledHeight() > worldHeight-100) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
  }
  if ( getX() + getScaledWidth() > worldWidth) {
    setVelocityX( -fabs( getVelocityX() ) );
  }  

}
