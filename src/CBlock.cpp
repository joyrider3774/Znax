#include "CBlock.h"
#include "Common.h"

CBlock::CBlock(int PlayFieldXin, int PlayFieldYin, int ColorIn,
               SDL_Texture **AIMGBlocks) {
  IMGBlocks = AIMGBlocks;
  AnimBase = 0;
  AnimPhase = 0;
  AnimCounter = 0;
  AnimDelayCounter = 0;
  AnimPhases = 1;
  AnimDelay = 2;
  PlayFieldX = PlayFieldXin;
  PlayFieldY = PlayFieldYin;
  Color = ColorIn;
  bNeedToKill = false;
  bSelected = false;
}

void CBlock::Select() {
  AnimBase = 0;
  AnimCounter = 0;
  AnimPhases = 6;
  bSelected = true;
}

void CBlock::DeSelect() {
  AnimBase = 0;
  AnimCounter = 0;
  AnimPhases = 1;
  bSelected = false;
}

void CBlock::Kill() {
  AnimBase = 6;
  AnimCounter = 0;
  AnimPhases = 1;
  bNeedToKill = true;
}

void CBlock::Draw(SDL_Renderer *Renderer) {
  SDL_Rect Dst, Src;

  Src.x = AnimPhase * TileWidth;
  Src.y = Color * TileHeight;
  Src.w = TileWidth;
  Src.h = TileHeight;

  Dst.x = PlayFieldX * TileWidth + 5;
  Dst.y = PlayFieldY * TileHeight + 5;
  Dst.w = TileWidth;
  Dst.h = TileHeight;

  SDL_RenderCopy(Renderer, *IMGBlocks, &Src, &Dst);

  AnimPhase = AnimBase + AnimCounter;
  if (AnimPhase != AnimBase + AnimPhases - 1) {
    AnimDelayCounter++;
    if (AnimDelayCounter >= AnimDelay) {
      AnimDelayCounter = 0;
      AnimCounter++;
      if (AnimCounter == AnimPhases)
        AnimCounter = 0;
    }
  }
}

int CBlock::GetColor() { return Color; }

bool CBlock::IsSelected() { return bSelected; }

bool CBlock::NeedToKill() { return bNeedToKill; }

CBlock::~CBlock() {}
