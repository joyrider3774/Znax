#ifndef CMAINMENU_H
#define CMAINMENU_H

#include "Common.h"
#include <SDL.h>
#include <SDL_mixer.h>

class CMainMenu {
private:
  int Selection; // Holds what we currently selected in the main menu, 1 = new
                 // game, 2= credits, 3 = quit
  SDL_Texture **IMGTitleScreen, **IMGPlay1, **IMGPlay2, **IMGHighScores1,
      **IMGHighScores2, **IMGCredits1, **IMGCredits2, **IMGQuit1, **IMGQuit2;
  bool GlobalSoundEnabled;
  Mix_Chunk **SNDMenu;

public:
  CMainMenu(SDL_Texture **AIMGTitleScreen, SDL_Texture **AIMGPlay1,
            SDL_Texture **AIMGPlay2, SDL_Texture **AIMGHighScores1,
            SDL_Texture **AIMGHighScores2, SDL_Texture **AIMGCredits1,
            SDL_Texture **AIMGCredits2, SDL_Texture **AIMGQuit1,
            SDL_Texture **AIMGQuit2, Mix_Chunk **ASNDMenu,
            bool AGlobalSoundEnabled);
  void Draw(SDL_Renderer *Renderer);
  int GetMouseSelection(int X, int Y);
  void NextItem();
  void PreviousItem();
  int GetSelection() { return Selection; }
  ~CMainMenu();
};

#endif
