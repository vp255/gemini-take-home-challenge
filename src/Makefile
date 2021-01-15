CXX                     := -g++ -std=c++17 -g
CXXFLAGS                := -pedantic-errors -Wall -Wextra -Werror -static-libgcc
LDFLAGS                 := -L/usr/lib -lstdc++ -lm -pthread
BUILD                   := ./build
OBJ_DIR                 := $(BUILD)/objects
APP_DIR                 := $(BUILD)/apps
LIB_DIR                 := $(BUILD)/libraries
TARGET                  := match_engine
HOME                    := /home/$(USERNAME)
CPPDIRS                 := $(HOME)/src/cpp
INCLUDE                 := -I$(CPPDIRS)
MATCH_ENGINE_SRC        := $(wildcard ./*.cpp)

all: build $(APP_DIR)/$(TARGET)

MATCHING_ENGINE_OBJS  := $(MATCH_ENGINE_SRC:%.cpp=$(OBJ_DIR)/%.o)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@ $(LDFLAGS)

$(APP_DIR)/$(TARGET): $(MATCHING_ENGINE_OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(APP_DIR)/$(TARGET) $(MATCHING_ENGINE_OBJS) $(LDFLAGS) -L$(LIB_DIR)

.PHONY: all build clean debug tests

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(LIB_DIR)
	
debug: CXXFLAGS += -DDEBUG -g
debug: all

clean:
	-@rm -rvf $(BUILD)

