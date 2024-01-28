#include "CGame.h"
#include "CGameTypeMenu.h"
#include "CMainMenu.h"
#include "CSelector.h"
#include "CWorldParts.h"
#include "Common.h"
#include <SDL.h>
#include <SDL_joystick.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>
#include <dirent.h>
#include <fstream>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

CGame::CGame() {}

CGame::~CGame() {}

char CGame::chr(int ascii) { return ((char)ascii); }

int CGame::ord(char chr) { return ((int)chr); }

bool CGame::FileExists(char *FileName) {
  FILE *Fp;
  Fp = fopen(FileName, "rb");
  if (Fp) {
    fclose(Fp);
    return true;
  } else
    return false;
}

void CGame::UnloadMusic() {
  if (GlobalSoundEnabled) {
    Mix_HaltMusic();
    for (int Teller = 0; Teller < MusicCount; Teller++)
      if (Music[Teller])
        Mix_FreeMusic(Music[Teller]);
  }
}

void CGame::ResetButtons() {
  ButLeft = false;
  ButRight = false;
  ButUp = false;
  ButDown = false;
  ButNextSkin = false; 
  ButVolDown = false;
  ButVolUp = false;
  ButNextMusic = false;
  ButBack = false; 
  ButA = false; 
  ButB = false;
  ButFullscreen = false;
  ButStart = false;
}

void CGame::HandleJoystickEvent(int Button) {
  switch (Button)
  {
	#if defined(RG35XX_PLUS_BATOCERA)
    case SDL_CONTROLLER_BUTTON_X:
	#else
	case SDL_CONTROLLER_BUTTON_Y:
	#endif
      ButNextSkin = true;
      break;
    #if defined(RG35XX_PLUS_BATOCERA)
    case SDL_CONTROLLER_BUTTON_Y:
	#else
	case SDL_CONTROLLER_BUTTON_X:
	#endif
      ButNextMusic = true;
      break;
    case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
      ButVolDown = true;
      break;
    case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
      ButVolUp = true;
      break;
    case SDL_CONTROLLER_BUTTON_DPAD_UP:
      ButUp = true;
      break;
    case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
      ButDown = true;
      break;
    case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
      ButLeft = true;
      break;
    case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
      ButRight = true;
      break;
	//trimui have buttons swapped
	#if defined(TRIMUI_SMART_PRO) || defined(RG35XX_PLUS_BATOCERA)
	case SDL_CONTROLLER_BUTTON_B:
	#else
    case SDL_CONTROLLER_BUTTON_A:
	#endif
      ButA = true;
      break;
   #if defined(TRIMUI_SMART_PRO) || defined(RG35XX_PLUS_BATOCERA)
	case SDL_CONTROLLER_BUTTON_A:
	#else
	case SDL_CONTROLLER_BUTTON_B:
	#endif
      ButB = true;
      break;
    case SDL_CONTROLLER_BUTTON_START:
      ButStart = true;
      break;
    case SDL_CONTROLLER_BUTTON_BACK:
      ButBack = true;
      break;
	#if defined(TRIMUI_SMART_PRO) || defined(RG35XX_PLUS_BATOCERA)
	case SDL_CONTROLLER_BUTTON_GUIDE:
      GameState = GSQuit;
      break;
	#endif
    default:
      break;
  }
}

void CGame::HandleKeyboardEvent(int Key) {
  switch (Key) {
    case SDLK_f:
      ButFullscreen = true;
      break;
    case SDLK_m:
      ButNextMusic = true;
      break;
    case SDLK_l:
      ButNextSkin = true;
      break;
    case SDLK_PAGEDOWN:
      ButVolDown = true;
      break;
    case SDLK_PAGEUP:
      ButVolUp = true;
      break;
    case SDLK_UP:
      ButUp = true;
      break;
    case SDLK_DOWN:
      ButDown = true;
      break;
    case SDLK_LEFT:
      ButLeft = true;
      break;
    case SDLK_RIGHT:
      ButRight = true;
      break;
    case SDLK_RETURN:
      ButStart = true;
      break;
    case SDLK_ESCAPE:
      ButBack = true;
      break;
    case SDLK_SPACE:
      ButA = true;
      break;
    case SDLK_b:
      ButB = true;
      break;
    default:
      break;
  }
}

void CGame::ToggleFullscreen() {
  // reset window size first before we go fullscreen
  // it will give more fps on the rpi if we for example scaled the window
  // first
  Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN;
  bool IsFullscreen = SDL_GetWindowFlags(SdlWindow) & FullscreenFlag;
  if (!IsFullscreen) {
    SDL_SetWindowSize(SdlWindow, 320 * SCALEFACTOR, 240 * SCALEFACTOR);
  }
  SDL_SetWindowFullscreen(SdlWindow, IsFullscreen ? 0 : FullscreenFlag);
  IsFullscreen = SDL_GetWindowFlags(SdlWindow) & FullscreenFlag;
  if (!IsFullscreen) {
    SDL_SetWindowSize(SdlWindow, 320 * SCALEFACTOR, 240 * SCALEFACTOR);
    SDL_SetWindowPosition(SdlWindow, SDL_WINDOWPOS_CENTERED,
                          SDL_WINDOWPOS_CENTERED);
  }
  SDL_ShowCursor(SDL_ENABLE);
}

void CGame::SearchForMusic() {
  DIR *Directory;
  struct stat Stats;
  int Teller;
  char FileName[PATH_MAX + FILENAME_MAX +
                20]; //+20 to get rid of warnings from sprintf stuff not sure
                     // how to fix
  char Path[PATH_MAX + 20]; //+20 to get rid of warnings from sprintf stuff not
                            // sure how to fix
  if (GlobalSoundEnabled) {
    sprintf(FileName, "%smusic/title.ogg", DataPath);
    Music[0] = Mix_LoadMUS(FileName);
  }
  Teller = 1;
  sprintf(Path, "%smusic", DataPath);
  Directory = opendir(Path);
  if (Directory) {
    struct dirent *Entry = readdir(Directory);
    while (Entry) {
      sprintf(FileName, "%smusic/%s", DataPath, Entry->d_name);
      stat(FileName, &Stats);
      if (!S_ISDIR(Stats.st_mode)) {
        if (strncmp(".", Entry->d_name, 1) &&
            (strcmp("title.ogg", Entry->d_name) != 0) &&
            (Teller < MaxMusicFiles)) {
          if (GlobalSoundEnabled) {
            Music[Teller] = Mix_LoadMUS(FileName);
            Teller++;
          }
        }
      }
      Entry = readdir(Directory);
    }
    closedir(Directory);
  }
  MusicCount = Teller;
}

void CGame::SearchForSkins() {
  DIR *Directory;
  struct stat Stats;
  int Teller = 0;
  char Path[PATH_MAX + 20]; //+20 to get rid of warnings from sprintf stuff not
                            // sure how to fix
  sprintf(Path, "%sskins", DataPath);
  Directory = opendir(Path);
  if (Directory) {
    struct dirent *Entry = readdir(Directory);
    char FileName[PATH_MAX + FILENAME_MAX +
                  20]; //+20 to get rid of warnings from sprintf stuff not sure
                       // how to fix
    while (Entry) {
      sprintf(FileName, "%sskins/%s", DataPath, Entry->d_name);
      stat(FileName, &Stats);
      if (S_ISDIR(Stats.st_mode)) {
        if (strncmp(".", Entry->d_name, 1) && (Teller < MaxSkins) &&
            (strlen(Entry->d_name) < FILENAME_MAX - 1)) {
          sprintf(InstalledSkins[Teller], "%s", Entry->d_name);
          Teller++;
        }
      }
      Entry = readdir(Directory);
    }
    closedir(Directory);
  }
  InstalledSkinsCount = Teller;
  SelectedSkin = -1;
}

void CGame::UnLoadGraphics() {
  if (IMGHighScores)
    SDL_DestroyTexture(IMGHighScores);
  if (IMGSelectGame)
    SDL_DestroyTexture(IMGSelectGame);
  if (IMGFixedTimer2)
    SDL_DestroyTexture(IMGFixedTimer2);
  if (IMGRelativeTimer2)
    SDL_DestroyTexture(IMGRelativeTimer2);
  if (IMGFixedTimer1)
    SDL_DestroyTexture(IMGFixedTimer1);
  if (IMGRelativeTimer1)
    SDL_DestroyTexture(IMGRelativeTimer1);
  if (IMGIntro1)
    SDL_DestroyTexture(IMGIntro1);
  if (IMGIntro2)
    SDL_DestroyTexture(IMGIntro2);
  if (IMGIntro3)
    SDL_DestroyTexture(IMGIntro3);
  if (IMGTimeOver)
    SDL_DestroyTexture(IMGTimeOver);
  if (IMGBlocks)
    SDL_DestroyTexture(IMGBlocks);
  if (IMGBackground)
    SDL_DestroyTexture(IMGBackground);
  if (IMGCursor)
    SDL_DestroyTexture(IMGCursor);
  if (IMGReady)
    SDL_DestroyTexture(IMGReady);
  if (IMGGo)
    SDL_DestroyTexture(IMGGo);
  if (IMGPlay1)
    SDL_DestroyTexture(IMGPlay1);
  if (IMGPlay2)
    SDL_DestroyTexture(IMGPlay2);
  if (IMGCredits1)
    SDL_DestroyTexture(IMGCredits1);
  if (IMGCredits2)
    SDL_DestroyTexture(IMGCredits2);
  if (IMGQuit1)
    SDL_DestroyTexture(IMGQuit1);
  if (IMGQuit2)
    SDL_DestroyTexture(IMGQuit2);
  if (IMGHighScores1)
    SDL_DestroyTexture(IMGHighScores1);
  if (IMGHighScores2)
    SDL_DestroyTexture(IMGHighScores2);
  if (IMGTitleScreen)
    SDL_DestroyTexture(IMGTitleScreen);
  if (IMGCredits)
    SDL_DestroyTexture(IMGCredits);
}

