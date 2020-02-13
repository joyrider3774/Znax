PROG=znax
VERS=1.0.2
CC=g++

OPT_LEVEL=-O2 
CFLAGS=-Wall -Wextra
CPPFLAGS=-I/usr/include -I/usr/include/SDL2
LDFLAGS=-L/usr/lib
LDLIBS=-lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2 -lSDL2_gfx -lX11
RM=/bin/rm -f
OBJS=znax.o CGame.o CSelector.o CMainMenu.o CGameTypeMenu.o CBlock.o CWorldParts.o

ifeq ($(PREFIX),)
    PREFIX=$(DESTDIR)/usr
endif
GAMEDIR=$(PREFIX)/games/znax
DATADIR=$(GAMEDIR)/data
GRAPHICSDIR=$(GAMEDIR)/graphics
MUSICDIR=$(GAMEDIR)/music
SOUNDDIR=$(GAMEDIR)/sound
SKINSDIR=$(GAMEDIR)/skins
REDSKINDIR=$(SKINSDIR)/red
NUMBERSKINDIR=$(SKINSDIR)/numbers
NEWBLOCKSSKINDIR=$(SKINSDIR)/newblocks
ICONDIR=$(GAMEDIR)/icon
DESKTOPDIR=$(PREFIX)/share/applications


.PHONY: clean distclean
all: $(PROG)
%.o: %.cpp
		$(CC) $(CFLAGS) $(OPT_LEVEL) $(CPPFLAGS) -c $< -o $@  
$(PROG): $(OBJS)
		$(CC) $(LDFLAGS) $(TARGET_ARCH) $^ -o $@ $(LDLIBS)
clean:
		$(RM) *~ $(OBJS) $(PROG)
.PHONY:  clean

