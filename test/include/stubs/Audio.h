#ifndef AUDIO_H
#define AUDIO_H

#include "AudioIds.h"
#include "CommonTypes.h"

class Engine;

class Audio {
public:
  Audio(Engine* engine) {(void)engine;}
  ~Audio() {}

  int play(const Sfx_t, const int = 100, const int = 50) {return -1;}

  void playFromDirection(const Sfx_t, const Direction_t, const int) {}

  void tryPlayAmb(const int) {}
};

#endif