void CGame::LoadGraphics() {
  char FileName[PATH_MAX + FILENAME_MAX +
                1000]; //+1000 to get rid of warnings from sprintf stuff not
                       // sure how to fix
  SDL_Surface *TmpSurface = NULL;

  if (IMGHighScores)
    SDL_DestroyTexture(IMGHighScores);
  if (IMGSelectGame)
    SDL_DestroyTexture(IMGSelectGame);
  if (IMGFixedTimer2)
    SDL_DestroyTexture(IMGFixedTimer2);
  if (IMGRelativeTimer2)
    SDL_DestroyTexture(IMGRelativeTimer2);
  if (IMGFixedTimer1)
    SDL_DestroyTexture(IMGFixedTimer1);
  if (IMGRelativeTimer1)
    SDL_DestroyTexture(IMGRelativeTimer1);
  if (IMGIntro1)
    SDL_DestroyTexture(IMGIntro1);
  if (IMGIntro2)
    SDL_DestroyTexture(IMGIntro2);
  if (IMGIntro3)
    SDL_DestroyTexture(IMGIntro3);
  if (IMGTimeOver)
    SDL_DestroyTexture(IMGTimeOver);
  if (IMGBlocks)
    SDL_DestroyTexture(IMGBlocks);
  if (IMGBackground)
    SDL_DestroyTexture(IMGBackground);
  if (IMGCursor)
    SDL_DestroyTexture(IMGCursor);
  if (IMGReady)
    SDL_DestroyTexture(IMGReady);
  if (IMGGo)
    SDL_DestroyTexture(IMGGo);
  if (IMGPlay1)
    SDL_DestroyTexture(IMGPlay1);
  if (IMGPlay2)
    SDL_DestroyTexture(IMGPlay2);
  if (IMGCredits1)
    SDL_DestroyTexture(IMGCredits1);
  if (IMGCredits2)
    SDL_DestroyTexture(IMGCredits2);
  if (IMGQuit1)
    SDL_DestroyTexture(IMGQuit1);
  if (IMGQuit2)
    SDL_DestroyTexture(IMGQuit2);
  if (IMGHighScores1)
    SDL_DestroyTexture(IMGHighScores1);
  if (IMGHighScores2)
    SDL_DestroyTexture(IMGHighScores2);
  if (IMGTitleScreen)
    SDL_DestroyTexture(IMGTitleScreen);
  if (IMGCredits)
    SDL_DestroyTexture(IMGCredits);

  sprintf(FileName, "%sskins/%s/highscores.png", DataPath, SkinName);
  if (FileExists(FileName))
    TmpSurface = IMG_Load(FileName);
  else {
    sprintf(FileName, "%sgraphics/highscores.png", DataPath);
    TmpSurface = IMG_Load(FileName);
  }
  IMGHighScores = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
  SDL_FreeSurface(TmpSurface);

  sprintf(FileName, "%sskins/%s/fixedtimer1.png", DataPath, SkinName);
  if (FileExists(FileName))
    TmpSurface = IMG_Load(FileName);
  else {
    sprintf(FileName, "%sgraphics/fixedtimer1.png", DataPath);
    TmpSurface = IMG_Load(FileName);
  }
  SDL_SetColorKey(TmpSurface, SDL_TRUE,
                  SDL_MapRGBA(TmpSurface->format, 255, 0, 255, 255));
  IMGFixedTimer1 = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
  SDL_FreeSurface(TmpSurface);

  sprintf(FileName, "%sskins/%s/relativetimer1.png", DataPath, SkinName);
  if (FileExists(FileName))
    TmpSurface = IMG_Load(FileName);
  else {
    sprintf(FileName, "%sgraphics/relativetimer1.png", DataPath);
    TmpSurface = IMG_Load(FileName);
  }
  SDL_SetColorKey(TmpSurface, SDL_TRUE,
                  SDL_MapRGBA(TmpSurface->format, 255, 0, 255, 255));
  IMGRelativeTimer1 = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
  SDL_FreeSurface(TmpSurface);

  sprintf(FileName, "%sskins/%s/fixedtimer2.png", DataPath, SkinName);
  if (FileExists(FileName))
    TmpSurface = IMG_Load(FileName);
  else {
    sprintf(FileName, "%sgraphics/fixedtimer2.png", DataPath);
    TmpSurface = IMG_Load(FileName);
  }
  SDL_SetColorKey(TmpSurface, SDL_TRUE,
                  SDL_MapRGBA(TmpSurface->format, 255, 0, 255, 255));
  IMGFixedTimer2 = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
  SDL_FreeSurface(TmpSurface);

  sprintf(FileName, "%sskins/%s/relativetimer2.png", DataPath, SkinName);
  if (FileExists(FileName))
    TmpSurface = IMG_Load(FileName);
  else {
    sprintf(FileName, "%sgraphics/relativetimer2.png", DataPath);
    TmpSurface = IMG_Load(FileName);
  }
  SDL_SetColorKey(TmpSurface, SDL_TRUE,
                  SDL_MapRGBA(TmpSurface->format, 255, 0, 255, 255));
  IMGRelativeTimer2 = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
  SDL_FreeSurface(TmpSurface);

  sprintf(FileName, "%sskins/%s/selectgame.png", DataPath, SkinName);
  if (FileExists(FileName))
    TmpSurface = IMG_Load(FileName);
  else {
    sprintf(FileName, "%sgraphics/selectgame.png", DataPath);
    TmpSurface = IMG_Load(FileName);
  }
  SDL_SetColorKey(TmpSurface, SDL_TRUE,
                  SDL_MapRGBA(TmpSurface->format, 255, 0, 255, 255));
  IMGSelectGame = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
  SDL_FreeSurface(TmpSurface);

  sprintf(FileName, "%sskins/%s/credits.png", DataPath, SkinName);
  if (FileExists(FileName))
    TmpSurface = IMG_Load(FileName);
  else {
    sprintf(FileName, "%sgraphics/credits.png", DataPath);
    TmpSurface = IMG_Load(FileName);
  }
  SDL_SetColorKey(TmpSurface, SDL_TRUE,
                  SDL_MapRGBA(TmpSurface->format, 255, 0, 255, 255));
  IMGCredits = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
  SDL_FreeSurface(TmpSurface);

  sprintf(FileName, "%sskins/%s/titlescreen.png", DataPath, SkinName);
  if (FileExists(FileName))
    TmpSurface = IMG_Load(FileName);
  else {
    sprintf(FileName, "%sgraphics/titlescreen.png", DataPath);
    TmpSurface = IMG_Load(FileName);
  }
  SDL_SetColorKey(TmpSurface, SDL_TRUE,
                  SDL_MapRGBA(TmpSurface->format, 255, 0, 255, 255));
  IMGTitleScreen = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
  SDL_FreeSurface(TmpSurface);

  sprintf(FileName, "%sskins/%s/play1.png", DataPath, SkinName);
  if (FileExists(FileName))
    TmpSurface = IMG_Load(FileName);
  else {
    sprintf(FileName, "%sgraphics/play1.png", DataPath);
    TmpSurface = IMG_Load(FileName);
  }
  SDL_SetColorKey(TmpSurface, SDL_TRUE,
                  SDL_MapRGBA(TmpSurface->format, 255, 0, 255, 255));
  IMGPlay1 = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
  SDL_FreeSurface(TmpSurface);

  sprintf(FileName, "%sskins/%s/play2.png", DataPath, SkinName);
  if (FileExists(FileName))
    TmpSurface = IMG_Load(FileName);
  else {
    sprintf(FileName, "%sgraphics/play2.png", DataPath);
    TmpSurface = IMG_Load(FileName);
  }
  SDL_SetColorKey(TmpSurface, SDL_TRUE,
                  SDL_MapRGBA(TmpSurface->format, 255, 0, 255, 255));
  IMGPlay2 = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
  SDL_FreeSurface(TmpSurface);

  sprintf(FileName, "%sskins/%s/quit1.png", DataPath, SkinName);
  if (FileExists(FileName))
    TmpSurface = IMG_Load(FileName);
  else {
    sprintf(FileName, "%sgraphics/quit1.png", DataPath);
    TmpSurface = IMG_Load(FileName);
  }
  SDL_SetColorKey(TmpSurface, SDL_TRUE,
                  SDL_MapRGBA(TmpSurface->format, 255, 0, 255, 255));
  IMGQuit1 = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
  SDL_FreeSurface(TmpSurface);

  sprintf(FileName, "%sskins/%s/quit2.png", DataPath, SkinName);
  if (FileExists(FileName))
    TmpSurface = IMG_Load(FileName);
  else {
    sprintf(FileName, "%sgraphics/quit2.png", DataPath);
    TmpSurface = IMG_Load(FileName);
  }
  SDL_SetColorKey(TmpSurface, SDL_TRUE,
                  SDL_MapRGBA(TmpSurface->format, 255, 0, 255, 255));
  IMGQuit2 = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
  SDL_FreeSurface(TmpSurface);

  sprintf(FileName, "%sskins/%s/credits2.png", DataPath, SkinName);
  if (FileExists(FileName))
    TmpSurface = IMG_Load(FileName);
  else {
    sprintf(FileName, "%sgraphics/credits2.png", DataPath);
    TmpSurface = IMG_Load(FileName);
  }
  SDL_SetColorKey(TmpSurface, SDL_TRUE,
                  SDL_MapRGBA(TmpSurface->format, 255, 0, 255, 255));
  IMGCredits2 = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
  SDL_FreeSurface(TmpSurface);

  sprintf(FileName, "%sskins/%s/credits1.png", DataPath, SkinName);
  if (FileExists(FileName))
    TmpSurface = IMG_Load(FileName);
  else {
    sprintf(FileName, "%sgraphics/credits1.png", DataPath);
    TmpSurface = IMG_Load(FileName);
  }
  SDL_SetColorKey(TmpSurface, SDL_TRUE,
                  SDL_MapRGBA(TmpSurface->format, 255, 0, 255, 255));
  IMGCredits1 = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
  SDL_FreeSurface(TmpSurface);

  sprintf(FileName, "%sskins/%s/highscores2.png", DataPath, SkinName);
  if (FileExists(FileName))
    TmpSurface = IMG_Load(FileName);
  else {
    sprintf(FileName, "%sgraphics/highscores2.png", DataPath);
    TmpSurface = IMG_Load(FileName);
  }
  SDL_SetColorKey(TmpSurface, SDL_TRUE,
                  SDL_MapRGBA(TmpSurface->format, 255, 0, 255, 255));
  IMGHighScores2 = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
  SDL_FreeSurface(TmpSurface);

  sprintf(FileName, "%sskins/%s/highscores1.png", DataPath, SkinName);
  if (FileExists(FileName))
    TmpSurface = IMG_Load(FileName);
  else {
    sprintf(FileName, "%sgraphics/highscores1.png", DataPath);
    TmpSurface = IMG_Load(FileName);
  }
  SDL_SetColorKey(TmpSurface, SDL_TRUE,
                  SDL_MapRGBA(TmpSurface->format, 255, 0, 255, 255));
  IMGHighScores1 = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
  SDL_FreeSurface(TmpSurface);

  sprintf(FileName, "%sskins/%s/intro1.png", DataPath, SkinName);
  if (FileExists(FileName))
    TmpSurface = IMG_Load(FileName);
  else {
    sprintf(FileName, "%sgraphics/intro1.png", DataPath);
    TmpSurface = IMG_Load(FileName);
  }
  IMGIntro1 = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
  SDL_FreeSurface(TmpSurface);

  sprintf(FileName, "%sskins/%s/intro2.png", DataPath, SkinName);
  if (FileExists(FileName))
    TmpSurface = IMG_Load(FileName);
  else {
    sprintf(FileName, "%sgraphics/intro2.png", DataPath);
    TmpSurface = IMG_Load(FileName);
  }
  IMGIntro2 = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
  SDL_FreeSurface(TmpSurface);

  sprintf(FileName, "%sskins/%s/intro3.png", DataPath, SkinName);
  if (FileExists(FileName))
    TmpSurface = IMG_Load(FileName);
  else {
    sprintf(FileName, "%sgraphics/intro3.png", DataPath);
    TmpSurface = IMG_Load(FileName);
  }
  IMGIntro3 = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
  SDL_FreeSurface(TmpSurface);

  sprintf(FileName, "%sskins/%s/timeover.png", DataPath, SkinName);
  if (FileExists(FileName))
    TmpSurface = IMG_Load(FileName);
  else {
    sprintf(FileName, "%sgraphics/timeover.png", DataPath);
    TmpSurface = IMG_Load(FileName);
  }
  IMGTimeOver = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
  SDL_FreeSurface(TmpSurface);

  sprintf(FileName, "%sskins/%s/ready.png", DataPath, SkinName);
  if (FileExists(FileName))
    TmpSurface = IMG_Load(FileName);
  else {
    sprintf(FileName, "%sgraphics/ready.png", DataPath);
    TmpSurface = IMG_Load(FileName);
  }
  IMGReady = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
  SDL_FreeSurface(TmpSurface);

  sprintf(FileName, "%sskins/%s/go.png", DataPath, SkinName);
  if (FileExists(FileName))
    TmpSurface = IMG_Load(FileName);
  else {
    sprintf(FileName, "%sgraphics/go.png", DataPath);
    TmpSurface = IMG_Load(FileName);
  }
  IMGGo = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
  SDL_FreeSurface(TmpSurface);

  sprintf(FileName, "%sskins/%s/cursor.png", DataPath, SkinName);
  if (FileExists(FileName))
    TmpSurface = IMG_Load(FileName);
  else {
    sprintf(FileName, "%sgraphics/cursor.png", DataPath);
    TmpSurface = IMG_Load(FileName);
  }
  SDL_SetColorKey(TmpSurface, SDL_TRUE,
                  SDL_MapRGBA(TmpSurface->format, 255, 0, 255, 255));
  IMGCursor = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
  SDL_FreeSurface(TmpSurface);

  sprintf(FileName, "%sskins/%s/blocks.png", DataPath, SkinName);
  if (FileExists(FileName))
    TmpSurface = IMG_Load(FileName);
  else {
    sprintf(FileName, "%sgraphics/blocks.png", DataPath);
    TmpSurface = IMG_Load(FileName);
  }
  SDL_SetColorKey(TmpSurface, SDL_TRUE,
                  SDL_MapRGBA(TmpSurface->format, 255, 0, 255, 0));
  IMGBlocks = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
  SDL_FreeSurface(TmpSurface);

  sprintf(FileName, "%sskins/%s/background.png", DataPath, SkinName);
  if (FileExists(FileName))
    TmpSurface = IMG_Load(FileName);
  else {
    sprintf(FileName, "%sgraphics/background.png", DataPath);
    TmpSurface = IMG_Load(FileName);
  }
  IMGBackground = SDL_CreateTextureFromSurface(Renderer, TmpSurface);
  SDL_FreeSurface(TmpSurface);
}

