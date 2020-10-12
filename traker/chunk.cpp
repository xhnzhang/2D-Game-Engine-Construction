/***********
*Xiaohong Zhang
*CPSC 6160, S19
*xiaohoz@g.clemson.edu
*Asg #6
*Menu and restart
************/
#include <iostream>
#include <cmath>
#include "chunk.h"

void Chunk::update(Uint32 ticks) { 
  float yincr = getVelocityY() * static_cast<float>(ticks) * 0.001;
  setY( getY()- yincr );
  float xincr = getVelocityX() * static_cast<float>(ticks) * 0.001;
  setX( getX()- xincr );
  distance += ( hypot(xincr, yincr) );
  if (distance > maxDistance) tooFar = true;
}

