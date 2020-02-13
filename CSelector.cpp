#include "CSelector.h"

// constructor, parameters define the starting position
CSelector::CSelector(const int PlayFieldXin, const int PlayFieldYin,
                     SDL_Texture **AIMGCursor) {
  IMGCursor = AIMGCursor;
  SelectedPoint.X = 0; // there's no selection set it 0
  SelectedPoint.Y = 0;
  CurrentPoint.X = PlayFieldXin; // set current position
  CurrentPoint.Y = PlayFieldYin;
  HasSelection = false; // No selection when it's created
}

CSelector::~CSelector() {}

// Will set the position only if it layes within the board boundary
void CSelector::SetPosition(const int PlayFieldXin, const int PlayFieldYin) {
  // check if the new position is inside the board, if so set the new position
  // as the currentposition
  if ((PlayFieldYin >= 0) && (PlayFieldYin < NrOfRows) && (PlayFieldXin >= 0) &&
      (PlayFieldXin < NrOfCols)) {
    CurrentPoint.X = PlayFieldXin;
    CurrentPoint.Y = PlayFieldYin;
  } else {
    if (PlayFieldYin < 0) {
      CurrentPoint.X = PlayFieldXin;
      CurrentPoint.Y = NrOfRows - 1;
    }

    if (PlayFieldYin >= NrOfRows) {
      CurrentPoint.X = PlayFieldXin;
      CurrentPoint.Y = 0;
    }

    if (PlayFieldXin < 0) {
      CurrentPoint.X = NrOfCols - 1;
      CurrentPoint.Y = PlayFieldYin;
    }

    if (PlayFieldXin >= NrOfCols) {
      CurrentPoint.X = 0;
      CurrentPoint.Y = PlayFieldYin;
    }
  }
}

// Select will set the current position as the selected position
void CSelector::Select() {
  SelectedPoint.X = CurrentPoint.X;
  SelectedPoint.Y = CurrentPoint.Y;
  HasSelection = true;
}

// return the selected position
SPoint CSelector::GetSelection() { return SelectedPoint; }

// return the current position
SPoint CSelector::GetPosition() { return CurrentPoint; }

void CSelector::DeSelect() { HasSelection = false; }

// Draw the blue box on the current position, with the offsets in mind
void CSelector::Draw(SDL_Renderer *Renderer) {
  SDL_Rect DstRect;
  DstRect.x = CurrentPoint.X * TileWidth + 5;
  DstRect.y = CurrentPoint.Y * TileHeight + 5;
  DstRect.w = TileWidth;
  DstRect.h = TileHeight;
  SDL_RenderCopy(Renderer, *IMGCursor, NULL, &DstRect);
}
