#ifndef CSELECTOR_H
#define CSELECTOR_H

#include "Common.h"
#include <SDL.h>

class CSelector {
private:
  SPoint SelectedPoint, CurrentPoint;

  SDL_Texture **IMGCursor;

public:
  bool HasSelection;
  CSelector(const int PlayFieldXin, const int PlayFieldYin,
            SDL_Texture **AIMGCursor);
  ~CSelector();
  void SetPosition(const int PlayFieldXin, const int PlayFieldYin);
  void Select();
  SPoint GetSelection();
  SPoint GetPosition();
  void DeSelect();
  void Draw(SDL_Renderer *Renderer);
};

#endif
