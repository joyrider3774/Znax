SRC_DIR = src
OBJ_DIR = obj
DAT_DIR = znaxfs
EXE=znax

SRC=$(wildcard $(SRC_DIR)/*.cpp)
OBJS=$(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CC ?= g++
PREFIX ?= /usr
OPT_LEVEL ?= -O2 
CPPFLAGS ?= -Wall -Wextra -std=c++11 `sdl2-config --cflags` -I/usr/include -I/usr/include/SDL2  
LDFLAGS ?= -L$(PREFIX)/lib
LDLIBS ?= `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2 -lSDL2_gfx -lm -lstdc++

#MINGW does not have X11 and does not require it
#dont know about cygwin
ifneq ($(OS),Windows_NT)
LDLIBS += -lX11
endif

GAMEDIR = $(PREFIX)/games/znax
DESKTOPDIR = $(PREFIX)/share/applications
ICONDIR = $(PREFIX)/share/icons/hicolor/scalable/apps


.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(LDFLAGS) $(TARGET_ARCH) $^ $(LDLIBS) -o $@ 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	$(RM) -rv *~ $(OBJS) $(EXE)

install: all
	install -d $(GAMEDIR)
	install -d $(DESKTOPDIR)
	install -d $(ICONDIR)

	install -m 0755 $(EXE) $(GAMEDIR)
	sed -i '6c\Exec=$(GAMEDIR)/$(EXE)' $(DAT_DIR)/data/io.github.joyrider3774.znax.desktop
	install -m 0644 $(DAT_DIR)/data/io.github.joyrider3774.znax.desktop $(DESKTOPDIR) 
	install -m 0644 $(DAT_DIR)/data/io.github.joyrider3774.znax.svg $(ICONDIR)
	\cp -rf $(DAT_DIR) $(GAMEDIR)

uninstall:
	$(RM) -rf $(GAMEDIR)
	$(RM) -f $(DESKTOPDIR)/io.github.joyrider3774.znax.desktop
	$(RM) -f $(ICONDIR)/io.github.joyrider3774.znax.svg