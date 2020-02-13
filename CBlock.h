#ifndef CBLOCK_H
#define CBLOCK_H

#include <SDL.h>

class CBlock {
private:
  int Color, AnimCounter, AnimBase, AnimDelay, AnimDelayCounter, AnimPhases,
      AnimPhase, PlayFieldX, PlayFieldY;
  bool bNeedToKill, bSelected;
  SDL_Texture **IMGBlocks;

public:
  CBlock(int PlayFieldXin, int PlayFieldYin, int ColorIn,
         SDL_Texture **AIMGBlocks);
  void Select();
  void DeSelect();
  void Kill();
  void Draw(SDL_Renderer *Renderer);
  int GetColor();
  bool IsSelected();
  bool NeedToKill();
  ~CBlock();
};

#endif
