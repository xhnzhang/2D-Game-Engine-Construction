/***********
*Xiaohong Zhang
*CPSC 6160, S19
*xiaohoz@g.clemson.edu
*Asg #6
*Menu and restart
************/
#include <string>
#include <iostream>
#include <ctime>
#include "sound.h"
using std::string;

SDLSound::~SDLSound() {
  std::cout << "Cleaning up sounds ..." << std::endl;
  std::clock_t start = std::clock();
  Mix_HaltMusic();
  Mix_FreeMusic(music);
  for (unsigned int i = 0; i < sounds.size(); ++i) {
    Mix_FreeChunk(sounds[i]); //deallocat chunks since SDL2 sound has leaks
  }
  Mix_CloseAudio();
  std::clock_t duration = std::clock() - start;
  std::cout << "Clean up took " << duration << " ticks\n";
}
/* play around with /10 to increase or reduce the sound */
SDLSound::SDLSound() : 
  volume(SDL_MIX_MAXVOLUME/10), 
  currentSound(-1), 
  music(NULL),
  audioRate(22050), 
  audioChannels(2), 
  audioBuffers(4096),
  sounds(),
  channels() 
{

  if(Mix_OpenAudio(audioRate, MIX_DEFAULT_FORMAT, audioChannels, 
                   audioBuffers)){
    throw string("Unable to open audio!");
  }
  music = Mix_LoadMUS("sound/bgm.ogg");
  // Need to install midi to play the following:
  // music = Mix_LoadMUS("sound/ballad2.mid");
  if (!music) throw string("Couldn't load bgm.ogg")+Mix_GetError();

  startMusic();

  sounds.push_back( Mix_LoadWAV("sound/Boom.wav") );
  sounds.push_back( Mix_LoadWAV("sound/Gun.wav") );
  sounds.push_back( Mix_LoadWAV("sound/Thump.wav") );
  sounds.push_back( Mix_LoadWAV("sound/phaser.wav") );
  sounds.push_back( Mix_LoadWAV("sound/Putt1.wav") );
  sounds.push_back( Mix_LoadWAV("sound/explosion.wav") );
  sounds.push_back( Mix_LoadWAV("sound/cannon.wav") );
  sounds.push_back( Mix_LoadWAV("sound/Larc.wav") );
  for (unsigned int i = 0; i < sounds.size(); ++i) channels.push_back(i);
  std::cout << "Music and Sound is loaded" << std::endl;
}

void SDLSound::toggleMusic() {
  if( Mix_PausedMusic() ) { 
    Mix_ResumeMusic(); 
  } 
  else { 
    Mix_PauseMusic(); 
  } 
}
/*overload operator to use different sound effects*/
void SDLSound::operator[](int index) {
  if (currentSound >= 0) Mix_HaltChannel(currentSound);
  currentSound = index;
  Mix_VolumeChunk(sounds[index], volume); //adjust chunk volume
  channels[index] = 
     Mix_PlayChannel(-1, sounds[index], 0);
}

void SDLSound::startMusic() {
  Mix_VolumeMusic(volume);   // adjust music volume
  Mix_PlayMusic(music, -1);  // -1 means play infinitely
}

void SDLSound::stopMusic() {
  Mix_HaltMusic();
  Mix_FreeMusic(music);
}

