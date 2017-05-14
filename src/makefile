#
# makefile
#
# using raylib by raysan5
# using raygui by raysan5
#
# karl eirik bang fossberg
# system: mac os 10.11
#

RAYLIB_PATH = ../../../cubed/lib/raylib
RAYGUI_PATH = ../../raygui
TINYFILE_PATH = ../../tinyfiledialogs-code

CXX = llvm-g++
SRC = rfxgen.c $(TINYFILE_PATH)/tinyfiledialogs.c
CFLAGS = -std=c++11 -Wno-c++11-narrowing -Wno-writable-strings

CFLAGS += -I$(RAYLIB_PATH)/release/osx
CFLAGS += -I$(RAYGUI_PATH)
LFLAGS += $(RAYLIB_PATH)/release/osx/libraylib.a
LFLAGS += `pkg-config --static --libs glfw3`
LFLAGS += -framework OpenGL -framework OpenAL

all:
	$(CXX) $(CFLAGS) $(LFLAGS) $(SRC) -o rfxgen
