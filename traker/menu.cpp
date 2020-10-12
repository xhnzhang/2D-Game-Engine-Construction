/***********
*Xiaohong Zhang
*CPSC 6160, S19
*xiaohoz@g.clemson.edu
*Asg #6
*Menu and restart
************/
#include "menu.h"
#include <sstream>

// const SDL_Color c = {255, 255, 0, 255};

Menu::Menu(SDL_Renderer* rend)
    : renderer(rend),
      gdata(Gamedata::getInstance()),
      hudFrame({gdata.getXmlInt("menu/loc/x"), 
                gdata.getXmlInt("menu/loc/y"), 
                gdata.getXmlInt("menu/width"),
                gdata.getXmlInt("menu/height")}),
      backColor({static_cast<Uint8>(gdata.getXmlInt("menu/backColor/r")),
                 static_cast<Uint8>(gdata.getXmlInt("menu/backColor/g")),
                 static_cast<Uint8>(gdata.getXmlInt("menu/backColor/b")),
                 static_cast<Uint8>(gdata.getXmlInt("menu/backColor/a"))}),
      menuColor(
          {static_cast<Uint8>(gdata.getXmlInt("menu/color/r")), 
           static_cast<Uint8>(gdata.getXmlInt("menu/color/g")),
           static_cast<Uint8>(gdata.getXmlInt("menu/color/b")), 
           static_cast<Uint8>(gdata.getXmlInt("menu/color/a"))}),
      clock(Clock::getInstance()),
      io(IoMod::getInstance()),
      options(),
      optionLoc({gdata.getXmlInt("menu/optionLoc/x"), 
                 gdata.getXmlInt("menu/optionLoc/y")}),
      clicks({Sprite("clickOff"), Sprite("clickOn")}),
      currentClick(0),
      currentOption(0),
      spaces(gdata.getXmlInt("menu/spaces")),
      startClickX(optionLoc[0] - spaces),
      startClickY(optionLoc[1] + spaces),
      clickX(startClickX),
      clickY(startClickY) {
  int noOfOptions = gdata.getXmlInt("menu/noOfOptions");
  std::stringstream strm;
  for (int i = 0; i < noOfOptions; ++i) {
    strm.clear();
    strm.str("");
    strm << i;
    std::string option("menu/option" + strm.str());
    options.push_back(gdata.getXmlStr(option));
  }
}

void Menu::incrIcon() {
  clickY += spaces;
  if (clickY > static_cast<int>(options.size()) * spaces + optionLoc[1]) {
    clickY = startClickY;
    currentOption = 0;
  } else
    ++currentOption;
}

void Menu::decrIcon() {
  clickY -= spaces;
  if (clickY < spaces + optionLoc[1]) {
    clickY = startClickY + 2 * spaces;
    currentOption = options.size() - 1;
  } else
    --currentOption;
}

void Menu::drawBackground() const {
  // First set the blend mode so that alpha blending will work;
  // the default blend mode is SDL_BLENDMODE_NONE!
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  // Set the hud background color:
  SDL_SetRenderDrawColor(renderer, backColor.r, backColor.g, backColor.b, backColor.a);

  // Draw the filled rectangle:
  SDL_RenderFillRect(renderer, &hudFrame);

  // Set the  color for the Menu outline:
  SDL_Rect menuFrame = {hudFrame.x + 50, hudFrame.y + 40, 
                        hudFrame.w - 100, hudFrame.h - 100};
  SDL_SetRenderDrawColor(renderer, menuColor.r, menuColor.g, menuColor.b, menuColor.a);
  SDL_RenderFillRect(renderer, &menuFrame);
}

int Menu::getInputEventLoop() const {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  drawBackground();
  std::string inNumber = " ";
  std::string msg = "Press return/esc when finished.";
  while (!done) {
    // The next loop polls for events, guarding against key bounce:
    while (SDL_PollEvent(&event)) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type == SDL_QUIT) {
        done = true;
        break;
      }
      if (event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q] || keystate[SDL_SCANCODE_RETURN]) {
          done = true;
          break;
        }
        if (keystate[SDL_SCANCODE_0]) {
          inNumber += '0';
        } else if (keystate[SDL_SCANCODE_1]) {
          inNumber += '1';
        } else if (keystate[SDL_SCANCODE_2]) {
          inNumber += '2';
        } else if (keystate[SDL_SCANCODE_3]) {
          inNumber += '3';
        } else if (keystate[SDL_SCANCODE_4]) {
          inNumber += '4';
        } else if (keystate[SDL_SCANCODE_5]) {
          inNumber += '5';
        } else if (keystate[SDL_SCANCODE_6]) {
          inNumber += '6';
        } else if (keystate[SDL_SCANCODE_7]) {
          inNumber += '7';
        } else if (keystate[SDL_SCANCODE_8]) {
          inNumber += '8';
        } else if (keystate[SDL_SCANCODE_9]) {
          inNumber += '9';
        }
      }
      // In this section of the event loop we allow key bounce:
      drawBackground();
      int offset_w = 380;
      io.writeText(msg, hudFrame.x + 330, hudFrame.y + 50);
      io.writeText("WSAD : Move", hudFrame.x + offset_w, hudFrame.y + 90);
      io.writeText("Space : Shoot", hudFrame.x + offset_w, hudFrame.y + 130);
      io.writeText("R: Restart", hudFrame.x + offset_w, hudFrame.y + 170);
      io.writeText("P: Pause", hudFrame.x + offset_w, hudFrame.y + 210);  
      io.writeText("ESC: Quit", hudFrame.x + offset_w, hudFrame.y + 250);  
      io.writeText("G : God Mode", hudFrame.x + offset_w, hudFrame.y + 290);
      io.writeText("F1 : Show HUD", hudFrame.x + offset_w, hudFrame.y + 330);
      io.writeText("F4: Make Video", hudFrame.x + offset_w, hudFrame.y + 370);
      io.writeText("** Shoot bats and avoid fireball", hudFrame.x + offset_w, hudFrame.y + 410);
      io.writeText("** Shoot all bats to win", hudFrame.x + offset_w, hudFrame.y + 450);
      // io.writeText("Press F1 : Trigger HUD ", hudFrame.x + offset_w, hudFrame.y + 330);
      SDL_RenderPresent(renderer);
    }
  }
  return atoi(inNumber.c_str());
}

int Menu::getNumStars() const {
  int numStars = getInputEventLoop();
  return numStars;
}

void Menu::draw() const {
  drawBackground();
  io.writeText("Options Menu", hudFrame.x + 400, hudFrame.y + 60);
  int space = spaces;
  for (const std::string& option : options) {
    io.writeText(option, optionLoc[0], optionLoc[1] + space);
    space += spaces;
  }
  // We have to draw the clickOn & clickOff relative to the screen,
  // and we don't want to offset by the location of the viewprot:
  clicks[currentClick].getImage()->draw(0, 0, clickX, clickY);
}
