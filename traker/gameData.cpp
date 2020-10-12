/***********
*Xiaohong Zhang
*CPSC 6160, S19
*xiaohoz@g.clemson.edu
*Asg #6
*Menu and restart
************/
#include <sstream>
#include <iostream>
#include <limits>
#include <cmath>
#include "gameData.h"

Gamedata& Gamedata::getInstance() {
  static Gamedata gamedata;
  return gamedata;
}

Gamedata::~Gamedata( ) { }

Gamedata::Gamedata(const string& fn ) :
  parser(fn),
  gameData(parser.getXmlData())
{ }

float Gamedata::getRandInRange(int min, int max) const {
  return min + (rand()/(std::numeric_limits<int>::max()+1.0f))*(max-min);
}

float Gamedata::getRandFloat(float min, float max) const {
  return min + (rand() / (RAND_MAX + 1.0f)) * (max - min);
}

bool  Gamedata::checkTag(const std::string& tag)const{
  return gameData.count(tag)!=0;
}

bool Gamedata::getXmlBool(const string& tag) const {
  std::map<string, string>::const_iterator ptr = gameData.find(tag);
  if ( ptr == gameData.end() )
    throw string("Game: Didn't find boolean tag ")+tag+string(" in xml");
  else {
    if ( ptr->second == "true" ) return true;
    else return false;
  }
}

int Gamedata::getXmlInt(const string& tag) const {
  std::map<string, string>::const_iterator ptr = gameData.find(tag);
  if ( ptr == gameData.end() )
    throw string("Game: Didn't find integer tag ")+tag+string(" in xml");
  else {
    std::stringstream strm;
    strm << ptr->second;
    std::string stringNumber = strm.str();
    int data = 0;
    strm >> data;
    if ( strm.fail()  ) {
      throw "Invalid integer for "+tag+string(" in xml");
    }
    return data;
  }
}

float Gamedata::getXmlFloat(const string& tag) const {
  std::map<string, string>::const_iterator ptr = gameData.find(tag);
  if ( ptr == gameData.end() )
    throw string("Game: Didn't find float tag ")+tag+string(" in xml");
  else {
    std::stringstream strm;
    strm << ptr->second;
    float data = 0.0f;
    strm >> data;
    if ( strm.fail()  ) {
      throw "Invalid float for "+tag+string(" in xml");
    }
    return data;
  }
}

const string& Gamedata::getXmlStr(const string& tag) const {
  std::map<string, string>::const_iterator ptr = gameData.find(tag);
  if ( ptr == gameData.end() )
    throw string("Game: Didn't find string tag ")+tag+string(" in xml");
  else return ptr->second;
}
// TASK 1-2: Covert while loop to ranged for loop
void Gamedata::displayData() const {
  for (const auto& i : gameData) {
    std::cout << i.first << ", " << i.second << std::endl;
  }
}


