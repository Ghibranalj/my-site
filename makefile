CC = emcc
SRC = src
BUILD = build
RUNNER = emrun


SOURCES = $(wildcard $(SRC)/*.c)
SHELL_HTML = index.html

CCFLAGS=-Wall -D_DEFAULT_SOURCE -Os -s USE_GLFW=3 -s -sASYNCIFY -s TOTAL_MEMORY=16777216 -s FORCE_FILESYSTEM=1 -DPLATFORM_WEB
RAYLIB_INCLUDE=vendor/raylib_wasm/include
RAYLIB_LIBRARY=vendor/raylib_wasm/lib/libraylib.a



PHONY: mkdir binary run

release: mkdir binary

mkdir :
	mkdir -p $(BUILD)

binary: $(SOURCES)
	$(CC) $(SOURCES) -I./$(SRC) -o $(BUILD)/index.html\
	 $(CCFLAGS) $(RAYLIB_LIBRARY) -I$(RAYLIB_INCLUDE) -L$(RAYLIB_LIBRARY) --shell-file $(SHELL_HTML)

run:
	$(RUNNER) $(BUILD)/index.html