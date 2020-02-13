# Znax
Znax is a cross platform puzzle / arcade game using SDL2 libraries.
It has skin support and can be played using a keyboard, mouse or touchscreen

![Znax screenshot2](/images/znaxscreenshot2.png) ![Znax screenshot1](/images/znaxscreenshot1.png) ![Znax screenshot3](/images/znaxscreenshot3.png)

## Building
Znax can be build on linux, i tried on raspberry pi and linux for windows subsystem using a debian distro. It can also be build from mingw32,
a specific makefile for this is included.

### dependencies
Znax uses SDL2 libaries, it needs libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev libsdl2-gfx-dev and your system would need to
be setup to compile programs.

you would generally get these by doing the following on a linux system

```
sudo apt-get update
sudo apt-get install build-essential libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev libsdl2-gfx-dev libx11-dev
```

### making the binary
once you have the source of the game and you have installed the required dependencies you start to build the program. 
a makefile is included.

you would execute the following command in the main source directory
```
make 
```

## installing
once the binary has been build you can install the game. I have provided a desktop shortcut and the game will install it. I have only tried this part
on raspbian buster, but by default the game would install all files to `/usr/games/znax`.

to install you would execute the following command after the binary has been build
```
sudo make install
```

if all went well, a new desktop shortcut in the games menu for znax should be visible at least on the raspberry pi, i have no idea about other systems.
if not desktop icon is visible you can still run the game by starting a terminal window and executing the following command

```
/usr/games/znax/znax
```


if you don't want to install the game but still play it you can copy the build `znax` binary to the `znaxfs` directory and you can launch the game from there 

```
cp znax ./znaxfs
cd znaxfs
znax
```

## What is Znax

Znax is a remake of a game by Nick Kouvaris.
It is a sort of puzzle / arcade game where you as the player need to select 
4 blocks of the same color and form rectangles as big as you can. By doing 
this you will erase all blocks in this square and they will be replaced by 
new blocks. You keep on doing this till the time runs out, and try to gain 
your highest score possible. There are two game modes, Relative timer and 
fixed Timer, in the first mode you'll also gain extra time for deleting 
blocks so you can play longer if you are fast enough. With the second mode 
you don't get extra time for deleting blocks but just points added to your 
score so here you try to get the highest amount of points in the given time 
period. 

## How To Play 
The game is fully playable using the mouse and keyboard: 

### general

arrow keys: 
Move in menu's, Move the cursor around while playing. 

enter / mouse button: 
Select a menu option, Select one block. 

Key L: 
Load the following skin. 

Escape: 
Quit to menu, Quit game 

Key pageup: 
Decrease Volume 

Key pagedown: 
Increase Volume 

### Entering a highscore name 

arrow up & down: 
Cycle through the letters. 

arrow Right: 
Move to the following letter. 

arrow left: 
Move to the previous lettter. 

To erase a letter you have to choose the whitespace. 

## Credits
Original (flash) Game Created by Nick Kouvaris 
http://lightforce.freestuff.gr

Original Gp2x Game and SDL2 version Created by Willems Davy
Willems Soft 2020
[https://Http://www.willemssoft.be](Http://www.willemssoft.be)

Music is made by Donskeeto! 
Thanks man

Game graphics created in paint shop pro 7

Game audio are some parts recorded by myselve and modified with goldwave studio,
some parts are made using [SFXR](http://www.drpetter.se/project_sfxr.html) 

Game uses
 SDL2: [https://www.libsdl.org](https://www.libsdl.org)
 SDL2_image: [https://www.libsdl.org/projects/SDL_image](https://www.libsdl.org/projects/SDL_image)
 SDL2_mixer: [https://www.libsdl.org/projects/SDL_mixer](https://www.libsdl.org/projects/SDL_mixer)
 SDL2_ttf: [https://www.libsdl.org/projects/SDL_ttf](https://www.libsdl.org/projects/SDL_ttf)
 SDL2_gfx: [https://sourceforge.net/projects/sdl2gfx](https://sourceforge.net/projects/sdl2gfx)
 Roboto font family by google: [https://fonts.google.com](https://fonts.google.com)