// set the volume of the music and both channels (left/right)
void CGame::SetVolume(const int VolumeIn) {
  if (GlobalSoundEnabled) {
    Mix_Volume(0, VolumeIn);
    Mix_Volume(1, VolumeIn);
    Mix_VolumeMusic(VolumeIn);
  }
}

// increase the volume with 8
void CGame::IncVolume() {
  if (GlobalSoundEnabled) {
    if (Volume < 128) {
      Volume += 8;
      SetVolume(Volume);
    }
  }
}

// decrease the volume with 8
void CGame::DecVolume() {
  if (GlobalSoundEnabled) {
    if (Volume > 0) {
      Volume -= 8;
      SetVolume(Volume);
    }
  }
}

void CGame::LoadSettings() {
  FILE *SettingsFile;
  char FileName[PATH_MAX + FILENAME_MAX];
#ifdef WIN32
  sprintf(FileName, "%s%s/.znaxsettings", getenv("HOMEDRIVE"),
          getenv("HOMEPATH"));
#else
  sprintf(FileName, "%s/.znaxsettings", getenv("HOME"));
#endif
  SettingsFile = fopen(FileName, "r");
  if (SettingsFile) {
    fscanf(SettingsFile, "Volume=%d\n", &Volume);
    SetVolume(Volume);
    fscanf(SettingsFile, "Skin=%1023s\n", SkinName);
    for (int Teller = 0; Teller < InstalledSkinsCount; Teller++) {
      if (strcmp(InstalledSkins[Teller], SkinName) == 0) {
        SelectedSkin = Teller;
        break;
      } else
        SelectedSkin = -1;
    }
    fclose(SettingsFile);
  } else {
    SetVolume(128);
  }
}

void CGame::SaveSettings() {
  FILE *SettingsFile;
  char FileName[PATH_MAX + FILENAME_MAX];
#ifdef WIN32
  sprintf(FileName, "%s%s/.znaxsettings", getenv("HOMEDRIVE"),
          getenv("HOMEPATH"));
#else
  sprintf(FileName, "%s/.znaxsettings", getenv("HOME"));
#endif
  SettingsFile = fopen(FileName, "w");
  if (SettingsFile) {
    fprintf(SettingsFile, "Volume=%d\n", Volume);
    fprintf(SettingsFile, "Skin=%s\n", SkinName);
    fclose(SettingsFile);
  }
}

void CGame::NextSkin(SDL_Texture *Texture) {
  SDL_Color Color = {255, 255, 255, 255};
  if (InstalledSkinsCount > 0) {
    char Text[4096];
    SelectedSkin++;
    if (SelectedSkin >= InstalledSkinsCount)
      SelectedSkin = -1;
    if (SelectedSkin == -1) {
      sprintf(SkinName, "%s", "");
      sprintf(Text, "Please Wait Loading Skin:\nDefault Skin");
    } else {
      sprintf(SkinName, "%s", InstalledSkins[SelectedSkin]);
      sprintf(Text, "Please Wait Loading Skin:\n%s", SkinName);
    }

    SDL_SetRenderTarget(Renderer, Texture);
    boxRGBA(Renderer, 0, 0, 319, 48, 255, 255, 255, 200);
    boxRGBA(Renderer, 1, 1, 318, 47, 0, 0, 0, 200);
    WriteText(BigFont, Text, strlen(Text), 5, 0, 0, Color);

    SDL_SetRenderTarget(Renderer, NULL);
    SDL_RenderCopy(Renderer, Texture, NULL, NULL);
    SDL_RenderPresent(Renderer);

    LoadGraphics();

    SDL_SetRenderTarget(Renderer, Texture);
  }
}

void CGame::PreviousSkin(SDL_Texture *Texture) {
  if (InstalledSkinsCount > 0) {
    SDL_Color Color = {255, 255, 255, 255};
    char Text[4096];
    SelectedSkin--;
    if (SelectedSkin <= -2)
      SelectedSkin = InstalledSkinsCount - 1;
    if (SelectedSkin == -1) {
      sprintf(SkinName, "%s", "");
      sprintf(Text, "Please Wait Loading Skin:\nDefault Skin");
    } else {
      sprintf(SkinName, "%s", InstalledSkins[SelectedSkin]);
      sprintf(Text, "Please Wait Loading Skin:\n%s", SkinName);
    }

    SDL_SetRenderTarget(Renderer, Texture);
    boxRGBA(Renderer, 0, 0, 319, 48, 255, 255, 255, 200);
    boxRGBA(Renderer, 1, 1, 318, 47, 0, 0, 0, 200);
    WriteText(BigFont, Text, strlen(Text), 5, 0, 0, Color);

    SDL_SetRenderTarget(Renderer, NULL);
    SDL_RenderCopy(Renderer, Texture, NULL, NULL);
    SDL_RenderPresent(Renderer);

    LoadGraphics();

    SDL_SetRenderTarget(Renderer, Texture);
  }
}

void CGame::GameTypeMenu() {
  SDL_Event Event;
  Uint8 alpha = 0;
  int Selection;
  CGameTypeMenu *Menu =
      new CGameTypeMenu(&IMGTitleScreen, &IMGSelectGame, &IMGFixedTimer1,
                        &IMGFixedTimer2, &IMGRelativeTimer1, &IMGRelativeTimer2,
                        &Sounds[SND_MENU], GlobalSoundEnabled);
  SDL_SetTextureBlendMode(TextureBuffer1, SDL_BLENDMODE_BLEND);
  Uint32 AlphaTimer = SDL_GetTicks();
  while (GameState == GSGameTypeMenu) {
    if (MusicCount > 0)
      if (GlobalSoundEnabled)
        if (!Mix_PlayingMusic()) {
          SelectedMusic = 0;
          Mix_PlayMusic(Music[SelectedMusic], 0);
          SetVolume(Volume);
        }
        
    ResetButtons();
    while (SDL_PollEvent(&Event)) {
      if (Event.type == SDL_QUIT)
        GameState = GSQuit;
      
      if (Event.type == SDL_JOYDEVICEADDED)
        if(GameController == NULL)
          if(SDL_IsGameController(Event.jdevice.which))
            GameController = SDL_GameControllerOpen(Event.jdevice.which);

      if (Event.type == SDL_JOYDEVICEREMOVED)
      {
        SDL_Joystick* Joystick = SDL_GameControllerGetJoystick(GameController);
        if (Joystick)
          if (Event.jdevice.which == SDL_JoystickInstanceID(Joystick))
          {
            SDL_GameControllerClose(GameController);
            GameController = NULL;
          }
      }

      if (Event.type == SDL_CONTROLLERBUTTONUP)
        HandleJoystickEvent(Event.cbutton.button);

      if (Event.type == SDL_KEYUP)
        HandleKeyboardEvent(Event.key.keysym.sym);

      if ((Event.type == SDL_FINGERUP) || (Event.type == SDL_MOUSEBUTTONUP)) {
        int RealX;
        int RealY;
        if (Event.type == SDL_FINGERUP) {
          RealX = Event.tfinger.x * 320; //needs to be logical size
          RealY = Event.tfinger.y * 240; //needs to be logical size
        } else {
          RealX = Event.button.x;
          RealY = Event.button.y;
        }
        Selection = Menu->GetMouseSelection(RealX, RealY);
        if (Selection != -1) {
          if (GlobalSoundEnabled)
            Mix_PlayChannel(-1, Sounds[SND_SELECT], 0);
          switch (Selection) {
          case 0:
            GameType = Fixed;
            break;
          case 1:
            GameType = Relative;
            break;
          }
          GameState = GSReadyGo;
        }
      }
    }

     if (ButFullscreen)
      ToggleFullscreen();

    if (ButNextSkin)
      NextSkin(TextureBuffer1);

    if (ButVolDown)
      DecVolume();

    if (ButVolUp)
      IncVolume();

    if (ButUp)
      Menu->NextItem();

    if (ButDown)
      Menu->PreviousItem();

    if (ButStart || ButA) {
      if (GlobalSoundEnabled)
          Mix_PlayChannel(-1, Sounds[SND_SELECT], 0);
        switch (Menu->GetSelection()) {
          case Fixed:
            GameType = Fixed;
            break;
          case Relative:
            GameType = Relative;
            break;
        }
        GameState = GSReadyGo;
    }

    if (ButBack || ButB)
      GameState = GSTitleScreen;

    SDL_SetRenderTarget(Renderer, TextureBuffer1);
    Menu->Draw(Renderer);

    if (alpha < MaxAlpha) {
      alpha = trunc(MaxAlpha *
                    ((double)(SDL_GetTicks() - AlphaTimer) / MaxAlphaTime));

      if (alpha + AlphaIncrease >= MaxAlpha) {
        SDL_SetTextureBlendMode(TextureBuffer1, SDL_BLENDMODE_NONE);
        alpha = 255;
      }
      SDL_SetTextureAlphaMod(TextureBuffer1, alpha);
    }
    SDL_SetRenderTarget(Renderer, TextureBuffer2);
    SDL_RenderCopy(Renderer, TextureBuffer1, NULL, NULL);
    SDL_SetRenderTarget(Renderer, NULL);
    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
    SDL_RenderClear(Renderer);
    SDL_RenderCopy(Renderer, TextureBuffer2, NULL, NULL);
    SDL_RenderPresent(Renderer);
  }
  delete Menu;
}

