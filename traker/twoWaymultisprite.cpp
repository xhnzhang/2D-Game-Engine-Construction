/***********
*Xiaohong Zhang
*CPSC 6160, S19
*xiaohoz@g.clemson.edu
*Asg #6
*Menu and restart
************/
#include "twoWaymultisprite.h"
#include "gameData.h"
#include "imageFactory.h"
#include "explodingSprite.h"


//add for proj5
TwoWayMultiSprite::~TwoWayMultiSprite( ) { if (explosion) delete explosion; }

void TwoWayMultiSprite::explode() {
  if ( !explosion ) {
    //now just use scale 1.0 for explode, otherwise needs to modify a lot of code
    Sprite 
    sprite(getName(), getPosition(), getVelocity(), images[currentFrame], 1.0);
    // sprite.setScale( getScale() );
    explosion = new ExplodingSprite(sprite);
  }
}

bool TwoWayMultiSprite::explosionDone() const {
  if ( explosion && explosion->chunkCount() == 0 ) {
    return true;
  }
  else return false;
}


void TwoWayMultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

Vector2f TwoWayMultiSprite::makeVelocity(int vx, int vy) const {
  float newvx = Gamedata::getInstance().getRandFloat(vx-50,vx+50);;
  float newvy = Gamedata::getInstance().getRandFloat(vy-50,vy+50);;
  newvx *= [](){ if(rand()%2) return -1; else return 1; }();
  newvy *= [](){ if(rand()%2) return -1; else return 1; }();

  return Vector2f(newvx, newvy);
}

TwoWayMultiSprite::TwoWayMultiSprite( const std::string& name) :
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
{ } //Use ImageFactory to process image into vector of sprites

TwoWayMultiSprite::TwoWayMultiSprite(const TwoWayMultiSprite& s) :
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

TwoWayMultiSprite& TwoWayMultiSprite::operator=(const TwoWayMultiSprite& rhs) {
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

//call a fliped draw() go go left 
void TwoWayMultiSprite::draw() const { 

  //proj5
  if ( explosion && explosion->chunkCount() > 0 ) explosion->draw();
  else if ( explosionDone() ) return;
  else { 
    if (getVelocityX() > 0) {  
      images[currentFrame]->draw(getX(), getY(), getScale());    
    }
    else { 
      images[currentFrame]->draw(getX(), getY(), getScale(), 0, NULL, SDL_FLIP_HORIZONTAL);
    }
  }
}

void TwoWayMultiSprite::update(Uint32 ticks) { 
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

  // if ( getY() < worldHeight-150) { //do not swim to the sky
  if (getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() + getScaledHeight() > worldHeight-100) { //do not swim to sea land
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
  }
  if ( getX() + getScaledWidth() > worldWidth) {
    setVelocityX( -fabs( getVelocityX() ) );
  }  

}
