/***********
*Xiaohong Zhang
*CPSC 6160, S19
*xiaohoz@g.clemson.edu
*Asg #6
*Menu and restart
************/
#ifndef IMAGE__H
#define IMAGE__H

#include <iostream>
#include <string>
#include <SDL.h>

class Image {
public:
  Image(SDL_Surface*);
  Image(const Image&);
  Image& operator=(const Image&);

  void regenerateTexture();
  void draw(int x, int y) const;
  void draw(int x, int y, float scale) const;
  void draw(int sx, int sy, int dx, int dy) const;
  // TASK 9: two-way-multi-sprites 
  void draw(int x, int y, float scale, double angle, 
            SDL_Point* center, SDL_RendererFlip flip) const; //(angle in degree)
  inline int getWidth()  const { return surface->w; }
  inline int getHeight() const { return surface->h; }
  SDL_Surface* getSurface() const { return surface; }
  Image* crop(SDL_Rect)const;
private:
  SDL_Renderer * renderer;
  SDL_Surface * surface;
  SDL_Texture * texture;
  SDL_Rect view;
  Image();
};

#endif
