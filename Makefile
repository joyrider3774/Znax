SRC_DIR = src
OBJ_DIR = obj
DAT_DIR = znaxfs
EXE=znax

SRC=$(wildcard $(SRC_DIR)/*.cpp)
OBJS=$(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CXX ?= g++
SDL2CONFIG = sdl2-config
DEFINES ?=
DESTDIR ?=
PREFIX ?= /usr
OPT_LEVEL ?= -O2 
CPPFLAGS ?= -Wall -Wextra -std=c++11 `$(SDL2CONFIG) --cflags` 
LDFLAGS ?=
LDLIBS ?= `$(SDL2CONFIG) --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2 -lSDL2_gfx -lm -lstdc++

#MINGW does not have X11 and does not require it
#dont know about cygwin
ifneq ($(OS),Windows_NT)
ifeq ($(NOX11),)
LDLIBS += -lX11
endif
endif

FINALDEFINES = $(DEFINES)

GAMEDIR = $(DESTDIR)$(PREFIX)/games/znax
DESKTOPDIR = $(DESTDIR)$(PREFIX)/share/applications
ICONDIR = $(DESTDIR)$(PREFIX)/share/icons/


.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $(LDFLAGS) $(TARGET_ARCH) $^ $(LDLIBS) $(FINALDEFINES) -o $@ 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CFLAGS) $(FINALDEFINES) -c $< -o $@

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
