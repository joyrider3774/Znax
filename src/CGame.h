#ifndef CGAME_H
#define CGAME_H

#include "CWorldParts.h"
#include "Common.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <dirent.h>
#include <limits.h>

#ifdef WIN32
#include <windows.h>
#ifndef MAX_PATH
#define MAX_PATH PATH_MAX
#endif
#endif

class CGame {
private:
  const static int NrOfSounds = 14, BlockBlue = 0, BlockYellow = 1,
                   BlockOrange = 2, BlockGreen = 3, BlockRed = 4,
                   MaxSkins = 100, MaxMusicFiles = 26, MaxAlpha = 245,
                   AlphaIncrease = 5, MaxAlphaTime = 1500, SND_READYGO = 0,
                   SND_5MINUTE = 1, SND_1MINUTE = 2, SND_3 = 3, SND_2 = 4,
                   SND_1 = 5, SND_TIMEOVER = 6, SND_MENU = 7, SND_SELECT = 8,
                   SND_WELCOME = 9, SND_GOODBYE = 10, SND_DELETE = 11,
                   SND_BLOCKSELECT = 12, SND_ERROR = 13, SCALEFACTOR = 2;

  SDL_Texture *IMGBackground = NULL, *IMGBlocks = NULL, *IMGCursor = NULL,
              *IMGReady = NULL, *IMGGo = NULL, *IMGTimeOver = NULL,
              *IMGIntro1 = NULL, *IMGIntro2 = NULL, *IMGIntro3 = NULL,
              *IMGTitleScreen = NULL, *IMGQuit1 = NULL, *IMGQuit2 = NULL,
              *IMGPlay1 = NULL, *IMGPlay2 = NULL, *IMGHighScores1 = NULL,
              *IMGHighScores2 = NULL, *IMGCredits1 = NULL, *IMGCredits2 = NULL,
              *IMGCredits = NULL, *IMGSelectGame = NULL, *IMGFixedTimer1 = NULL,
              *IMGFixedTimer2 = NULL, *IMGRelativeTimer1 = NULL,
              *IMGRelativeTimer2 = NULL, *IMGHighScores = NULL;

  Mix_Chunk *Sounds[NrOfSounds] = {NULL};
  Mix_Music *Music[MaxMusicFiles] = {NULL};
  SDL_Renderer *Renderer = NULL;
  SDL_Window *SdlWindow = NULL;
  TTF_Font *font = NULL, *BigFont = NULL, *MonoFont = NULL;
  GameStates GameState = GSIntro;
  int MusicCount = 0, SelectedMusic = 0, Volume = 128, InitialStartScreenX = 20,
      InitialStartScreenY = 24, InstalledSkinsCount = 0, SelectedSkin = -1,
      Score = 0;
  char InstalledSkins[MaxSkins][1024];
  char SkinName[1024];
  CWorldParts *World = NULL;
  GameTypes GameType = Fixed;
  SHighScore HighScores[2][10];
  char DataPath[PATH_MAX] = {'\0'};
  bool GlobalSoundEnabled = true;
  bool ShowFPS = false;
  SDL_Texture *TextureBuffer1 = NULL, *TextureBuffer2 = NULL;

public:
  CGame();
  ~CGame();
  char chr(int ascii);
  int ord(char chr);
  bool FileExists(char *FileName);
  void UnloadMusic();
  void SearchForMusic();
  void SearchForSkins();
  void UnLoadGraphics();
  void LoadGraphics();
  void SetVolume(const int VolumeIn);
  void IncVolume();
  void DecVolume();
  void LoadSettings();
  void SaveSettings();
  void NextSkin(SDL_Texture *Texture);
  void PreviousSkin(SDL_Texture *Texture);
  void GameTypeMenu();
  void Credits();
  void TitleScreen();
  void ToggleFullscreen();
  void GetHighScoreName(char NameIn[21], int Place, int PScore);
  void TimeOver();
  void ReadyGo();
  void Intro();
  void Game();
  void LoadHighScores();
  void SaveHighScores();
  void ShowHighScores();
  void LoadSounds();
  void UnloadSounds();
  void GetFilePath(char *InputFile, char *result);
  void Run(int argc, char **argv);
  void WriteText(TTF_Font *FontIn, char *Tekst, int NrOfChars, int X, int Y,
                 int YSpacing, SDL_Color ColorIn);
};

#endif
