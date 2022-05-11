CC = emcc
# hack for github pages
BUILD_PATH = docs
OBJ_PATH = objs
SRC_PATH = src
LIB_PATH = vendor
SHELL_HTML = index.html
ASSET_PATH = resources
DEP_PATH = deps


#INCLUDE
INC = $(shell find $(SRC_PATH) $(LIB_PATH) -type d -name 'include')

#LIBRARIES
LIB = $(shell find $(LIB_PATH) -type f -name '*.a')

#############
### LOGIC ###
#############
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
.PHONY: dev releases mkdir index air

dev: format mkdir index run

release: mkdir index

mkdir :
	@mkdir -p $(BUILD_PATH) $(OBJ_PATH) $(DEP_PATH)
ifneq ($(MODULES),)
	@mkdir -p $(foreach dir,$(MODULES),$(OBJ_PATH)/$(dir) $(DEP_PATH)/$(dir))
endif

index: $(OBJS)
	$(info Building index.html...)
	@$(CC) $^ -o $(BUILD_PATH)/index.html $(CCFLAGS) $(WASMFLAGS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	$(info Building $< ..)
	@$(CC) $(INC_PARAMS) $(DEPFLAGS) $(CCFLAGS) -c -o $@ $<


.PHONY: run
run:
	@python -m http.server 8080 --directory $(BUILD_PATH)

.PHONY: clean
clean:
	@rm -rfv $(BUILD_PATH) $(OBJ_PATH) $(DEP_PATH)
	
.PHONY: format
format:
	@find $(SRC_PATH) -iname *.c -o -iname *.h | xargs clang-format -i

include $(wildcard $(DEPFILES))