void CGame::Credits() {
  SDL_Rect Rect1;
  SDL_Event Event;
  Uint8 alpha = 0;

  SDL_QueryTexture(IMGCredits, NULL, NULL, &Rect1.w, &Rect1.h);
  Rect1.x = 48;
  Rect1.y = 76;

  SDL_SetTextureBlendMode(TextureBuffer1, SDL_BLENDMODE_BLEND);
  Uint32 AlphaTimer = SDL_GetTicks();
  while (GameState == GSCredits) {
    if (MusicCount > 0)
      if (GlobalSoundEnabled)
        if (!Mix_PlayingMusic()) {
          SelectedMusic = 0;
          Mix_PlayMusic(Music[SelectedMusic], 0);
          SetVolume(Volume);
        }

    ResetButtons();
    while (SDL_PollEvent(&Event)) {

      if (Event.type == SDL_JOYDEVICEADDED)
        if(GameController == NULL)
          if(SDL_IsGameController(Event.jdevice.which))
            GameController = SDL_GameControllerOpen(Event.jdevice.which);

      if (Event.type == SDL_JOYDEVICEREMOVED)
      {
        SDL_Joystick* Joystick = SDL_GameControllerGetJoystick(GameController);
        if (Joystick)
          if (Event.jdevice.which == SDL_JoystickInstanceID(Joystick))
          {
            SDL_GameControllerClose(GameController);
            GameController = NULL;
          }
      }

      if (Event.type == SDL_QUIT)
        GameState = GSQuit;

      if ((Event.type == SDL_FINGERUP) || (Event.type == SDL_MOUSEBUTTONUP))
        ButA = true;

      if (Event.type == SDL_CONTROLLERBUTTONUP)  
        HandleJoystickEvent(Event.cbutton.button);
          
      if (Event.type == SDL_KEYUP)
        HandleKeyboardEvent (Event.key.keysym.sym);
    }

    if (ButFullscreen)
      ToggleFullscreen();

    if (ButNextSkin)
      NextSkin(TextureBuffer1);

    if (ButVolDown)
      DecVolume();

    if (ButVolUp)
      IncVolume();

    if (ButA || ButB || ButBack || ButStart)
      GameState = GSTitleScreen;

    SDL_SetRenderTarget(Renderer, TextureBuffer1);
    SDL_RenderCopy(Renderer, IMGTitleScreen, NULL, NULL);
    SDL_RenderCopy(Renderer, IMGCredits, NULL, &Rect1);

    if (alpha < MaxAlpha) {
      alpha = trunc(MaxAlpha *
                    ((double)(SDL_GetTicks() - AlphaTimer) / MaxAlphaTime));

      if (alpha + AlphaIncrease >= MaxAlpha) {
        SDL_SetTextureBlendMode(TextureBuffer1, SDL_BLENDMODE_NONE);
        alpha = 255;
      }
      SDL_SetTextureAlphaMod(TextureBuffer1, alpha);
    }
    SDL_SetRenderTarget(Renderer, TextureBuffer2);
    SDL_RenderCopy(Renderer, TextureBuffer1, NULL, NULL);
    SDL_SetRenderTarget(Renderer, NULL);
    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
    SDL_RenderClear(Renderer);
    SDL_RenderCopy(Renderer, TextureBuffer2, NULL, NULL);
    SDL_RenderPresent(Renderer);
  }
}

void CGame::TitleScreen() {
  GameType = Fixed;
  SDL_Event Event;
  int Selection;
  Uint8 alpha = 0;
  CMainMenu *Menu =
      new CMainMenu(&IMGTitleScreen, &IMGPlay1, &IMGPlay2, &IMGHighScores1,
                    &IMGHighScores2, &IMGCredits1, &IMGCredits2, &IMGQuit1,
                    &IMGQuit2, &Sounds[SND_MENU], GlobalSoundEnabled);
  Uint32 AlphaTimer = SDL_GetTicks();
  SDL_SetTextureBlendMode(TextureBuffer1, SDL_BLENDMODE_BLEND);
  while (GameState == GSTitleScreen) {
    if (MusicCount > 0)
      if (GlobalSoundEnabled)
        if (!Mix_PlayingMusic()) {
          SelectedMusic = 0;
          Mix_PlayMusic(Music[SelectedMusic], 0);
          SetVolume(Volume);
        }

    ResetButtons();
    while (SDL_PollEvent(&Event)) {
      if (Event.type == SDL_QUIT)
        GameState = GSQuit;

      if (Event.type == SDL_JOYDEVICEADDED)
        if(GameController == NULL)
          if(SDL_IsGameController(Event.jdevice.which))
            GameController = SDL_GameControllerOpen(Event.jdevice.which);

      if (Event.type == SDL_JOYDEVICEREMOVED)
      {
        SDL_Joystick* Joystick = SDL_GameControllerGetJoystick(GameController);
        if (Joystick)
          if (Event.jdevice.which == SDL_JoystickInstanceID(Joystick))
          {
            SDL_GameControllerClose(GameController);
            GameController = NULL;
          }
      }

      if ((Event.type == SDL_FINGERUP) || (Event.type == SDL_MOUSEBUTTONUP)) {
        int RealX;
        int RealY;
        if (Event.type == SDL_FINGERUP) {
          RealX = Event.tfinger.x * 320; //needs to be logical size
          RealY = Event.tfinger.y * 240; //needs to be logical size
        } else {
          RealX = Event.button.x;
          RealY = Event.button.y;
        }
        Selection = Menu->GetMouseSelection(RealX, RealY);
        if (Selection != -1) {
          if (Selection != 4)
            if (GlobalSoundEnabled)
              Mix_PlayChannel(-1, Sounds[SND_SELECT], 0);
          switch (Selection) {
          case 1:
            GameState = GSGameTypeMenu;
            break;
          case 2:
            GameState = GSShowHighScores;
            break;
          case 3:
            GameState = GSCredits;
            break;
          case 4:
            GameState = GSQuit;

            if (GlobalSoundEnabled) {
              Mix_HaltMusic();
              Mix_PlayChannel(-1, Sounds[SND_GOODBYE], 0);
            }
            SDL_Delay(750);
            break;
          }
        }
      }

      if (Event.type == SDL_CONTROLLERBUTTONUP)
        HandleJoystickEvent(Event.cbutton.button);

      if (Event.type == SDL_KEYUP)
        HandleKeyboardEvent(Event.key.keysym.sym);
    }

    if (ButFullscreen)
      ToggleFullscreen();

    if (ButNextSkin)
      NextSkin(TextureBuffer1);

    if (ButVolDown)
      DecVolume();

    if (ButVolUp)
      IncVolume();

    if (ButBack) {
      GameState = GSQuit;
      if (GlobalSoundEnabled) {
        Mix_HaltMusic();
        Mix_PlayChannel(-1, Sounds[SND_GOODBYE], 0);
      }
      SDL_Delay(750);
    }

    if (ButUp)
      Menu->PreviousItem();

    if (ButDown)
      Menu->NextItem();

    if (ButStart || ButA) {
      if (Menu->GetSelection() != 4)
        if (GlobalSoundEnabled)
          Mix_PlayChannel(-1, Sounds[SND_SELECT], 0);
      switch (Menu->GetSelection()) {
        case 1:
          GameState = GSGameTypeMenu;
          break;
        case 2:
          GameState = GSShowHighScores;
          break;
        case 3:
          GameState = GSCredits;
          break;
        case 4:
          GameState = GSQuit;
          if (GlobalSoundEnabled) {
            Mix_HaltMusic();
            Mix_PlayChannel(-1, Sounds[SND_GOODBYE], 0);
          }
          SDL_Delay(750);
          break;
      }
    }

    SDL_SetRenderTarget(Renderer, TextureBuffer1);
    Menu->Draw(Renderer);

    if (alpha < MaxAlpha) {
      alpha = trunc(MaxAlpha *
                    ((double)(SDL_GetTicks() - AlphaTimer) / MaxAlphaTime));

      if (alpha + AlphaIncrease >= MaxAlpha) {
        SDL_SetTextureBlendMode(TextureBuffer1, SDL_BLENDMODE_NONE);
        alpha = 255;
      }
      SDL_SetTextureAlphaMod(TextureBuffer1, alpha);
    }
    SDL_SetRenderTarget(Renderer, TextureBuffer2);
    SDL_RenderCopy(Renderer, TextureBuffer1, NULL, NULL);
    SDL_SetRenderTarget(Renderer, NULL);
    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
    SDL_RenderClear(Renderer);
    SDL_RenderCopy(Renderer, TextureBuffer2, NULL, NULL);
    SDL_RenderPresent(Renderer);
  }
  delete Menu;
}

