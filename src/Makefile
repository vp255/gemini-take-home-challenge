CXX                     := -g++ -std=c++17 -g
CXXFLAGS                := -pedantic-errors -Wall -Wextra -Werror -static-libgcc
LDFLAGS                 := -L/usr/lib -lstdc++ -lm -pthread
BUILD                   := ./build
MATCH_ENGINE_OBJ_DIR                 := $(BUILD)/matching_engine_objects
TEST_OBJ_DIR                 := $(BUILD)/matching_engine_objects
APP_DIR                 := $(BUILD)/apps
LIB_DIR                 := $(BUILD)/libraries
TARGET                  := match_engine
TEST_TARGET             := unit_tests
MATCH_ENGINE_DIR        := ./match_engine
MATCH_ENGINE_TESTS_DIR  := ./match_engine_tests
INCLUDE                 := -I$(CPPDIRS)
MATCH_ENGINE_SRC        := $(wildcard ./match_engine/*.cpp)
MATCH_ENGINE_TESTS_SRC  := $(wildcard ./match_engine_tests/*.cpp)

all: build $(APP_DIR)/$(TARGET) $(APP_DIR)/$(TEST_TARGET)

MATCHING_ENGINE_OBJS       := $(MATCH_ENGINE_SRC:%.cpp=$(MATCH_ENGINE_OBJ_DIR)/%.o)
TEST_OBJS                  := $(MATCH_ENGINE_TESTS_SRC:%.cpp=$(TEST_OBJ_DIR)/%.o)

$(MATCH_ENGINE_OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -I$(MATCH_ENGINE_DIR) -c $< -o $@ $(LDFLAGS)

$(TEST_OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	echo -I$(MATCH_ENGINE_DIR) -I$(MATCH_ENGINE_TESTS_DIR) 
	$(CXX) $(CXXFLAGS) -I$(MATCH_ENGINE_DIR) -I$(MATCH_ENGINE_TESTS_DIR) -c $< -o $@ $(LDFLAGS)

$(APP_DIR)/$(TARGET): $(MATCHING_ENGINE_OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(APP_DIR)/$(TARGET) $(MATCHING_ENGINE_OBJS) $(LDFLAGS) -L$(LIB_DIR)

$(APP_DIR)/$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $(APP_DIR)/$(TEST_TARGET) $(TEST_OBJS) $(LDFLAGS) -L$(LIB_DIR)

.PHONY: all build clean debug tests

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(MATCH_ENGINE_OBJ_DIR)
	@mkdir -p $(TEST_OBJ_DIR)
	@mkdir -p $(LIB_DIR)
	
debug: CXXFLAGS += -DDEBUG -g
debug: all

clean:
	-@rm -rvf $(BUILD)

