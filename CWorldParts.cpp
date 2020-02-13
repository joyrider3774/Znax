#include "CBlock.h"
#include "CWorldParts.h"
#include "Common.h"

CWorldParts::CWorldParts(SDL_Texture **AIMGBackground, SDL_Texture **AIMGBlocks,
                         Mix_Chunk **ASNDDelete, Mix_Chunk **ASNDError,
                         Mix_Chunk **ASNDBlockSelect,
                         bool AGlobalSoundEnabled) {
  int X, Y, Teller;
  GlobalSoundEnabled = AGlobalSoundEnabled;
  IMGBackground = AIMGBackground;
  IMGBlocks = AIMGBlocks;
  SNDDelete = ASNDDelete;
  SNDError = ASNDError;
  SNDBlockSelect = ASNDBlockSelect;

  NeedToKillBlocks = false;
  NeedToAddBlocks = false;
  NumSelected = 0;
  SelectedColor = -1;
  for (Y = 0; Y < NrOfRows; Y++)
    for (X = 0; X < NrOfCols; X++)
      Items[X][Y] = NULL;
  for (Teller = 0; Teller < 4; Teller++) {
    Selects[Teller].X = 0;
    Selects[Teller].Y = 0;
  }
}

void CWorldParts::NewGame() {
  int X, Y;
  for (Y = 0; Y < NrOfRows; Y++)
    for (X = 0; X < NrOfCols; X++) {
      if (Items[X][Y])
        delete Items[X][Y];
      Items[X][Y] = new CBlock(X, Y, rand() % NrOfBlockColors, IMGBlocks);
    }
  NeedToKillBlocks = false;
  NeedToAddBlocks = false;
  NumSelected = 0;
  SelectedColor = -1;
}

CWorldParts::~CWorldParts() {
  int X, Y;
  for (Y = 0; Y < NrOfRows; Y++)
    for (X = 0; X < NrOfCols; X++)
      if (Items[X][Y])
        delete Items[X][Y];
}

void CWorldParts::Draw(SDL_Renderer *Renderer) {
  if (NeedToKillBlocks && (Time < SDL_GetTicks())) {
    KillBlocks();
    NeedToKillBlocks = false;
    NeedToAddBlocks = true;
    Time = SDL_GetTicks() + 350;
    Mix_PlayChannel(-1, *SNDDelete, 0);
  }

  if (NeedToAddBlocks && (Time < SDL_GetTicks())) {
    AddBlocks();
    NeedToAddBlocks = false;
    NumSelected = 0;
  }

  SDL_RenderCopy(Renderer, *IMGBackground, NULL, NULL);
  int X, Y;
  for (Y = 0; Y < NrOfRows; Y++)
    for (X = 0; X < NrOfCols; X++)
      Items[X][Y]->Draw(Renderer);
}

void CWorldParts::DeSelect(bool PlaySound) {
  if (GlobalSoundEnabled)
    if (PlaySound)
      Mix_PlayChannel(-1, *SNDError, 0);
  int Teller = 0;
  for (Teller = 0; Teller < NumSelected; Teller++)
    Items[Selects[Teller].X][Selects[Teller].Y]->DeSelect();
  NumSelected = 0;
  SelectedColor = -1;
}

long CWorldParts::Select(int PlayFieldX, int PlayFieldY) {

  int Score = 0;
  if (!NeedToKillBlocks && !NeedToAddBlocks) {
    if (!Items[PlayFieldX][PlayFieldY]->IsSelected()) {

      if (NumSelected == 0) {
        Items[PlayFieldX][PlayFieldY]->Select();
        SelectedColor = Items[PlayFieldX][PlayFieldY]->GetColor();
        Selects[NumSelected].X = PlayFieldX;
        Selects[NumSelected].Y = PlayFieldY;
        NumSelected++;
        Mix_PlayChannel(-1, *SNDBlockSelect, 0);

      } else {
        if (Items[PlayFieldX][PlayFieldY]->GetColor() == SelectedColor) {
          int NumEqualY = 0, NumEqualX = 0;

          Items[PlayFieldX][PlayFieldY]->Select();
          Selects[NumSelected].X = PlayFieldX;
          Selects[NumSelected].Y = PlayFieldY;
          NumSelected++;
          if (NumSelected > 2) {
            for (int Teller1 = 0; Teller1 < NumSelected; Teller1++) {
              for (int Teller2 = 0; Teller2 < NumSelected; Teller2++)
                if (Teller1 != Teller2) {
                  if (Selects[Teller1].X == Selects[Teller2].X)
                    NumEqualX++;
                  if (Selects[Teller1].Y == Selects[Teller2].Y)
                    NumEqualY++;
                }
            }
            if ((NumEqualX > 4) || (NumEqualY > 4))
              DeSelect(true);
          }

          if (NumSelected == 3)
            if (!(((NumEqualX == 2) && (NumEqualY == 2))))
              DeSelect(true);

          if (NumSelected == 4) {
            if ((NumEqualX == 4) && (NumEqualY == 4)) {
              // Mix_PlayChannel(-1,Sounds[SND_BLOCKSELECT],0);
              int EndX = -1, EndY = -1, StartX = NrOfCols, StartY = NrOfRows;
              for (int Teller1 = 0; Teller1 < NumSelected; Teller1++) {
                if (Selects[Teller1].X > EndX)
                  EndX = Selects[Teller1].X;
                if (Selects[Teller1].X < StartX)
                  StartX = Selects[Teller1].X;
                if (Selects[Teller1].Y > EndY)
                  EndY = Selects[Teller1].Y;
                if (Selects[Teller1].Y < StartY)
                  StartY = Selects[Teller1].Y;
              }
              Score = (EndY - StartY + 1) * (EndX - StartX + 1) * 100;
              NeedToKillBlocks = true;
              Time = SDL_GetTicks() + 350;
            } else
              DeSelect(true);
          }

          if (((NumSelected <= 4) && (NumSelected > 0)))
            Mix_PlayChannel(-1, *SNDBlockSelect, 0);
        } else
          DeSelect(true);
      }

    } else
      DeSelect(true);
  }
  return Score;
}

void CWorldParts::KillBlocks() {
  int X, Y, Teller = 0, StartX = NrOfCols, StartY = NrOfRows, EndX = -1,
            EndY = -1;
  for (Teller = 0; Teller < NumSelected; Teller++) {
    if (Selects[Teller].X > EndX)
      EndX = Selects[Teller].X;
    if (Selects[Teller].X < StartX)
      StartX = Selects[Teller].X;
    if (Selects[Teller].Y > EndY)
      EndY = Selects[Teller].Y;
    if (Selects[Teller].Y < StartY)
      StartY = Selects[Teller].Y;
  }
  for (Y = StartY; Y <= EndY; Y++)
    for (X = StartX; X <= EndX; X++)
      Items[X][Y]->Kill();
}

void CWorldParts::AddBlocks() {
  int X, Y;
  for (Y = 0; Y < NrOfRows; Y++)
    for (X = 0; X < NrOfCols; X++)
      if (Items[X][Y]->NeedToKill()) {
        delete Items[X][Y];
        Items[X][Y] = new CBlock(X, Y, rand() % NrOfBlockColors, IMGBlocks);
      }
}
