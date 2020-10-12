/***********
*Xiaohong Zhang
*CPSC 6160, S19
*xiaohoz@g.clemson.edu
*Asg #6
*Menu and restart
************/
#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "gameData.h"
#include "engine.h"
#include "frameGenerator.h"
#include "hud.h"
#include "sound.h"

Engine::~Engine() { 
  delete player;
  for (auto& s : sprites) {
    delete s;
  }
  for(auto& st : strategies) {
    delete st;
  }
  for(auto& i : fireball) {
    delete i;
  }
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc.getRenderer() ),
  world(),
  viewport( Viewport::getInstance() ),
  player(new Player("Alien")),
  sprites(),
  fireball(),
  strategies(),
  currentSprite(0),
  currentStrategy(0),
  collision(false),
  sound(), //add for proj5
  makeVideo(false),
  lights(), //add for proj6
  menuEngine(),
  menuMsgLoc(
    Vector2f( Gamedata::getInstance().getXmlInt("menu/msg/x"), 
              Gamedata::getInstance().getXmlInt("menu/msg/y"))),
  health( Health::getInstance())
{
  world.reserve(3);
  world.emplace_back("bg", Gamedata::getInstance().getXmlInt("bg/factor"));
  world.emplace_back("land",  Gamedata::getInstance().getXmlInt("land/factor"));

  int bat_num = Gamedata::getInstance().getXmlInt("Bat/number");
  // sprites.reserve(bat_num);

  Vector2f pos = player->getPosition();
  int w = player->getScaledWidth();
  int h = player->getScaledHeight();
  for (int i = 0; i < bat_num; ++i) {
    sprites.emplace_back(new SmartSprite("Bat", pos, w, h));
  }
  for (auto& s : sprites) {
    player->attach(s);
  }

  int fire_num = Gamedata::getInstance().getXmlInt("Fireball/number");
  for (int i = 0; i < fire_num; ++i) {
    fireball.emplace_back(new MultiSprite("Fireball"));
  } 

  strategies.push_back(new RectangularCollisionStrategy);
  strategies.push_back(new PerPixelCollisionStrategy);
  strategies.push_back(new MidPointCollisionStrategy);  

  Viewport::getInstance().setObjectToTrack(player);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  for (const auto& w : world) { 
    w.draw();
  }
  for (const auto& s : sprites) {
    s->draw();
  }
  player->draw();
  //proj6
  for (const auto& i : fireball) {
    i->draw();
  }
  lights.draw();


  health.setLives(player->getLives());
  // std::cout << player->getLives() << std::endl;
  health.drawLives();


  // io.writeText("Press F1 to see HUD", 30, 30);
  // std::stringstream strmfps;
  // strmfps << "fps: " << clock.getFps();
  // io.writeText(strmfps.str(), 30, 60);
  //upper left
  std::stringstream strmsprites; //smart sprite number
  strmsprites << sprites.size() << " Bats Remaining";
  io.writeText(strmsprites.str(), 30, 60);
  io.writeText("M: Menu", 30, 90);

  //upper right
  SDL_Color c = {0, 255, 0, 255};
  if (player->getGodMode()) {
    io.writeText("God Mode:", 720, 30);
    io.writeText("YES", 750, 60, c);
  }
  else {
    io.writeText("God Mode:", 720, 30);
    io.writeText("NO", 750, 60, c);
  }

  //lower left
  std::string name = Gamedata::getInstance().getXmlStr("name");
  io.writeText(name, 30, Gamedata::getInstance().getXmlInt("view/height")-50);

  // lower right
  std::stringstream strmbullet;
  strmbullet << "Bullet List: " << player->getBulletCount();
  io.writeText(strmbullet.str(),Gamedata::getInstance().getXmlInt("view/width")-120,670);
  std::stringstream strmpool;
  strmpool << "Pool: " << player->getFreeCount();
  io.writeText(strmpool.str(),Gamedata::getInstance().getXmlInt("view/width")-120,690);

  // viewport.draw(); // draw Tracking obj
  if(player->getLives() <= 0) {
    // sound[5];
    health.showLose();
    clock.pause();
  }
  if(sprites.size() <= 0) {
    health.showWin();
    clock.pause();
  }

  Hud::getInstance().draw();
  SDL_RenderPresent(renderer);
}


