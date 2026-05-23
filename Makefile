COMPILE = g++ -std=c++17 -Wall -Wextra -I./include

SRC_DIR = src
TEST_DIR = present
BUILD_DIR = build

LIB = $(BUILD_DIR)/libtester.a
TARGET = $(BUILD_DIR)/runTests

SRC_FILES := $(shell find $(SRC_DIR) -name "*.cpp")
TEST_FILES := $(shell find $(TEST_DIR) -name "*.cpp")
MAIN_FILE := runtime/Main.cpp

SRC_OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/obj/%.o,$(SRC_FILES))
TEST_OBJECTS := $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/tests_%.o,$(TEST_FILES))

all: $(BUILD_DIR) $(LIB) $(TARGET)

$(LIB): $(SRC_OBJECTS)
	@echo "Archiving library..."
	ar rcs $@ $^

$(TARGET): $(TEST_OBJECTS) $(LIB) $(MAIN_FILE)
	$(CXX) $(CXXFLAGS) $(MAIN_FILE) $(TEST_OBJECTS) $(LIB) -o $@

$(BUILD_DIR)/obj/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(COMPILE) -c $< -o $@

$(BUILD_DIR)/tests_%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(COMPILE) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

run: clean all
	./$(TARGET)
