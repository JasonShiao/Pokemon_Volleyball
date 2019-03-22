CXX = g++
CXXFLAGS = -Wall -g -MMD -MP -MF $(DEP_DIR)/$*.d
LDFLAGS += -L lib
INCLUDES += -Iinclude -Iinclude/SDL2
LIBRARY +=	\
	-lSDL2-2.0.0 \
	-lSDL2_image-2.0.0 \
	-lSDL2_ttf-2.0.0 \
	-lSDL2_mixer-2.0.0
BUILD_DIR = build
SRC_DIR = src
INC_DIR = include
DEP_DIR = $(BUILD_DIR)/dep
OBJ_DIR = $(BUILD_DIR)/obj
APP_DIR = $(BUILD_DIR)/app
TARGET = mygame

.DEFAULT_GOAL := all

#SRCS = $(wildcard $(SRC_DIR)/*.cpp)
SRCS = $(shell find $(SRC_DIR) -name '*.cpp')
SRCDIRS := $(shell find . -name '*.cpp' -exec dirname {} \; | uniq)
INCS = $(wildcard include/*.h)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
#OBJS = $(SRCS:%.cpp=$(OBJ_DIR)/%.o)
DEPS = $(patsubst $(SRC_DIR)/%.cpp,$(DEP_DIR)/%.d,$(SRCS))

.PHONY: all build clean debug release depend

all: Game

-include $(DEPS)

Game: $(OBJS)
	@mkdir -p $(APP_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) $(LIBRARY) -o $(APP_DIR)/$(TARGET) $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

#$(DEP_DIR)/%.d: $(SRC_DIR)/%.cpp
#	@set -e; \
#	rm -f $@; \
#	$(CXX) -M $(INCLUDES) $< > $@.$$$$; \
#	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
#	rm -f $@.$$$$


#$(APP_DIR)/$(TARGET): $(OBJS)
#	@mkdir -p $(@D)
#	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) $(LIBRARY) -o $(APP_DIR)/$(TARGET) $(OBJS)

buildrepo:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	rm -rvf $(OBJ_DIR)
	rm -rvf $(APP_DIR)
