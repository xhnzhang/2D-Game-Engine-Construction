/***********
*Xiaohong Zhang
*CPSC 6160, S19
*xiaohoz@g.clemson.edu
*Asg #6
*Menu and restart
************/
#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "smartSprite.h"
#include "player.h"
#include "collisionStrategy.h"
#include "sound.h"
#include "lights.h"
#include "menuEngine.h"
#include "drawable.h"
#include "multisprite.h"
#include "health.h"

class Engine {
public:
  Engine ();
  ~Engine ();
  bool play(); //change from void to bool
  void switchSprite();

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
private:
  const RenderContext& rc;
  const IoMod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  std::vector<World> world;
  Viewport& viewport;

  Player* player;
  std::vector<SmartSprite*> sprites;
  std::vector<MultiSprite*> fireball;
  std::vector<CollisionStrategy*> strategies;
  int currentSprite;
  int currentStrategy;
  bool collision;

  SDLSound sound;
  bool makeVideo;

  //add lights for proj6
  Lights lights;
  MenuEngine menuEngine;
  Vector2f menuMsgLoc;
  Health& health;

  void draw() const;
  void update(Uint32);

  void printScales() const;
  void checkForCollisions();
};