void CGame::GetHighScoreName(char NameIn[21], int Place, int PScore) {
  char Name[21];
  char Msg[500];
  bool End = false, SubmitChanges = false;
  int Teller, MaxSelection = 0, Selection = 0, asci = 97;
  SDL_Color TextColor = {102, 115, 152, 255}, TextColor1 = {255, 115, 152, 255};
  SDL_Event Event;
  SDL_PollEvent(&Event);
  sprintf(Name, "%s", NameIn);
  MaxSelection = strlen(NameIn);
  Name[Selection + 1] = '\0';
  if (MaxSelection == 0)
    Name[MaxSelection] = chr(asci);
  char Tekst[100];
  while (!End) {
    if (MusicCount > 0)
      if (GlobalSoundEnabled)
        if (!Mix_PlayingMusic()) {
          SelectedMusic = 0;
          Mix_PlayMusic(Music[SelectedMusic], 0);
          SetVolume(Volume);
        }

    ResetButtons();
    while (SDL_PollEvent(&Event)) {

      if (Event.type == SDL_JOYDEVICEADDED)
        if(GameController == NULL)
          if(SDL_IsGameController(Event.jdevice.which))
            GameController = SDL_GameControllerOpen(Event.jdevice.which);

      if (Event.type == SDL_JOYDEVICEREMOVED)
      {
        SDL_Joystick* Joystick = SDL_GameControllerGetJoystick(GameController);
        if (Joystick)
          if (Event.jdevice.which == SDL_JoystickInstanceID(Joystick))
          {
            SDL_GameControllerClose(GameController);
            GameController = NULL;
          }
      }

      if ((Event.type == SDL_FINGERUP) || (Event.type == SDL_MOUSEBUTTONUP)) {
        ButStart = true;
      }

      if (Event.type == SDL_QUIT) {
        GameState = GSQuit;
        End = true;
        SubmitChanges = false;
      }

      if (Event.type == SDL_CONTROLLERBUTTONUP)
        HandleJoystickEvent(Event.cbutton.button);

      if (Event.type == SDL_KEYUP)
        HandleKeyboardEvent (Event.key.keysym.sym);
    }

    if (ButFullscreen)
      ToggleFullscreen();

    if (ButLeft) {
      if (Selection > 0) {
        Selection--;
        asci = ord(Name[Selection]);
      }
    }

    if (ButRight) {
      if (Selection < 19) {
        Selection++;
        if (Selection > MaxSelection) {
          Name[Selection] = chr(97);
          Name[Selection + 1] = '\0';
          MaxSelection = Selection;
        }
        asci = ord(Name[Selection]);
      }
    }

    if (ButUp) {
      asci++;
      if (asci == 123)
        asci = 32;
      if (asci == 33)
        (asci = 48);
      if (asci == 58)
        asci = 97;
      Name[Selection] = chr(asci);
    }

    if (ButDown) {
      asci--;
      if (asci == 96)
        asci = 57;
      if (asci == 47)
        asci = 32;
      if (asci == 31)
        asci = 122;
      Name[Selection] = chr(asci);
    }

    if (ButStart || ButA) {
      if (GlobalSoundEnabled)
        Mix_PlayChannel(-1, Sounds[SND_SELECT], 0);
      End = true;
      SubmitChanges = true;
    }

    if (ButBack) {
      End = true;
      SubmitChanges = false;
    }

    if (ButVolUp)
      IncVolume();

    if (ButVolDown)
      DecVolume();

    SDL_SetRenderTarget(Renderer, TextureBuffer1);
    SDL_RenderCopy(Renderer, IMGHighScores, NULL, NULL);

    for (Teller = 0; Teller < 9; Teller++) {
      if (Teller < Place) {
        sprintf(Msg, "%2d.%s", Teller + 1, HighScores[GameType][Teller].PName);
        WriteText(MonoFont, Msg, strlen(Msg), 50, (62 + Teller * 16), 0,
                  TextColor);
        sprintf(Msg, "%7d", HighScores[GameType][Teller].PScore);
        WriteText(MonoFont, Msg, strlen(Msg), 220, (62 + Teller * 16), 0,
                  TextColor);
      } else {
        sprintf(Msg, "%2d.%s", Teller + 2, HighScores[GameType][Teller].PName);
        WriteText(MonoFont, Msg, strlen(Msg), 50, (62 + (Teller + 1) * 16), 0,
                  TextColor);
        sprintf(Msg, "%7d", HighScores[GameType][Teller].PScore);
        WriteText(MonoFont, Msg, strlen(Msg), 220, (62 + (Teller + 1) * 16), 0,
                  TextColor);
      }
    }
    sprintf(Msg, "%2d.%s", Place + 1, Name);
    WriteText(MonoFont, Msg, strlen(Msg), 50, (62 + Place * 16), 0, TextColor1);
    sprintf(Msg, "%7d", PScore);
    WriteText(MonoFont, Msg, strlen(Msg), 220, (62 + Place * 16), 0,
              TextColor1);
    if (Selection > 0) {
      sprintf(Tekst, "    ");
      for (Teller = 1; Teller < Selection; Teller++) {
        strcat(Tekst, " ");
      };
      strcat(Tekst, "_");
    } else
      sprintf(Tekst, "   _");
    WriteText(MonoFont, Tekst, strlen(Tekst), 50, (64 + (Place)*16), 2,
              TextColor1);
    sprintf(Tekst, "Use Up,Down,Left,right. Enter = Confirm, Esc = Cancel");
    WriteText(font, Tekst, strlen(Tekst), 30, 227, 0, TextColor);

    SDL_SetRenderTarget(Renderer, TextureBuffer2);
    SDL_RenderCopy(Renderer, TextureBuffer1, NULL, NULL);
    SDL_SetRenderTarget(Renderer, NULL);
    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
    SDL_RenderClear(Renderer);
    SDL_RenderCopy(Renderer, TextureBuffer2, NULL, NULL);
    SDL_RenderPresent(Renderer);
  }
  Name[MaxSelection + 1] = '\0';
  while ((Name[0] == ' ') && (MaxSelection > -1)) {
    for (Teller = 0; Teller < MaxSelection; Teller++)
      Name[Teller] = Name[Teller + 1];
    MaxSelection--;
  }
  if (MaxSelection > -1)
    while ((Name[MaxSelection] == ' ') && (MaxSelection > 0)) {
      Name[MaxSelection] = '\0';
      MaxSelection--;
    }
  if (!SubmitChanges)
    sprintf(NameIn, "%s", " ");
  else
    sprintf(NameIn, "%s", Name);
}

void CGame::TimeOver() {
  char Name[21];
  int Teller1, Teller2;
  SDL_Rect Dst;
  SDL_Event Event;
  Uint8 alpha = 0;
  Uint32 AlphaTimer = SDL_GetTicks();
  Uint32 Time2 = SDL_GetTicks() + 1250;
  int Timer = 0, Counter = 0;
  char Text[4096];
  World->DeSelect(false);
  memset(Name, '\0', 21);
  SDL_SetTextureBlendMode(TextureBuffer1, SDL_BLENDMODE_BLEND);
  while (GameState == GSTimeOver) {

    ResetButtons();
    while (SDL_PollEvent(&Event)) {

      if (Event.type == SDL_JOYDEVICEADDED)
        if(GameController == NULL)
          if(SDL_IsGameController(Event.jdevice.which))
            GameController = SDL_GameControllerOpen(Event.jdevice.which);

      if (Event.type == SDL_JOYDEVICEREMOVED)
      {
        SDL_Joystick* Joystick = SDL_GameControllerGetJoystick(GameController);
        if (Joystick)
          if (Event.jdevice.which == SDL_JoystickInstanceID(Joystick))
          {
            SDL_GameControllerClose(GameController);
            GameController = NULL;
          }
      }

      if (Event.type == SDL_CONTROLLERBUTTONUP)
        HandleJoystickEvent(Event.cbutton.button);

      if (Event.type == SDL_KEYUP)
        HandleKeyboardEvent(Event.key.keysym.sym);
    }
    if (ButFullscreen)
      ToggleFullscreen();

    if (ButBack || ButStart || ButA)
      GameState = GSShowHighScores;

    if (ButNextSkin)
      NextSkin(TextureBuffer1);

    if (ButVolUp)
      IncVolume();

    if (ButVolDown)
      DecVolume();

    SDL_SetRenderTarget(Renderer, TextureBuffer1);
    World->Draw(Renderer);

    switch (Counter) {
    case 0:
      Dst.x = 23;
      Dst.y = 85;
      SDL_QueryTexture(IMGTimeOver, NULL, NULL, &Dst.w, &Dst.h);
      SDL_RenderCopy(Renderer, IMGTimeOver, NULL, &Dst);
      break;
    }
    if (alpha < MaxAlpha) {
      alpha = trunc(MaxAlpha *
                    ((double)(SDL_GetTicks() - AlphaTimer) / MaxAlphaTime));

      if (alpha + AlphaIncrease >= MaxAlpha) {
        SDL_SetTextureBlendMode(TextureBuffer1, SDL_BLENDMODE_NONE);
        alpha = 255;
      }
      SDL_SetTextureAlphaMod(TextureBuffer1, alpha);
    }
    sprintf(Text, "Timer:\n%2d:%02d\n\nScore:\n%d", Timer / 60, Timer % 60,
            Score);
    SDL_SetRenderTarget(Renderer, TextureBuffer2);
    SDL_RenderCopy(Renderer, TextureBuffer1, NULL, NULL);
    SDL_SetRenderTarget(Renderer, NULL);
    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
    SDL_RenderClear(Renderer);
    SDL_RenderCopy(Renderer, TextureBuffer2, NULL, NULL);
    SDL_RenderPresent(Renderer);
    if (Time2 < SDL_GetTicks()) {
      GameState = GSShowHighScores;
    }
  }
  for (Teller1 = 0; Teller1 < 10; Teller1++)
    if (HighScores[GameType][Teller1].PScore < Score) {
      GetHighScoreName(Name, Teller1, Score);
      for (Teller2 = 8; Teller2 >= Teller1; Teller2--)
        HighScores[GameType][Teller2 + 1] = HighScores[GameType][Teller2];
      sprintf(HighScores[GameType][Teller1].PName, "%s", Name);
      HighScores[GameType][Teller1].PScore = Score;
      break;
    }
}

