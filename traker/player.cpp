/***********
*Xiaohong Zhang
*CPSC 6160, S19
*xiaohoz@g.clemson.edu
*Asg #6
*Menu and restart
************/
#include "player.h"
#include "gameData.h"
#include "imageFactory.h"
#include "bulletPool.h"
#include "collisionStrategy.h"
#include "explodingSprite.h"

void Player::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks; //data member
	if (timeSinceLastFrame > frameInterval) {
		currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

Vector2f Player::makeVelocity(int vx, int vy) const {
	float newvx = Gamedata::getInstance().getRandFloat(vx-50,vx+50);
	float newvy = Gamedata::getInstance().getRandFloat(vy-50,vy+50);
	newvx *= []() { if(rand()%2) return -1; else return 1; }();
	newvy *= []() { if(rand()%2) return -1; else return 1; }();

	return Vector2f(newvx, newvy);
}

Player::~Player() {
	if (explosion) delete explosion;
	if (collisionStrategy) delete collisionStrategy;
}


//do not use makeVelocity for initial player velocity
Player::Player(const std::string& name) :
	Drawable(name, 
			Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
					 Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
			Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
					 Gamedata::getInstance().getXmlInt(name+"/speedY")),
			Gamedata::getInstance().getXmlFloat(name+"/scale")
			),
	observers(),
	images(ImageFactory::getInstance().getImages(name)),
	currentFrame(0),
	numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
	frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
	timeSinceLastFrame(0),
	worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
	worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
	initialVelocity(getVelocity()),	
	/*add for proj 5*/
	collisionStrategy(new RectangularCollisionStrategy),
	explosion(nullptr),
	bulletName(Gamedata::getInstance().getXmlStr(name+"/bulletName")),
	bullets(bulletName),
	bulletSpeed(Gamedata::getInstance().getXmlInt(bulletName+"/speedX")),
	bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/interval")),
	timeSinceLastBullet(0),
	/*proj6*/
	godMode(true),
  	initLives(Gamedata::getInstance().getXmlInt(name+"/initLives")),
  	lives(initLives)
{ }

//proj5 check if explosion
void Player::draw() const { 
	if (explosion) {
		explosion->draw(); //draw exploded chunks
		return;
	}
	if (getVelocityX() > 0) {  
	  images[currentFrame]->draw(getX(), getY(), getScale());    
	}
	else { 
	  images[currentFrame]->draw(getX(), getY(), getScale(), 0, NULL, SDL_FLIP_HORIZONTAL);
	}
	bullets.draw(); //why always draw bulltes?
}


void Player::stop() { 
	//setVelocity( Vector2f(0, 0) );
	setVelocityX( 0.93*getVelocityX() );
	setVelocityY(0);
}

void Player::right() { 
	if ( getX() < worldWidth-getScaledWidth()) {
	  setVelocityX(initialVelocity[0]);
	}
} 
void Player::left()  { 
	if ( getX() > 0) {
	  setVelocityX(-initialVelocity[0]);
	}
} 
void Player::up()    { 
	if ( getY() > 0) {
	  setVelocityY( -initialVelocity[1] );
	}
} 
void Player::down()  { 
	if ( getY() < worldHeight-getScaledHeight()) {
	  setVelocityY( initialVelocity[1] );
	}
}


//proj5 shoot
void Player::shoot() {
	//see if it's time to shoot a bullet
	//get bullet position and velocity
	if (timeSinceLastBullet >  bulletInterval) {
		Vector2f vel = getVelocity();
		float x;
		float y = getY()+getScaledHeight()/4+11;
		if ( vel[0] > 0 ) {
			x = getX()+getScaledWidth()-10;
			vel[0] += bulletSpeed;
		}
		else {
			x = getX();
			vel[0] -= bulletSpeed;
		}
		bullets.shoot( Vector2f(x, y), vel ); 
		timeSinceLastBullet = 0;
	}
}


bool Player::shot(const Drawable* obj) {
	if ( bullets.collidedWith(obj) ) return true;
	else return false;
}

//what does this do?
void Player::explode() {
	//now just use scale 1.0 for explode, otherwise needs to modify a lot of code
	if (!explosion) {
		Sprite sprite(getName(), getPosition(), getVelocity(), images[currentFrame], 1.0);
		// sprite.setScale( getScale() );
		explosion = new ExplodingSprite(sprite);
	}
}

bool Player::explosionDone() const {
  if ( explosion && explosion->chunkCount() == 0 ) {
    return true;
  }
  else return false;
}

bool Player::collidedWith(const Drawable* obj) {
	if (explosion) return false;
	if (collisionStrategy->execute(*this, *obj)) {
		if(!godMode) {reduceLives();}
		return true;
	}
	return false;
}
/**/


void Player::update(Uint32 ticks) { 
	if (explosion) {
		explosion->update(ticks);
		if (explosion->chunkCount() == 0) {
			delete explosion;
			explosion = nullptr;
		}
		return;
	}

	advanceFrame(ticks);
	timeSinceLastBullet += ticks;
	bullets.update(ticks);

	Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
	setPosition(getPosition() + incr);

	if ( getY() < 0) {
	  setVelocityY( fabs( getVelocityY() ) );
	}
	if ( getY() > worldHeight-getScaledHeight()-100) {
	  setVelocityY( -fabs( getVelocityY() ) );
	}

	if ( getX() < 0) {
	  setVelocityX( fabs( getVelocityX() ) );
	}
	if ( getX() > worldWidth-getScaledWidth()) {
	  setVelocityX( -fabs( getVelocityX() ) );
	}  
	stop();	
	notify();
}

void Player::detach(SmartSprite* o) {
	std::list<SmartSprite*>::iterator ptr = observers.begin();
	while (ptr != observers.end()) {
		if (*ptr == o) {
			ptr = observers.erase(ptr);
			return;
		}
		++ptr;
	}
}

void Player::notify() {
	std::list<SmartSprite*>::iterator ptr = observers.begin();
	while (ptr != observers.end()) {
		int offset = rand()%5;
		offset = offset*(rand()%2?-1:1);
		const Vector2f off(offset, offset);
		(*ptr)->setPlayerPos(getPosition()+off);
		++ptr;
	}	
}




























