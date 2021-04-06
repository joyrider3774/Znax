#ifndef CGAMETYPEMENU_H
#define CGAMETYPEMENU_H

#include "Common.h"
#include <SDL.h>
#include <SDL_mixer.h>

class CGameTypeMenu {
private:
  int Selection;
  SDL_Texture **IMGTitleScreen, **IMGSelectGame, **IMGFixedTimer1,
      **IMGFixedTimer2, **IMGRelativeTimer1, **IMGRelativeTimer2;
  bool GlobalSoundEnabled;
  Mix_Chunk **SNDMenu;

public:
  CGameTypeMenu(SDL_Texture **AIMGTitleScreen, SDL_Texture **AIMGSelectGame,
                SDL_Texture **AIMGFixedTimer1, SDL_Texture **AIMGFixedTimer2,
                SDL_Texture **AIMGRelativeTimer1,
                SDL_Texture **AIMGRelativeTimer2, Mix_Chunk **ASNDMenu,
                bool AGlobalSoundEnabled);
  void Draw(SDL_Renderer *Renderer);
  int GetMouseSelection(int X, int Y);
  void NextItem();
  void PreviousItem();
  int GetSelection() { return Selection; }
  ~CGameTypeMenu();
};

#endif