void CGame::ReadyGo() {
  SDL_Rect Dst;
  SDL_Event Event;
  Uint8 alpha = 0;
  Uint32 Time1 = SDL_GetTicks();
  Uint32 Time2 = SDL_GetTicks() + 500;
  World->NewGame();
  Score = 0;
  int Timer = 600, Counter = 0;
  char Text[500];
  SDL_Color TextColor = {255, 255, 255, 255};
  if (GameType == Relative)
    Timer = 200;
  else
    Timer = 600;
  int FrameCount = 0, Fps = 0;

  if (GlobalSoundEnabled) {
    if (MusicCount > 1) {
      SelectedMusic = 1 + rand() % (MusicCount - 1);
      Mix_PlayMusic(Music[SelectedMusic], 0);
      SetVolume(Volume);
    } else if (MusicCount == 1) {
      if (!Mix_PlayingMusic()) {
        SelectedMusic = 0;
        Mix_PlayMusic(Music[SelectedMusic], 0);
        SetVolume(Volume);
      }
    }
  }
  SDL_SetTextureBlendMode(TextureBuffer1, SDL_BLENDMODE_BLEND);
  Uint32 AlphaTimer = SDL_GetTicks();
  while (GameState == GSReadyGo) {

    if (GlobalSoundEnabled) {
      if (!Mix_PlayingMusic()) {
        if (MusicCount > 1) {
          SelectedMusic = 1 + rand() % (MusicCount - 1);
          Mix_PlayMusic(Music[SelectedMusic], 0);
          SetVolume(Volume);
        } else if (MusicCount == 1) {
          SelectedMusic = 0;
          Mix_PlayMusic(Music[SelectedMusic], 0);
          SetVolume(Volume);
        }
      }
    }

    ResetButtons();
    while (SDL_PollEvent(&Event)) {
      if ((Event.type == SDL_FINGERUP) || (Event.type == SDL_MOUSEBUTTONUP)) {
        int RealX;
        int RealY;
        if (Event.type == SDL_FINGERUP) {
          RealX = Event.tfinger.x * 320; //needs to be logical size
          RealY = Event.tfinger.y * 240; //needs to be logical size
        } else {
          RealX = Event.button.x;
          RealY = Event.button.y;
        }
        if (((RealX > 247)) && (RealX < 290) && (RealY > 180) && (RealY < 200))
          GameState = GSTitleScreen;
      }

      if (Event.type == SDL_JOYDEVICEADDED)
        if(GameController == NULL)
          if(SDL_IsGameController(Event.jdevice.which))
            GameController = SDL_GameControllerOpen(Event.jdevice.which);

      if (Event.type == SDL_JOYDEVICEREMOVED)
      {
        SDL_Joystick* Joystick = SDL_GameControllerGetJoystick(GameController);
        if (Joystick)
          if (Event.jdevice.which == SDL_JoystickInstanceID(Joystick))
          {
            SDL_GameControllerClose(GameController);
            GameController = NULL;
          }
      }

      if (Event.type == SDL_QUIT)
        GameState = GSQuit;

      if (Event.type == SDL_CONTROLLERBUTTONUP)
        HandleJoystickEvent(Event.cbutton.button);

      if (Event.type == SDL_KEYUP)
        HandleKeyboardEvent(Event.key.keysym.sym);
    }

    if (ButFullscreen)
      ToggleFullscreen();

    if (ButNextSkin)
      NextSkin(TextureBuffer1);

    if (ButVolDown)
      DecVolume();
      
    if (ButVolUp)
      IncVolume();

    if (ButBack || ButB)
      GameState = GSTitleScreen;

    SDL_SetRenderTarget(Renderer, TextureBuffer1);
    World->Draw(Renderer);
    switch (Counter) {
    case 1:
      Dst.x = 46;
      Dst.y = 85;
      SDL_QueryTexture(IMGReady, NULL, NULL, &Dst.w, &Dst.h);

      SDL_RenderCopy(Renderer, IMGReady, NULL, &Dst);
      break;
    case 2:
      Dst.x = 93;
      Dst.y = 85;
      SDL_QueryTexture(IMGGo, NULL, NULL, &Dst.w, &Dst.h);

      SDL_RenderCopy(Renderer, IMGGo, NULL, &Dst);
      break;
    case 3:
      GameState = GSGame;
      break;
    }
    if (alpha < MaxAlpha) {
      alpha = trunc(MaxAlpha *
                    ((double)(SDL_GetTicks() - AlphaTimer) / MaxAlphaTime));

      if (alpha + AlphaIncrease >= MaxAlpha) {
        SDL_SetTextureBlendMode(TextureBuffer1, SDL_BLENDMODE_NONE);
        alpha = 255;
      }
      SDL_SetTextureAlphaMod(TextureBuffer1, alpha);
    }

    if (ShowFPS) {
      sprintf(Text, "FPS : %d", Fps);
      WriteText(BigFont, Text, strlen(Text), 247, 140, 0, TextColor);
    }

    sprintf(Text, "Timer:\n%2d:%02d\n\nScore:\n%d", Timer / 60, Timer % 60,
            Score);
    WriteText(BigFont, Text, strlen(Text), 247, 10, 0, TextColor);
    sprintf(Text, "QUIT");
    WriteText(BigFont, Text, strlen(Text), 247, 180, 0, TextColor);

    SDL_SetRenderTarget(Renderer, TextureBuffer2);
    SDL_RenderCopy(Renderer, TextureBuffer1, NULL, NULL);
    SDL_SetRenderTarget(Renderer, NULL);
    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
    SDL_RenderClear(Renderer);
    SDL_RenderCopy(Renderer, TextureBuffer2, NULL, NULL);
    SDL_RenderPresent(Renderer);

    FrameCount++;
    if (Time1 + 1000 < SDL_GetTicks()) {
      Fps = FrameCount;
      FrameCount = 0;
      Time1 = SDL_GetTicks();
    }

    if (Time2 < SDL_GetTicks()) {
      Counter++;
      if (Counter == 1)
        if (GlobalSoundEnabled)
          Mix_PlayChannel(-1, Sounds[SND_READYGO], 0);
      if (Counter == 2) {
        Time2 = SDL_GetTicks() + 400;
      } else
        Time2 = SDL_GetTicks() + 900;
    }
  }
}

void CGame::Intro() {
  SDL_Event Event;
  Uint8 alpha = 0;
  int IntroScreenNr = 1;
  Uint32 Time1 = SDL_GetTicks();
  Uint32 AlphaTimer = SDL_GetTicks();
  while (GameState == GSIntro) {
    while (SDL_PollEvent(&Event)) {
      if (Event.type == SDL_JOYDEVICEADDED)
        if(GameController == NULL)
          if(SDL_IsGameController(Event.jdevice.which))
            GameController = SDL_GameControllerOpen(Event.jdevice.which);

      if (Event.type == SDL_JOYDEVICEREMOVED)
      {
        SDL_Joystick* Joystick = SDL_GameControllerGetJoystick(GameController);
        if (Joystick)
          if (Event.jdevice.which == SDL_JoystickInstanceID(Joystick))
          {
            SDL_GameControllerClose(GameController);
            GameController = NULL;
          }
      }

      if ((Event.type == SDL_FINGERUP) || (Event.type == SDL_MOUSEBUTTONUP) || 
        (Event.type == SDL_CONTROLLERBUTTONUP))
        GameState = GSTitleScreen;
      if (Event.type == SDL_KEYUP)
        GameState = GSTitleScreen;
      if (Event.type == SDL_QUIT)
        GameState = GSQuit;
    }
    
    SDL_SetRenderTarget(Renderer, TextureBuffer1);
    switch (IntroScreenNr) {
    case 1:
      SDL_RenderCopy(Renderer, IMGIntro1, NULL, NULL);
      break;
    case 2:
      SDL_RenderCopy(Renderer, IMGIntro2, NULL, NULL);
      break;
    case 3:
      SDL_RenderCopy(Renderer, IMGIntro3, NULL, NULL);
      break;
    }

    if (alpha < MaxAlpha) {
      alpha = trunc(MaxAlpha *
                    ((double)(SDL_GetTicks() - AlphaTimer) / MaxAlphaTime));

      if (alpha + AlphaIncrease >= MaxAlpha) {
        alpha = 255;
      }
      SDL_SetTextureAlphaMod(TextureBuffer1, alpha);
    }
    SDL_SetRenderTarget(Renderer, TextureBuffer2);
    SDL_RenderCopy(Renderer, TextureBuffer1, NULL, NULL);
    SDL_SetRenderTarget(Renderer, NULL);
    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
    SDL_RenderClear(Renderer);
    SDL_RenderCopy(Renderer, TextureBuffer2, NULL, NULL);
    SDL_RenderPresent(Renderer);

    if (Time1 + 3700 < SDL_GetTicks()) {
      AlphaTimer = SDL_GetTicks();
      alpha = 0;
      IntroScreenNr++;
      if (IntroScreenNr > 3)
        GameState = GSTitleScreen;
      Time1 = SDL_GetTicks();
    }
  }
  if (GlobalSoundEnabled)
    Mix_PlayChannel(-1, Sounds[SND_WELCOME], 0);
}

void CGame::Game() {
  CSelector *Selector;
  SDL_Event Event;
  Uint32 Time1 = SDL_GetTicks();
  Uint32 ScoreTimer = 0;
  Selector = new CSelector(5, 5, &IMGCursor);
  SDL_Color TextColor = {255, 255, 255, 255};
  int AddToScore = 0, Timer = 150;
  char Text[500];
  if (GameType == Relative)
    Timer = 200;
  else
    Timer = 600;

  int FrameCount = 0, Fps = 0;
  Uint32 FpsTime = SDL_GetTicks();

  while (GameState == GSGame) {
    if (GlobalSoundEnabled) {
      if (!Mix_PlayingMusic()) {
        if (MusicCount > 1) {
          SelectedMusic = 1 + rand() % (MusicCount - 1);
          Mix_PlayMusic(Music[SelectedMusic], 0);
          SetVolume(Volume);
        } else if (MusicCount == 1) {
          SelectedMusic = 0;
          Mix_PlayMusic(Music[SelectedMusic], 0);
          SetVolume(Volume);
        }
      }
    }

    ResetButtons();
    while (SDL_PollEvent(&Event)) {
      if (Event.type == SDL_JOYDEVICEADDED)
        if(GameController == NULL)
          if(SDL_IsGameController(Event.jdevice.which))
            GameController = SDL_GameControllerOpen(Event.jdevice.which);

      if (Event.type == SDL_JOYDEVICEREMOVED)
      {
        SDL_Joystick* Joystick = SDL_GameControllerGetJoystick(GameController);
        if (Joystick)
          if (Event.jdevice.which == SDL_JoystickInstanceID(Joystick))
          {
            SDL_GameControllerClose(GameController);
            GameController = NULL;
          }
      }

      if (Event.type == SDL_CONTROLLERBUTTONUP)
        HandleJoystickEvent (Event.cbutton.button);

      if (Event.type == SDL_KEYUP)
       HandleKeyboardEvent(Event.key.keysym.sym);

      if (Event.type == SDL_QUIT)
        GameState = GSQuit;

      if ((Event.type == SDL_FINGERUP) || (Event.type == SDL_MOUSEBUTTONDOWN)) {
        int RealX;
        int RealY;
        if (Event.type == SDL_FINGERUP) {
          RealX = Event.tfinger.x * 320; //needs to be logical size
          RealY = Event.tfinger.y * 240; //needs to be logical size
        } else {
          RealX = Event.button.x;
          RealY = Event.button.y;
        }
        if (((RealX - 5) > 0) &&
            ((RealY - 5 > 0) && ((RealX - 5) < NrOfCols * TileWidth) &&
             ((RealY - 5)) < NrOfRows * TileHeight)) {
          Selector->SetPosition((((RealX - 5) / TileWidth)),
                                ((RealY - 5) / TileHeight));
          AddToScore += World->Select(Selector->GetPosition().X,
                                      Selector->GetPosition().Y);
          if (AddToScore != 0) {
            ScoreTimer = SDL_GetTicks() + 700;
          }
        } else if ((RealX > (247 * 1)) && (RealX < (290 * 1)) &&
                   (RealY > (180 * 1)) && (RealY < (200 * 1)))
          GameState = GSTitleScreen;
      }
    }

    if (ButFullscreen)
      ToggleFullscreen();

    if (ButBack || ButB)  
      GameState = GSTitleScreen;

    if (ButA || ButStart)  {
      AddToScore += World->Select(Selector->GetPosition().X,
        Selector->GetPosition().Y);
      if (AddToScore != 0) {
        ScoreTimer = SDL_GetTicks() + 700;
      }
    }

    if (ButLeft)
      Selector->SetPosition(Selector->GetPosition().X - 1,
        Selector->GetPosition().Y);

    if (ButRight)
      Selector->SetPosition(Selector->GetPosition().X + 1,
        Selector->GetPosition().Y);

    if (ButUp)
      Selector->SetPosition(Selector->GetPosition().X,
        Selector->GetPosition().Y - 1);

    if (ButDown)
      Selector->SetPosition(Selector->GetPosition().X,
        Selector->GetPosition().Y + 1);

    if (ButNextSkin)
      NextSkin(TextureBuffer1);

    if (ButNextMusic) {
      if (MusicCount > 1) {
        SelectedMusic++;
        if (SelectedMusic >= MusicCount)
          SelectedMusic = 1;
        if (GlobalSoundEnabled) {
          Mix_HaltMusic();
          Mix_PlayMusic(Music[SelectedMusic], 0);
          SetVolume(Volume);
        }
      }
    }

    if (ButVolUp)
      IncVolume();

    if (ButVolDown)
      DecVolume();

    SDL_SetRenderTarget(Renderer, TextureBuffer1);
    World->Draw(Renderer);
    Selector->Draw(Renderer);
    if (AddToScore != 0) {
      if (ScoreTimer > SDL_GetTicks()) {
        sprintf(Text, "+%d", AddToScore);
        WriteText(font, Text, strlen(Text), 247, 105, 0, TextColor);
        if (GameType == Relative) {
          sprintf(Text, "+%d", AddToScore / 200);
          WriteText(font, Text, strlen(Text), 247, 50, 0, TextColor);
        }
      } else {
        ScoreTimer = 0;
        Score += AddToScore;
        if (GameType == Relative)
          Timer += AddToScore / 200;
        AddToScore = 0;
      }
    }

    if (ShowFPS) {
      sprintf(Text, "FPS:\n%d", Fps);
      WriteText(BigFont, Text, strlen(Text), 247, 140, 0, TextColor);
    }

    sprintf(Text, "Timer:\n%2d:%02d\n\nScore:\n%d", Timer / 60, Timer % 60,
            Score);
    WriteText(BigFont, Text, strlen(Text), 247, 10, 0, TextColor);
    sprintf(Text, "QUIT");
    WriteText(BigFont, Text, strlen(Text), 247, 180, 0, TextColor);

    SDL_SetRenderTarget(Renderer, TextureBuffer2);
    SDL_RenderCopy(Renderer, TextureBuffer1, NULL, NULL);
    SDL_SetRenderTarget(Renderer, NULL);
    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
    SDL_RenderClear(Renderer);
    SDL_RenderCopy(Renderer, TextureBuffer2, NULL, NULL);
    SDL_RenderPresent(Renderer);

    if (Time1 + 1000 < SDL_GetTicks()) {
      Timer -= 1;
      switch (Timer) {
      case 300:
        if (GlobalSoundEnabled)
          Mix_PlayChannel(-1, Sounds[SND_5MINUTE], 0);
        break;
      case 60:
        if (GlobalSoundEnabled)
          Mix_PlayChannel(-1, Sounds[SND_1MINUTE], 0);
        break;
      case 3:
        if (GlobalSoundEnabled)
          Mix_PlayChannel(-1, Sounds[SND_3], 0);
        break;
      case 2:
        if (GlobalSoundEnabled)
          Mix_PlayChannel(-1, Sounds[SND_2], 0);
        break;
      case 1:
        if (GlobalSoundEnabled)
          Mix_PlayChannel(-1, Sounds[SND_1], 0);
        break;
      case 0:
        if (ScoreTimer != 0) {
          Score += AddToScore;
          if (GameType == Relative)
            Timer += AddToScore / 200;
        }
        if (Timer == 0) {
          if (GlobalSoundEnabled)
            Mix_PlayChannel(-1, Sounds[SND_TIMEOVER], 0);
          GameState = GSTimeOver;
        }
        break;
      }
      Time1 = SDL_GetTicks();
    }

    FrameCount++;
    if (FpsTime + 1000 < SDL_GetTicks()) {
      Fps = FrameCount;
      FrameCount = 0;
      FpsTime = SDL_GetTicks();
    }
  }

  delete Selector;
  if (GlobalSoundEnabled) {
    Mix_HaltMusic();
  }
}