install: all
	install -d $(GAMEDIR)
	install -d $(DATADIR)
	install -d $(GRAPHICSDIR)
	install -d $(MUSICDIR)
	install -d $(SOUNDDIR)
	install -d $(SKINSDIR)
	install -d $(DESKTOPDIR)
	install -d $(REDSKINDIR)
	install -d $(NUMBERSKINDIR)
	install -d $(NEWBLOCKSSKINDIR)
	install -d $(ICONDIR)

	install -m 0755 znax $(GAMEDIR)
	install -m 0644 znaxfs/data/RobotoMono-Bold.ttf $(DATADIR)
	install -m 0644 znaxfs/data/Roboto-Regular.ttf $(DATADIR)
	install -m 0644 znaxfs/data/readme.txt $(DATADIR)
	install -m 0644 znaxfs/data/znax.desktop $(DESKTOPDIR) 
	install -m 0644 znaxfs/data/znax.png $(ICONDIR)
	
	install -m 0644 znaxfs/graphics/background.png $(GRAPHICSDIR)
	install -m 0644 znaxfs/graphics/blocks.png $(GRAPHICSDIR)
	install -m 0644 znaxfs/graphics/credits.png $(GRAPHICSDIR)
	install -m 0644 znaxfs/graphics/credits1.png $(GRAPHICSDIR)
	install -m 0644 znaxfs/graphics/credits2.png $(GRAPHICSDIR)
	install -m 0644 znaxfs/graphics/cursor.png $(GRAPHICSDIR)
	install -m 0644 znaxfs/graphics/fixedtimer1.png $(GRAPHICSDIR)
	install -m 0644 znaxfs/graphics/fixedtimer2.png $(GRAPHICSDIR)
	install -m 0644 znaxfs/graphics/go.png $(GRAPHICSDIR)
	install -m 0644 znaxfs/graphics/highscores.png $(GRAPHICSDIR)
	install -m 0644 znaxfs/graphics/highscores1.png $(GRAPHICSDIR)
	install -m 0644 znaxfs/graphics/highscores2.png $(GRAPHICSDIR)
	install -m 0644 znaxfs/graphics/intro1.png $(GRAPHICSDIR)
	install -m 0644 znaxfs/graphics/intro2.png $(GRAPHICSDIR)
	install -m 0644 znaxfs/graphics/intro3.png $(GRAPHICSDIR)
	install -m 0644 znaxfs/graphics/play1.png $(GRAPHICSDIR)
	install -m 0644 znaxfs/graphics/play2.png $(GRAPHICSDIR)
	install -m 0644 znaxfs/graphics/quit1.png $(GRAPHICSDIR)
	install -m 0644 znaxfs/graphics/quit2.png $(GRAPHICSDIR)
	install -m 0644 znaxfs/graphics/ready.png $(GRAPHICSDIR)
	install -m 0644 znaxfs/graphics/relativetimer1.png $(GRAPHICSDIR)
	install -m 0644 znaxfs/graphics/relativetimer2.png $(GRAPHICSDIR)
	install -m 0644 znaxfs/graphics/selectgame.png $(GRAPHICSDIR)
	install -m 0644 znaxfs/graphics/timeover.png $(GRAPHICSDIR)
	install -m 0644 znaxfs/graphics/titlescreen.png $(GRAPHICSDIR)
	
	install -m 0644 znaxfs/skins/newblocks/background.png $(NEWBLOCKSSKINDIR)
	install -m 0644 znaxfs/skins/newblocks/blocks.png $(NEWBLOCKSSKINDIR)
	
	install -m 0644 znaxfs/skins/numbers/background.png $(NUMBERSKINDIR)
	install -m 0644 znaxfs/skins/numbers/blocks.png $(NUMBERSKINDIR)
	install -m 0644 znaxfs/skins/numbers/credits.png $(NUMBERSKINDIR)
	install -m 0644 znaxfs/skins/numbers/credits1.png $(NUMBERSKINDIR)
	install -m 0644 znaxfs/skins/numbers/credits2.png $(NUMBERSKINDIR)
	install -m 0644 znaxfs/skins/numbers/fixedtimer1.png $(NUMBERSKINDIR)
	install -m 0644 znaxfs/skins/numbers/fixedtimer2.png  $(NUMBERSKINDIR)
	install -m 0644 znaxfs/skins/numbers/go.png $(NUMBERSKINDIR)
	install -m 0644 znaxfs/skins/numbers/highscores.png $(NUMBERSKINDIR)
	install -m 0644 znaxfs/skins/numbers/highscores1.png $(NUMBERSKINDIR)
	install -m 0644 znaxfs/skins/numbers/highscores2.png $(NUMBERSKINDIR)
	install -m 0644 znaxfs/skins/numbers/intro1.png $(NUMBERSKINDIR)
	install -m 0644 znaxfs/skins/numbers/intro2.png $(NUMBERSKINDIR)
	install -m 0644 znaxfs/skins/numbers/intro3.png $(NUMBERSKINDIR)
	install -m 0644 znaxfs/skins/numbers/play1.png $(NUMBERSKINDIR)
	install -m 0644 znaxfs/skins/numbers/play2.png $(NUMBERSKINDIR)
	install -m 0644 znaxfs/skins/numbers/quit1.png $(NUMBERSKINDIR)
	install -m 0644 znaxfs/skins/numbers/quit2.png $(NUMBERSKINDIR)
	install -m 0644 znaxfs/skins/numbers/ready.png $(NUMBERSKINDIR)
	install -m 0644 znaxfs/skins/numbers/relativetimer1.png $(NUMBERSKINDIR)
	install -m 0644 znaxfs/skins/numbers/relativetimer2.png $(NUMBERSKINDIR)
	install -m 0644 znaxfs/skins/numbers/selectgame.png $(NUMBERSKINDIR)
	install -m 0644 znaxfs/skins/numbers/timeover.png $(NUMBERSKINDIR)
	install -m 0644 znaxfs/skins/numbers/titlescreen.png $(NUMBERSKINDIR)
	
	install -m 0644 znaxfs/skins/red/background.png $(REDSKINDIR)
	install -m 0644 znaxfs/skins/red/blocks.png $(REDSKINDIR)
	install -m 0644 znaxfs/skins/red/credits.png $(REDSKINDIR)
	install -m 0644 znaxfs/skins/red/credits1.png $(REDSKINDIR)
	install -m 0644 znaxfs/skins/red/credits2.png $(REDSKINDIR)
	install -m 0644 znaxfs/skins/red/cursor.png $(REDSKINDIR)
	install -m 0644 znaxfs/skins/red/fixedtimer1.png $(REDSKINDIR)
	install -m 0644 znaxfs/skins/red/fixedtimer2.png  $(REDSKINDIR)
	install -m 0644 znaxfs/skins/red/go.png $(REDSKINDIR)
	install -m 0644 znaxfs/skins/red/highscores.png $(REDSKINDIR)
	install -m 0644 znaxfs/skins/red/highscores1.png $(REDSKINDIR)
	install -m 0644 znaxfs/skins/red/highscores2.png $(REDSKINDIR)
	install -m 0644 znaxfs/skins/red/intro1.png $(REDSKINDIR)
	install -m 0644 znaxfs/skins/red/intro2.png $(REDSKINDIR)
	install -m 0644 znaxfs/skins/red/intro3.png $(REDSKINDIR)
	install -m 0644 znaxfs/skins/red/play1.png $(REDSKINDIR)
	install -m 0644 znaxfs/skins/red/play2.png $(REDSKINDIR)
	install -m 0644 znaxfs/skins/red/quit1.png $(REDSKINDIR)
	install -m 0644 znaxfs/skins/red/quit2.png $(REDSKINDIR)
	install -m 0644 znaxfs/skins/red/ready.png $(REDSKINDIR)
	install -m 0644 znaxfs/skins/red/relativetimer1.png $(REDSKINDIR)
	install -m 0644 znaxfs/skins/red/relativetimer2.png $(REDSKINDIR)
	install -m 0644 znaxfs/skins/red/selectgame.png $(REDSKINDIR)
	install -m 0644 znaxfs/skins/red/timeover.png $(REDSKINDIR)
	install -m 0644 znaxfs/skins/red/titlescreen.png $(REDSKINDIR)
	
	install -m 0644 znaxfs/music/title.mod $(MUSICDIR)
	
	install -m 0644 znaxfs/sound/blockselect.wav $(SOUNDDIR)
	install -m 0644 znaxfs/sound/delete.wav $(SOUNDDIR)
	install -m 0644 znaxfs/sound/error.wav $(SOUNDDIR)
	install -m 0644 znaxfs/sound/fiveminute.wav $(SOUNDDIR)
	install -m 0644 znaxfs/sound/goodbye.wav $(SOUNDDIR)
	install -m 0644 znaxfs/sound/menu.wav $(SOUNDDIR)
	install -m 0644 znaxfs/sound/one.wav $(SOUNDDIR)
	install -m 0644 znaxfs/sound/oneminute.wav $(SOUNDDIR)
	install -m 0644 znaxfs/sound/readygo.wav $(SOUNDDIR)
	install -m 0644 znaxfs/sound/select.wav $(SOUNDDIR)
	install -m 0644 znaxfs/sound/three.wav $(SOUNDDIR)
	install -m 0644 znaxfs/sound/timeover.wav $(SOUNDDIR)
	install -m 0644 znaxfs/sound/two.wav $(SOUNDDIR)
	install -m 0644 znaxfs/sound/welcome.wav $(SOUNDDIR)

uninstall:
	rm -rf $(GAMEDIR)
	rm -f $(DESKTOPDIR)/znax.desktop
 
