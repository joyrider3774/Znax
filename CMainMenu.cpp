#include "CMainMenu.h"

// constructor of main menu will Load the graphics and set the current selection
// to 1 (io newgame)
CMainMenu::CMainMenu(SDL_Texture **AIMGTitleScreen, SDL_Texture **AIMGPlay1,
                     SDL_Texture **AIMGPlay2, SDL_Texture **AIMGHighScores1,
                     SDL_Texture **AIMGHighScores2, SDL_Texture **AIMGCredits1,
                     SDL_Texture **AIMGCredits2, SDL_Texture **AIMGQuit1,
                     SDL_Texture **AIMGQuit2, Mix_Chunk **ASNDMenu,
                     bool AGlobalSoundEnabled) {
  GlobalSoundEnabled = AGlobalSoundEnabled;
  Selection = 1;
  IMGTitleScreen = AIMGTitleScreen;
  IMGPlay1 = AIMGPlay1;
  IMGPlay2 = AIMGPlay2;
  IMGHighScores1 = AIMGHighScores1;
  IMGHighScores2 = AIMGHighScores2;
  IMGCredits1 = AIMGCredits1;
  IMGCredits2 = AIMGCredits2;
  IMGQuit1 = AIMGQuit1;
  IMGQuit2 = AIMGQuit2;
  SNDMenu = ASNDMenu;
}

CMainMenu::~CMainMenu() {}

// Increase the selection if it goes to far set i to the first selection
void CMainMenu::NextItem() {
  Selection++;
  if (Selection == 5)
    Selection = 1;
  if (GlobalSoundEnabled)
    Mix_PlayChannel(-1, *SNDMenu, 0);
}

// decrease the selection if it goes to low set it to the last selection
void CMainMenu::PreviousItem() {
  Selection--;
  if (Selection == 0)
    Selection = 4;
  if (GlobalSoundEnabled)
    Mix_PlayChannel(-1, *SNDMenu, 0);
}

int CMainMenu::GetMouseSelection(int X, int Y) {
  if ((X >= (98)) && (X <= (98 + 123))) {
    if ((Y >= (73)) && (Y < (73 + 32)))
      return 1;
    if ((Y >= (105)) && (Y < (105 + 32)))
      return 2;
    if ((Y >= (137)) && (Y < (137 + 32)))
      return 3;
    if ((Y >= (169)) && (Y < (169 + 32)))
      return 4;
  }
  return -1;
}

// Draw the main menu
void CMainMenu::Draw(SDL_Renderer *Renderer) {
  SDL_Rect Dest;
  SDL_RenderCopy(Renderer, *IMGTitleScreen, NULL, NULL);

  if (Selection == 1) {
    Dest.x = 98;
    Dest.y = 73;
    SDL_QueryTexture(*IMGPlay2, NULL, NULL, &Dest.w, &Dest.h);
    SDL_RenderCopy(Renderer, *IMGPlay2, NULL, &Dest);
  } else {
    Dest.x = 98;
    Dest.y = 73;
    SDL_QueryTexture(*IMGPlay1, NULL, NULL, &Dest.w, &Dest.h);
    SDL_RenderCopy(Renderer, *IMGPlay1, NULL, &Dest);
  }

  if (Selection == 2) {
    Dest.x = 98;
    Dest.y = 105;
    SDL_QueryTexture(*IMGHighScores2, NULL, NULL, &Dest.w, &Dest.h);
    SDL_RenderCopy(Renderer, *IMGHighScores2, NULL, &Dest);
  } else {
    Dest.x = 98;
    Dest.y = 105;
    SDL_QueryTexture(*IMGHighScores1, NULL, NULL, &Dest.w, &Dest.h);
    SDL_RenderCopy(Renderer, *IMGHighScores1, NULL, &Dest);
  }

  if (Selection == 3) {
    Dest.x = 98;
    Dest.y = 137;
    SDL_QueryTexture(*IMGCredits2, NULL, NULL, &Dest.w, &Dest.h);
    SDL_RenderCopy(Renderer, *IMGCredits2, NULL, &Dest);

  } else {
    Dest.x = 98;
    Dest.y = 137;
    SDL_QueryTexture(*IMGCredits1, NULL, NULL, &Dest.w, &Dest.h);
    SDL_RenderCopy(Renderer, *IMGCredits1, NULL, &Dest);
  }

  if (Selection == 4) {
    Dest.x = 98;
    Dest.y = 169;
    SDL_QueryTexture(*IMGQuit2, NULL, NULL, &Dest.w, &Dest.h);
    SDL_RenderCopy(Renderer, *IMGQuit2, NULL, &Dest);
  } else {
    Dest.x = 98;
    Dest.y = 169;
    SDL_QueryTexture(*IMGQuit1, NULL, NULL, &Dest.w, &Dest.h);
    SDL_RenderCopy(Renderer, *IMGQuit1, NULL, &Dest);
  }
}