void CGame::LoadHighScores() {
  FILE *FHighScores;
  char FileName[PATH_MAX + FILENAME_MAX];
#ifdef WIN32
  sprintf(FileName, "%s%s/.znaxhighscore", getenv("HOMEDRIVE"),
          getenv("HOMEPATH"));
#else
  sprintf(FileName, "%s/.znaxhighscore", getenv("HOME"));
#endif
  FHighScores = fopen(FileName, "rb");
  if (FHighScores) {
    fread(HighScores[Fixed], sizeof(HighScores[Fixed][0]), 10, FHighScores);
    fread(HighScores[Relative], sizeof(HighScores[Relative][0]), 10,
          FHighScores);
    fclose(FHighScores);
  } else {
    for (int Teller = 0; Teller < 10; Teller++) {
      sprintf(HighScores[Relative][Teller].PName, "%s", "joyrider");
      HighScores[Relative][Teller].PScore = 0;
      sprintf(HighScores[Fixed][Teller].PName, "%s", "joyrider");
      HighScores[Fixed][Teller].PScore = 0;
    }
  }
}

void CGame::SaveHighScores() {
  FILE *FHighScores;
  char FileName[PATH_MAX + FILENAME_MAX];
#ifdef WIN32
  sprintf(FileName, "%s%s/.znaxhighscore", getenv("HOMEDRIVE"),
          getenv("HOMEPATH"));
#else
  sprintf(FileName, "%s/.znaxhighscore", getenv("HOME"));
#endif
  FHighScores = fopen(FileName, "wb");
  if (FHighScores) {
    fwrite(HighScores[Fixed], sizeof(HighScores[Fixed][0]), 10, FHighScores);
    fwrite(HighScores[Relative], sizeof(HighScores[Relative][0]), 10,
           FHighScores);
    fclose(FHighScores);
  }
}

void CGame::ShowHighScores() {
  SDL_Event Event;
  int Teller = 0;
  char Text[250];
  int ScoreType = GameType;
  Uint8 alpha = 0;
  SDL_Color TextColor = {102, 115, 152, 255};
  SDL_SetTextureBlendMode(TextureBuffer1, SDL_BLENDMODE_BLEND);
  Uint32 AlphaTimer = SDL_GetTicks();
  while (GameState == GSShowHighScores) {
    if (MusicCount > 0)
      if (GlobalSoundEnabled)
        if (!Mix_PlayingMusic()) {
          SelectedMusic = 0;
          Mix_PlayMusic(Music[SelectedMusic], 0);
          SetVolume(Volume);
        }

    ResetButtons();
    while (SDL_PollEvent(&Event)) {
      if (Event.type == SDL_JOYDEVICEADDED)
        if(GameController == NULL)
          if(SDL_IsGameController(Event.jdevice.which))
            GameController = SDL_GameControllerOpen(Event.jdevice.which);

      if (Event.type == SDL_JOYDEVICEREMOVED)
      {
        SDL_Joystick* Joystick = SDL_GameControllerGetJoystick(GameController);
        if (Joystick)
          if (Event.jdevice.which == SDL_JoystickInstanceID(Joystick))
          {
            SDL_GameControllerClose(GameController);
            GameController = NULL;
          }
      }

      if ((Event.type == SDL_FINGERUP) || (Event.type == SDL_MOUSEBUTTONUP)) {
        if (ScoreType == Fixed)
          ScoreType = Relative;
        else
          GameState = GSTitleScreen;
      }

      if (Event.type == SDL_QUIT)
        GameState = GSQuit;

      if (Event.type == SDL_CONTROLLERBUTTONUP)
        HandleJoystickEvent(Event.cbutton.button);

      if (Event.type == SDL_KEYUP)
        HandleKeyboardEvent(Event.key.keysym.sym);
    }

    if (ButFullscreen)
      ToggleFullscreen();

    if (ButNextSkin)
      NextSkin(TextureBuffer1);

    if (ButVolDown)
      DecVolume();

    if (ButVolUp)
      IncVolume();

    if (ButStart || ButA) {
      if (ScoreType == Fixed)
        ScoreType = Relative;
      else
        GameState = GSTitleScreen;
    }

    if (ButBack || ButB)
      GameState = GSTitleScreen;

    SDL_SetRenderTarget(Renderer, TextureBuffer1);
    SDL_RenderCopy(Renderer, IMGHighScores, NULL, NULL);

    switch (ScoreType) {
    case Fixed:
      sprintf(Text, "Gametype: Fixed Timer");
      break;
    case Relative:
      sprintf(Text, "Gametype: Relative Timer");
      break;
    }

    WriteText(font, Text, strlen(Text), 105, 227, 0, TextColor);
    for (Teller = 0; Teller < 10; Teller++) {
      sprintf(Text, "%2d.%s", Teller + 1, HighScores[ScoreType][Teller].PName);
      WriteText(MonoFont, Text, strlen(Text), 50, (62 + Teller * 16), 0,
                TextColor);
      sprintf(Text, "%7d", HighScores[ScoreType][Teller].PScore);
      WriteText(MonoFont, Text, strlen(Text), 220, (62 + Teller * 16), 0,
                TextColor);
    }

    if (alpha < MaxAlpha) {
      alpha = trunc(MaxAlpha *
                    ((double)(SDL_GetTicks() - AlphaTimer) / MaxAlphaTime));

      if (alpha + AlphaIncrease >= MaxAlpha) {
        SDL_SetTextureBlendMode(TextureBuffer1, SDL_BLENDMODE_NONE);
        alpha = 255;
      }
      SDL_SetTextureAlphaMod(TextureBuffer1, alpha);
    }

    SDL_SetRenderTarget(Renderer, TextureBuffer2);
    SDL_RenderCopy(Renderer, TextureBuffer1, NULL, NULL);
    SDL_SetRenderTarget(Renderer, NULL);
    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
    SDL_RenderClear(Renderer);
    SDL_RenderCopy(Renderer, TextureBuffer2, NULL, NULL);
    SDL_RenderPresent(Renderer);
  }
}

void CGame::LoadSounds() {
  if (GlobalSoundEnabled) {
    char FileName[PATH_MAX + FILENAME_MAX];
    sprintf(FileName, "%ssound/readygo.wav", DataPath);
    Sounds[SND_READYGO] = Mix_LoadWAV(FileName);
    sprintf(FileName, "%ssound/one.wav", DataPath);
    Sounds[SND_1] = Mix_LoadWAV(FileName);
    sprintf(FileName, "%ssound/two.wav", DataPath);
    Sounds[SND_2] = Mix_LoadWAV(FileName);
    sprintf(FileName, "%ssound/three.wav", DataPath);
    Sounds[SND_3] = Mix_LoadWAV(FileName);
    sprintf(FileName, "%ssound/oneminute.wav", DataPath);
    Sounds[SND_1MINUTE] = Mix_LoadWAV(FileName);
    sprintf(FileName, "%ssound/fiveminute.wav", DataPath);
    Sounds[SND_5MINUTE] = Mix_LoadWAV(FileName);
    sprintf(FileName, "%ssound/timeover.wav", DataPath);
    Sounds[SND_TIMEOVER] = Mix_LoadWAV(FileName);
    sprintf(FileName, "%ssound/menu.wav", DataPath);
    Sounds[SND_MENU] = Mix_LoadWAV(FileName);
    sprintf(FileName, "%ssound/select.wav", DataPath);
    Sounds[SND_SELECT] = Mix_LoadWAV(FileName);
    sprintf(FileName, "%ssound/welcome.wav", DataPath);
    Sounds[SND_WELCOME] = Mix_LoadWAV(FileName);
    sprintf(FileName, "%ssound/goodbye.wav", DataPath);
    Sounds[SND_GOODBYE] = Mix_LoadWAV(FileName);
    sprintf(FileName, "%ssound/delete.wav", DataPath);
    Sounds[SND_DELETE] = Mix_LoadWAV(FileName);
    sprintf(FileName, "%ssound/blockselect.wav", DataPath);
    Sounds[SND_BLOCKSELECT] = Mix_LoadWAV(FileName);
    sprintf(FileName, "%ssound/error.wav", DataPath);
    Sounds[SND_ERROR] = Mix_LoadWAV(FileName);
  }
}

