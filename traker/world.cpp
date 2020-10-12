/***********
*Xiaohong Zhang
*CPSC 6160, S19
*xiaohoz@g.clemson.edu
*Asg #6
*Menu and restart
************/
#include <iostream>
#include "world.h"
#include "imageFactory.h"

World::World(const std::string& name, int fact) : 
  image( ImageFactory::getInstance().getImage(name) ),
  factor(fact),
    imageWidth( image->getWidth() ),
    viewX(0.0), viewY(0.0), 
    view(Viewport::getInstance()) 
{ }

World::World(const World& w) : 
	image(w.image),
	factor(w.factor),
	imageWidth(w.imageWidth),
	viewX(w.viewX),
	viewY(w.viewY),
	view(w.view)
{ }

// World& World::operator=(const World& rhs) {
// 	if (this == &rhs) { return *this; }
// 	//cannot do image=rhs.image, b/c Image* const, cannot change pointer.
// 	image = rhs.image; 
// 	factor = rhs.factor;
// 	imageWidth = rhs.imageWidth;
// 	viewX = rhs.viewX;
// 	viewY = rhs.viewY;
// 	view = Viewport::getInstance(); //cannot do view=rhs.view
// 	return *this;
// }

void World::update() {
  viewX = static_cast<int>(view.getX() / factor) % imageWidth;
  viewY = view.getY();
}

void World::draw() const { 
  image->draw(0,0,-viewX,-viewY);
  image->draw(0,0,imageWidth-viewX,-viewY);
}




