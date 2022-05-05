CC=emcc

CFLAGS=-Wall -D_DEFAULT_SOURCE -Os -s USE_GLFW=3 -s -sASYNCIFY -s TOTAL_MEMORY=16777216 -s FORCE_FILESYSTEM=1 -DPLATFORM_WEB
RAYLIB_INCLUDE=vendor/raylib_wasm/include
RAYLIB_LIBRARY=vendor/raylib_wasm/lib/libraylib.a


SOURCEDIR=src
SOURCE_FILES=$(find src -name '*.c')
BUILDDIR=build
SHELL_HTML=index.html
SOURCES := $(shell find $(SOURCEDIR) -name '*.c')

dev: all run

all: 
	mkdir -p $(BUILDDIR)
	$(CC) -o $(BUILDDIR)/index.html $(SOURCES) $(CFLAGS) $(RAYLIB_LIBRARY) -I$(SOURCES) -I$(RAYLIB_INCLUDE) -L$(RAYLIB_LIBRARY) -s --shell-file $(SHELL_HTML)



clean:
	$(RM) -r $(BUILDDIR)
run:
	emrun $(BUILDDIR)/index.html