void CGame::UnloadSounds() {
  if (GlobalSoundEnabled) {
    if (!Mix_PlayingMusic())
      Mix_HaltMusic();

    int Teller;
    for (Teller = 0; Teller < NrOfSounds; Teller++)
      if (Sounds[Teller])
        Mix_FreeChunk(Sounds[Teller]);
  }
}

void CGame::GetFilePath(char *InputFile, char *result) {
  int Teller, Pos = 0;
  for (Teller = strlen(InputFile) - 1; Teller >= 0; Teller--)
    if ((InputFile[Teller] == '/') || (InputFile[Teller] == '\\')) {
      Pos = Teller + 1;
      break;
    }
  memcpy(result, InputFile, Pos);
}

static int SDLCALL my_event_filter(void *userdata, SDL_Event *event)
{
    if ((event->type == SDL_MOUSEBUTTONUP) || (event->type == SDL_MOUSEBUTTONDOWN)) {
      return (((SDL_MouseButtonEvent *)event)->which != SDL_TOUCH_MOUSEID);
    }
    return 1;
}

void CGame::Run(int argc, char *argv[]) {

  bool useSoftwareRenderer = false;
  bool useLinear = false;
  bool useVsync = true;
  bool useFullScreenAtStartup = false;
  char StartPath[PATH_MAX] = {'\0'};
  GetFilePath(argv[0], StartPath);
  sprintf(DataPath, "%sznaxfs/", StartPath);
  int c;
  while ((c = getopt(argc, argv, "?slfdr")) != -1) {
    switch (c) {
    case '?':
      // i use sdl log because on windows printf did not show up
      SDL_Log("\nZnax Version 1.0.2\n\n\
Usage: Znax [Options]\n\n\
Possible options are:\n\
  -?: show this help message\n\
  -l: enable linear filtering (only works when hardware renderer is used)\n\
  -s: Use Software Renderer\n\
  -f: Show FPS\n\
  -d: Disable VSync (use this in combination with software rendering if it fails without it)\n\
  -r: Run fullscreen at startup (by default starts up as a sizeable window)\n");
      exit(0);
      break;
    case 'l':
      useLinear = true;
      break;
    case 's':
      useSoftwareRenderer = true;
      break;
    case 'f':
      ShowFPS = true;
      break;
    case 'd':
      useVsync = false;
      break;
    case 'r':
      useFullScreenAtStartup = true;
      break;
    }
  }

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) == 0) {
    Uint32 WindowFlags = SDL_WINDOW_RESIZABLE;
    if (useFullScreenAtStartup) {
      WindowFlags |= SDL_WINDOW_FULLSCREEN;
    }
    SdlWindow =
        SDL_CreateWindow("Znax", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         320 * SCALEFACTOR, 240 * SCALEFACTOR, WindowFlags);
    if (SdlWindow) {
      Uint32 flags = 0;
      if (useSoftwareRenderer) {
        flags |= SDL_RENDERER_SOFTWARE;
      } else {
        flags |= SDL_RENDERER_ACCELERATED;
      }

      if (useVsync) {
        flags |= SDL_RENDERER_PRESENTVSYNC;
      }
      SDL_Log("Succesfully Set %dx%d\n", 320 * SCALEFACTOR, 240 * SCALEFACTOR);
      Renderer = SDL_CreateRenderer(SdlWindow, -1, flags);
      if (Renderer) {
        SDL_RendererInfo rendererInfo;
        SDL_GetRendererInfo(Renderer, &rendererInfo);
        SDL_Log("Using Renderer:%s\n", rendererInfo.name);

        if (useLinear) {
          SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
        }

        SDL_RenderSetLogicalSize(Renderer, 320, 240);
        SDL_Log("Succesfully Created Buffer\n");
        SDL_ShowCursor(SDL_ENABLE);
        if (SDL_Init(SDL_INIT_AUDIO) == 0) {

          if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2,
                            1024) < 0) {
            GlobalSoundEnabled = false;
            SDL_Log("Failed to initialise sound: %s\n", Mix_GetError());
          } else {
            SDL_Log("Audio Succesfully initialised!\n");
          }
        } else {
          GlobalSoundEnabled = false;
          SDL_Log("Failed to initialise sound: %s\n", SDL_GetError());
        }
        if (TTF_Init() == 0) {
          char Filename[PATH_MAX + FILENAME_MAX] = {'\0'};
          SDL_Log("Succesfully initialized TTF\n");
          sprintf(Filename, "%sdata/Roboto-Regular.ttf", DataPath);
          font = TTF_OpenFont(Filename, 11);
          BigFont = TTF_OpenFont(Filename, 16);
          sprintf(Filename, "%sdata/RobotoMono-Bold.ttf", DataPath);
          MonoFont = TTF_OpenFont(Filename, 12);
          if (font && BigFont && MonoFont) {            
            SkinName[0] = '\0';
            srand(time(NULL));
            SDL_Log("Succesfully Loaded fonts\n");
            TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
            SDL_SetEventFilter(my_event_filter, NULL);
            //Check for joysticks
            for (int i=0; i < SDL_NumJoysticks(); i++)
            {
              if(SDL_IsGameController(i))
              {
                GameController = SDL_GameControllerOpen(i);
                SDL_GameControllerEventState(SDL_ENABLE);
                SDL_Log("Joystick Detected!\n");
                break;
              }
            }
            // Main game loop that loops untile the gamestate = GSQuit
            // and calls the procedure according to the gamestate.
            SearchForMusic();
            SearchForSkins();
            LoadSettings();
            LoadGraphics();
            LoadSounds();
            LoadHighScores();
            World =
                new CWorldParts(&IMGBackground, &IMGBlocks, &Sounds[SND_DELETE],
                                &Sounds[SND_ERROR], &Sounds[SND_BLOCKSELECT],
                                &GlobalSoundEnabled);
            SDL_SetRenderTarget(Renderer, NULL);
            SDL_SetRenderDrawColor(Renderer, 0x00, 0x00, 0x00, 0xFF);
            SDL_RenderClear(Renderer);
            TextureBuffer1 =
                SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_ARGB8888,
                                  SDL_TEXTUREACCESS_TARGET, 320, 240);
            SDL_SetRenderTarget(Renderer, TextureBuffer1);
            SDL_SetRenderDrawColor(Renderer, 0x00, 0x00, 0x00, 0xFF);
            SDL_RenderClear(Renderer);
            TextureBuffer2 =
                SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_ARGB8888,
                                  SDL_TEXTUREACCESS_TARGET, 320, 240);
            SDL_SetRenderTarget(Renderer, TextureBuffer2);
            SDL_SetRenderDrawColor(Renderer, 0x00, 0x00, 0x00, 0xFF);
            SDL_RenderClear(Renderer);
            SDL_SetTextureBlendMode(TextureBuffer1, SDL_BLENDMODE_BLEND);
            while (GameState != GSQuit) {
              switch (GameState) {
              case GSGame:
                Game();
                break;
              case GSTitleScreen:
                TitleScreen();
                break;
              case GSIntro:
                Intro();
                break;
              case GSReadyGo:
                ReadyGo();
                break;
              case GSTimeOver:
                TimeOver();
                break;
              case GSCredits:
                Credits();
                break;
              case GSGameTypeMenu:
                GameTypeMenu();
                break;
              case GSShowHighScores:
                ShowHighScores();
                break;
              default:
                break;
              }
            }
            SDL_DestroyTexture(TextureBuffer1);
            SDL_DestroyTexture(TextureBuffer2);
            delete World;
            SaveHighScores();
            UnloadSounds();
            UnloadMusic();
            UnLoadGraphics();
            SaveSettings();
            if (GlobalSoundEnabled)
              Mix_CloseAudio();
            TTF_CloseFont(font);
            TTF_CloseFont(MonoFont);
            TTF_CloseFont(BigFont);
            font = NULL;
            if(GameController) {
              SDL_GameControllerClose(GameController);
              GameController = NULL;
            }
          } else {
            SDL_Log("Failed loading fonts: %s\n", SDL_GetError());
          }
          TTF_Quit();
        } else {
          SDL_Log("Failed initializing SDL2_ttf %s\n", SDL_GetError());
        }
        SDL_DestroyRenderer(Renderer);
      } else {
        SDL_Log("Failed to create renderer: %s\n", SDL_GetError());
      }
    } else {
      SDL_Log("Failed to Set Videomode (%dx%d): %s\n", 320 * SCALEFACTOR,
              240 * SCALEFACTOR, SDL_GetError());
    }
    SDL_DestroyWindow(SdlWindow);
    SDL_Quit();
  } else {
    SDL_Log("Failed to initialise SDL: %s\n", SDL_GetError());
  }
}

void CGame::WriteText(TTF_Font *FontIn, char *Tekst, int NrOfChars, int X,
                      int Y, int YSpacing, SDL_Color ColorIn) {
  char List[100][255];
  int Lines, Teller, Chars;
  SDL_Rect DstRect;
  SDL_Surface *TextureBuffer1Surface1;
  SDL_Texture *TextureBuffer1Texture;
  Lines = 0;
  Chars = 0;
  for (Teller = 0; Teller < NrOfChars; Teller++) {
    if (Lines > 100)
      break;
    if ((Tekst[Teller] == '\n') || (Chars == 255)) {
      List[Lines][Chars] = '\0';
      Lines++;
      Chars = 0;
    } else {
      List[Lines][Chars] = Tekst[Teller];
      Chars++;
    }
  }
  List[Lines][Chars] = '\0';
  for (Teller = 0; Teller <= Lines; Teller++) {
    if (strlen(List[Teller]) > 0) {
      TextureBuffer1Surface1 =
          TTF_RenderText_Blended(FontIn, List[Teller], ColorIn);
      if (TextureBuffer1Surface1) {
        TextureBuffer1Texture =
            SDL_CreateTextureFromSurface(Renderer, TextureBuffer1Surface1);
        DstRect.x = X;
        DstRect.y = Y + (Teller)*TTF_FontLineSkip(FontIn) + (Teller * YSpacing);
        DstRect.w = TextureBuffer1Surface1->w;
        DstRect.h = TextureBuffer1Surface1->h;

        SDL_RenderCopy(Renderer, TextureBuffer1Texture, NULL, &DstRect);
        SDL_DestroyTexture(TextureBuffer1Texture);
        SDL_FreeSurface(TextureBuffer1Surface1);
      }
    }
  }
}