void Engine::checkForCollisions() {
  //check whether close enough for collision to happen

  if (!player->getGodMode()) {
    for (auto& i : fireball) {
      if (player->collidedWith(i)) {
        sound[5];
        player->explode();
      }
    }
  }


  for (auto& s: sprites) {
    if (player->shot(s)) {
      sound[3];
      s->explode();
    }
  }


  /////// If below is emplemented, sprite won't explode
//   auto it = sprites.begin();
//   while (it != sprites.end()) {                    
//     if (player->collidedWith(*it) || player->shot(*it)) {
//       collision = true;  //only use for displaying if collision happens
//       SmartSprite* doa = *it;
//       player->detach(doa);
//       delete doa;
//       it = sprites.erase(it);
//     }
//     else ++it;
//   }
// }

}

void Engine::update(Uint32 ticks) {
  collision = false;
  checkForCollisions();
  player->update(ticks);
  for (auto& w : world) { 
    w.update(); 
  } //world doesn't need redraw every ticks?


  // for (auto& s : sprites) {
  //   s->update(ticks); //sprites needs redraw for every tick
  // }

  //modified for proj6
  auto it = sprites.begin();
  while (it != sprites.end()) {
    (*it)->update(ticks);
    if ((*it)->explosionDone()) {
      SmartSprite* doa = (*it);
      player->detach(doa);
      delete doa;
      it = sprites.erase(it);
    } else
      ++it;
  }

  //proj6
  for (auto& i : fireball) {
    i->update(ticks);
  }
  lights.update();

  viewport.update(); // always update viewport last
}

void Engine::switchSprite(){
  ++currentSprite; 
  currentSprite = currentSprite % sprites.size();
  if (currentSprite) {  
  Viewport::getInstance().setObjectToTrack(sprites[currentSprite]);
  }
  else {
    Viewport::getInstance().setObjectToTrack(sprites[0]);
  }
}

bool Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if (event.type == SDL_KEYDOWN) {
        // if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
        if (keystate[SDL_SCANCODE_ESCAPE]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_R] ) {
          clock.unpause();
          return true;
        }
        // if ( keystate[SDL_SCANCODE_E] ) {
        //   //default currentSprite = 0
        //   if (!currentSprite) player->explode();
        //   else sprites[currentSprite]->explode();
        // }
        // if ( keystate[SDL_SCANCODE_T] ) {
        //   switchSprite();
        // }
        // if ( keystate[SDL_SCANCODE_M] ) {
        //   currentStrategy = (1 + currentStrategy) % strategies.size();
        // }
        if (keystate[SDL_SCANCODE_G]) {
          player->toggleGodMode();
        }        
        if ( keystate[SDL_SCANCODE_M] ) {
          clock.pause();
          menuEngine.play();
        



          // if (menuEngine.starsOptionChosen()) {
          //   int num = menuEngine.getNumStars();

          //   auto it = sprites.begin();
          //   while (it != sprites.end()) {
          //     player->detach(*it);
          //     it = sprites.erase(it);
          //     ++it;
          //   }
          //   // for (auto& i : sprites) {delete i;}
          //   Vector2f pos = player->getPosition();
          //   int w = player->getScaledWidth();
          //   int h = player->getScaledHeight();
          //   for (int i = 0; i < num; ++i) {
          //     sprites.emplace_back(new SmartSprite("Bat", pos, w, h));
          //   }
          //   for (auto& s : sprites) {
          //     player->attach(s);
          //   }
          // }


          clock.unpause();
        }
        if ( keystate[SDL_SCANCODE_F1] ) {
            Hud::getInstance().changeVisible();
        }

        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    // In this section of the event loop we allow key bounce:

    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      if (keystate[SDL_SCANCODE_SPACE]) {
        player->shoot();
      }
      if (keystate[SDL_SCANCODE_A]) {
        player->left();
      }
      if (keystate[SDL_SCANCODE_D]) {
        player->right();
      }        
      if (keystate[SDL_SCANCODE_W]) {
        player->up();
      }
      if (keystate[SDL_SCANCODE_S]) {
        player->down();
      }
      draw();
      update(ticks);
      if ( makeVideo ) { frameGen.makeFrame(); }
    }
  }
  //add for proj5
  return false;
}
