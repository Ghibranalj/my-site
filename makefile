CC = emcc
OBJ_PATH = objs
SRC_PATH = src
LIB_PATH = vendor
RUNNER = 
SHELL_HTML = index.html
ASSET_PATH = resources
# hack for github pages
BUILD_PATH = docs

#FLAGS
CCFLAGS=-Wall -D_DEFAULT_SOURCE -Os -s USE_GLFW=3 -s ASYNCIFY\
		 -s TOTAL_MEMORY=16777216 -s FORCE_FILESYSTEM=1 -DPLATFORM_WEB -sASSERTIONS -s ALLOW_MEMORY_GROWTH=1

#INCLUDE
INC = $(SRC_PATH) $(LIB_PATH)/flecs/include $(LIB_PATH)/raylib_wasm/include

#LIBRARIES
LIB = $(LIB_PATH)/flecs/libflecs_static.a $(LIB_PATH)/raylib_wasm/libraylib.a


### LOGIC ###
SRC = $(shell find $(SRC_PATH) -type f -name *.c)
OBJS  := $(SRC:$(SRC_PATH)/%.c=$(OBJ_PATH)/%.o)
MODULES = $(subst $(SRC_PATH)/,,$(shell find $(SRC_PATH) -type d))
MODULES := $(filter-out $(SRC_PATH),$(MODULES))
INC_PARAMS=$(foreach d, $(INC), -I$d)
LIB_PARAMS=$(foreach d, $(LIB), -L$(d))


### TARGETS ###

dev: mkdir binary run

release: mkdir binary

mkdir :
	mkdir -p $(BUILD_PATH) $(OBJ_PATH)
ifneq ($(MODULES),)
	mkdir -p $(foreach dir,$(MODULES),$(OBJ_PATH)/$(dir))
endif

binary: $(OBJS)
	$(CC) $(OBJS) -o $(BUILD_PATH)/index.html --shell-file $(SHELL_HTML) \
	--preload-file $(ASSET_PATH) \
	$(CCFLAGS) $(LIB_PARAMS) $(LIB)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	$(CC) $(INC_PARAMS) -c -o $@ $<
	
run:
	python -m http.server 8080 --directory $(BUILD_PATH)

clean:
	@rm -rf $(BUILD_PATH) $(OBJ_PATH)

test:
	@echo $(LIB_PARAMS)