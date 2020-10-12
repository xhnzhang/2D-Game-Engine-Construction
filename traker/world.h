/***********
*Xiaohong Zhang
*CPSC 6160, S19
*xiaohoz@g.clemson.edu
*Asg #6
*Menu and restart
************/
#ifndef WORLD__H
#define WORLD__H
#include <string>
#include "image.h"
#include "viewport.h"

class World {
public:
  World(const std::string& name, int fact);
  // ~World() { } // The image will be deleted by the ImageFactory
  void update();
  void draw() const;
  World(const World&); //do not set to delete, std::vector<World> in engin.cpp needs reserve, which needs copy cstr
  World& operator=(const World&) = delete; 
private:
  // Image* const image;
  Image* const image;
  int factor;
  unsigned imageWidth;
  float viewX;
  float viewY;
  const Viewport& view;
};

#endif
