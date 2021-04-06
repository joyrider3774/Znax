#include "CGameTypeMenu.h"

// constructor of main menu will Load the graphics and set the current selection
// to 1 (io newgame)
CGameTypeMenu::CGameTypeMenu(SDL_Texture **AIMGTitleScreen,
                             SDL_Texture **AIMGSelectGame,
                             SDL_Texture **AIMGFixedTimer1,
                             SDL_Texture **AIMGFixedTimer2,
                             SDL_Texture **AIMGRelativeTimer1,
                             SDL_Texture **AIMGRelativeTimer2,
                             Mix_Chunk **ASNDMenu, bool AGlobalSoundEnabled) {
  GlobalSoundEnabled = AGlobalSoundEnabled;
  Selection = Fixed;
  IMGTitleScreen = AIMGTitleScreen;
  IMGSelectGame = AIMGSelectGame;
  IMGFixedTimer1 = AIMGFixedTimer1;
  IMGFixedTimer2 = AIMGFixedTimer2;
  IMGRelativeTimer1 = AIMGRelativeTimer1;
  IMGRelativeTimer2 = AIMGRelativeTimer2;
  SNDMenu = ASNDMenu;
}

// Destructor will free the surface images
CGameTypeMenu::~CGameTypeMenu() {}

// Increase the selection if it goes to far set i to the first selection
void CGameTypeMenu::NextItem() {
  Selection++;
  if (Selection == 2)
    Selection = 0;
  if (GlobalSoundEnabled)
    Mix_PlayChannel(-1, *SNDMenu, 0);
}

// decrease the selection if it goes to low set it to the last selection
void CGameTypeMenu::PreviousItem() {
  Selection--;
  if (Selection == -1)
    Selection = 1;
  if (GlobalSoundEnabled)
    Mix_PlayChannel(-1, *SNDMenu, 0);
}

int CGameTypeMenu::GetMouseSelection(int X, int Y) {
  if ((X >= (68)) && (X <= (68 + 208))) {
    if ((Y >= (114)) && (Y < (114 + 37)))
      return 0;
    if ((Y >= (151)) && (Y < (151 + 37)))
      return 1;
  }
  return -1;
}

// Draw the main menu
void CGameTypeMenu::Draw(SDL_Renderer *Renderer) {
  SDL_Rect Dest;

  // draw the title screen background
  SDL_RenderCopy(Renderer, *IMGTitleScreen, NULL, NULL);

  Dest.x = 68;
  Dest.y = 77;
  SDL_QueryTexture(*IMGSelectGame, NULL, NULL, &Dest.w, &Dest.h);
  SDL_RenderCopy(Renderer, *IMGSelectGame, NULL, &Dest);

  if (Selection == Fixed) {
    Dest.x = 68;
    Dest.y = 114;
    SDL_QueryTexture(*IMGFixedTimer2, NULL, NULL, &Dest.w, &Dest.h);
    SDL_RenderCopy(Renderer, *IMGFixedTimer2, NULL, &Dest);
  } else {
    Dest.x = 68;
    Dest.y = 114;
    SDL_QueryTexture(*IMGFixedTimer1, NULL, NULL, &Dest.w, &Dest.h);
    SDL_RenderCopy(Renderer, *IMGFixedTimer1, NULL, &Dest);
  }

  if (Selection == Relative) {
    Dest.x = 68;
    Dest.y = 151;
    SDL_QueryTexture(*IMGRelativeTimer2, NULL, NULL, &Dest.w, &Dest.h);
    SDL_RenderCopy(Renderer, *IMGRelativeTimer2, NULL, &Dest);
  } else {
    Dest.x = 68;
    Dest.y = 151;
    SDL_QueryTexture(*IMGRelativeTimer1, NULL, NULL, &Dest.w, &Dest.h);
    SDL_RenderCopy(Renderer, *IMGRelativeTimer1, NULL, &Dest);
  }
}
