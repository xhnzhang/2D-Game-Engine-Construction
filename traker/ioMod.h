/***********
*Xiaohong Zhang
*CPSC 6160, S19
*xiaohoz@g.clemson.edu
*Asg #6
*Menu and restart
************/
#ifndef IOMOD_H
#define IOMOD_H
#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

class IoMod {
public:
  static IoMod& getInstance();
  ~IoMod();
  SDL_Texture* readTexture(const std::string& filename);
  SDL_Surface* readSurface(const std::string& filename);
  void writeText(const std::string&, int, int) const;
  void writeText(const std::string&, int, int, SDL_Color&) const;
  //proj6, add font size
  void writeTextGame(const std::string&, int, int, SDL_Color&) const;

  SDL_Renderer* getRenderer() const { return renderer; }
  IoMod(const IoMod&) = delete;
  IoMod& operator=(const IoMod&) = delete;
private:
  int init;
  SDL_Renderer* renderer;
  TTF_Font* font;
  TTF_Font* font2;
  SDL_Color textColor;
  IoMod();
};
#endif
