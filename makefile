CC = emcc
# hack for github pages
BUILD_PATH = docs
SRC_PATH = src
LIB_PATH = vendor
SHELL_HTML = index.html
ASSET_PATH = resources
TMP_PATH = tmp


#INCLUDE
INC = $(shell find $(SRC_PATH) $(LIB_PATH) -type d -name 'include')

#LIBRARIES
LIB = $(shell find $(LIB_PATH) -type f -name '*.a')

#############
### LOGIC ###
#############
DEP_PATH = $(TMP_PATH)/deps
OBJ_PATH = $(TMP_PATH)/objs

SRC = $(shell find $(SRC_PATH) -type f -name *.c)
OBJS  := $(SRC:$(SRC_PATH)/%.c=$(OBJ_PATH)/%.o)
MODULES = $(subst $(SRC_PATH)/,,$(shell find $(SRC_PATH) -type d))
MODULES := $(filter-out $(SRC_PATH),$(MODULES))
INC_PARAMS=$(foreach d, $(INC), -I$d)
LIB_PARAMS=$(foreach d, $(LIB), -L$(d))
DEPFILES := $(SRC:$(SRC_PATH)/%.c=$(DEP_PATH)/%.d)

#########
# FLAGS #
#########
WASMFLAGS= -s USE_GLFW=3 -s ASYNCIFY -s TOTAL_MEMORY=16777216 -s FORCE_FILESYSTEM=1 \
			-sASSERTIONS -s ALLOW_MEMORY_GROWTH=1 \
		 	--shell-file $(SHELL_HTML) --preload-file $(ASSET_PATH) $(LIB_PARAMS) $(LIB)

CCFLAGS = -Wall  -Wno-unused-parameter -Wno-unused-variable -O0 -DPLATFORM_WEB

DEPFLAGS = -MT $@ -MMD -MP -MF $(DEP_PATH)/$*.d


###############
### TARGETS ###
###############
.PHONY: release mkdir index air all dev run clean format setup

all: format mkdir index

release: mkdir index

# run with make dev -j2
dev: run air

air:
	air -c air.toml

mkdir :
	@mkdir -p $(BUILD_PATH) $(OBJ_PATH) $(DEP_PATH)
ifneq ($(MODULES),)
	@mkdir -p $(foreach dir,$(MODULES),$(OBJ_PATH)/$(dir) $(DEP_PATH)/$(dir))
endif

debug : all
debug : DEBUG_FLAGS=-g3 -DDEBUG=1

index: $(OBJS)
	$(info Building index.html...)
	@$(CC) $^ -o $(BUILD_PATH)/index.html $(CCFLAGS) $(WASMFLAGS) $(DEBUG_FLAGS)
	$(info  **************** BUILD SUCCESS ******************)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	$(info Building $<...)
	@$(CC) $(INC_PARAMS) $(DEPFLAGS) $(CCFLAGS) $(DEBUG_FLAGS) -c -o $@ $<

run:
	$(info running on 8080)
	@live-server $(BUILD_PATH)  --ignore-Pattern=.*tmp.* --wait=1500 -q  

clean:
	@rm -rfv $(BUILD_PATH) $(OBJ_PATH) $(DEP_PATH) $(TMP_PATH)

format:
	@find $(SRC_PATH) -iname *.c -o -iname *.h | xargs clang-format -i

setup : clean
	bear -- make debug

include $(wildcard $(DEPFILES))
