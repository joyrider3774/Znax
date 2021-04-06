#ifndef CWORLDPARTS_H
#define CWORLDPARTS_H

#include "CBlock.h"
#include "Common.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <stdlib.h>

class CWorldParts {
private:
  bool GlobalSoundEnabled;
  SDL_Texture **IMGBackground = NULL, **IMGBlocks = NULL;
  Mix_Chunk **SNDDelete = NULL, **SNDError = NULL, **SNDBlockSelect = NULL;
  CBlock *Items[NrOfCols][NrOfRows];
  SPoint Selects[4];
  int NumSelected, SelectedColor;
  Uint32 Time;
  bool NeedToKillBlocks, NeedToAddBlocks;
  void KillBlocks();
  void AddBlocks();

public:
  CWorldParts(SDL_Texture **AIMGBackground, SDL_Texture **AIMGBlocks,
              Mix_Chunk **ASNDDelete, Mix_Chunk **ASNDError,
              Mix_Chunk **ASNDBlockSelect, bool AGlobalSoundEnabled);
  void NewGame();
  void Draw(SDL_Renderer *Renderer);
  void DeSelect(bool PlaySound);
  long Select(int PlayFieldX, int PlayFieldY);
  ~CWorldParts();
};

#endif
