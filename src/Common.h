#ifndef COMMON_H
#define COMMON_H

enum GameStates {
  GSQuit,
  GSIntro,
  GSGame,
  GSTitleScreen,
  GSTimeOver,
  GSReadyGo,
  GSCredits,
  GSGameTypeMenu,
  GSShowHighScores
};

enum GameTypes { Fixed = 0, Relative = 1 };

const int NrOfRows = 10, NrOfCols = 10, TileWidth = 23, TileHeight = 23,
          NrOfBlockColors = 5;

struct SPoint {
  int X, Y;
};

struct SHighScore {
  char PName[21];
  int PScore;
};

#endif
