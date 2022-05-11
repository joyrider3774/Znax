SRC_DIR=src
OBJ_DIR=obj
DAT_DIR=znaxfs
BIN_DIR=.

EXE=$(BIN_DIR)/znax
SRC=$(wildcard $(SRC_DIR)/*.cpp)
OBJS=$(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CC=g++
OPT_LEVEL=-O2 
CFLAGS=-Wall -Wextra -std=c++11
CPPFLAGS=-I/usr/include -I/usr/include/SDL2
LDFLAGS=-L/usr/lib
LDLIBS=-lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2 -lSDL2_gfx -lX11

ifeq ($(PREFIX),)
    PREFIX=$(DESTDIR)/usr
endif
GAMEDIR=$(PREFIX)/games/znax
DESKTOPDIR=$(PREFIX)/share/applications
ICONDIR=$(PREFIX)/share/icons/hicolor/scalable/apps


.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(LDFLAGS) $(TARGET_ARCH) $^ $(LDLIBS) -o $@ 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	$(RM) -rv *~ $(OBJS) $(EXE)

install: all
	install -d $(GAMEDIR)
	install -d $(DESKTOPDIR)
	install -d $(ICONDIR)

	install -m 0755 $(EXE) $(GAMEDIR)
	install -m 0644 $(DAT_DIR)/data/znax.desktop $(DESKTOPDIR) 
	install -m 0644 $(DAT_DIR)/data/znax.svg $(ICONDIR)
	\cp -rf $(DAT_DIR) $(GAMEDIR)

uninstall:
	$(RM) -rf $(GAMEDIR)
	$(RM) -f $(DESKTOPDIR)/znax.desktop
	$(RM) -f $(ICONFILE)
