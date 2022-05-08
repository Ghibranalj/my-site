CC = emcc
# hack for github pages
BUILD_PATH = docs
OBJ_PATH = objs
SRC_PATH = src
LIB_PATH = vendor
SHELL_HTML = index.html
ASSET_PATH = resources


#INCLUDE
INC = $(SRC_PATH) $(shell find $(LIB_PATH) -type d -name 'include')

#LIBRARIES
LIB = $(shell find $(LIB_PATH) -type f -name '*.a')


### LOGIC ###
SRC = $(shell find $(SRC_PATH) -type f -name *.c)
OBJS  := $(SRC:$(SRC_PATH)/%.c=$(OBJ_PATH)/%.o)
MODULES = $(subst $(SRC_PATH)/,,$(shell find $(SRC_PATH) -type d))
MODULES := $(filter-out $(SRC_PATH),$(MODULES))
INC_PARAMS=$(foreach d, $(INC), -I$d)
LIB_PARAMS=$(foreach d, $(LIB), -L$(d))

#FLAGS
CCFLAGS=-Wall -D_DEFAULT_SOURCE -Os -s USE_GLFW=3 -s ASYNCIFY\
		 -s TOTAL_MEMORY=16777216 -s FORCE_FILESYSTEM=1 -DPLATFORM_WEB -sASSERTIONS -s ALLOW_MEMORY_GROWTH=1 \
		 --shell-file $(SHELL_HTML) --preload-file $(ASSET_PATH) $(LIB_PARAMS) $(LIB)

### TARGETS ###

.PHONY: dev releases mkdir index

dev: mkdir index run

release: mkdir index

mkdir :
	@mkdir -p $(BUILD_PATH) $(OBJ_PATH)
ifneq ($(MODULES),)
	@mkdir -p $(foreach dir,$(MODULES),$(OBJ_PATH)/$(dir))
endif

index: $(OBJS)
	$(CC) $^ -o $(BUILD_PATH)/index.html $(CCFLAGS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	$(CC) $(INC_PARAMS) -c -o $@ $<

.PHONY: run
run:
	@python -m http.server 8080 --directory $(BUILD_PATH)

.PHONY: clean
clean:
	@rm -rf $(BUILD_PATH) $(OBJ_PATH)