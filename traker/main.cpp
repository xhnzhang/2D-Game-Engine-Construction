/***********
*Xiaohong Zhang
*CPSC 6160, S19
*xiaohoz@g.clemson.edu
*Asg #6
*Menu and restart
************/
#include "engine.h"


int main(int, char*[]) {
   bool keepPlaying = true;
   try {
      while (keepPlaying) {
         Engine* engine = new Engine;
         keepPlaying = engine->play();
         delete engine;
      }
   }
   catch (const string& msg) { std::cout << msg << std::endl; }
   catch (...) {
      std::cout << "Oops, someone threw an exception!" << std::endl;
   }
   return 0;
}
