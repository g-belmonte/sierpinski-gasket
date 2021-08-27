TARGET_EXEC := sierpinski

BUILD_DIR := build
SRC_DIR := src

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:%.cpp=$(BUILD_DIR)/%.o)
DEP := $(OBJ:%.o=%.d)

CXXFLAGS = -Wall -g -MMD
LDFLAGS = -lglfw -lGLEW -lGL

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJ)
		$(CXX) $(OBJ) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cpp
		[[ -d $(dir $@) ]] || mkdir -p $(dir $@)
		$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
		$(RM) -r $(BUILD_DIR)

.PHONY: run
run: $(BUILD_DIR)/$(TARGET_EXEC)
		./$(BUILD_DIR)/$(TARGET_EXEC)

-include $(DEP)