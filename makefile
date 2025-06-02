CXX = g++
CXXFLAGS = -std=c++20 -Wall -Iinclude
LDFLAGS = -lgtest -lgtest_main -lpthread
BUILD_DIR = build

.phony: all test clean

# Targets
all: $(BUILD_DIR)/main

$(BUILD_DIR)/main: src/main.cpp include/skip_list.hpp
	$(CXX) $(CXXFLAGS) -o $@ src/main.cpp

# Build test binary
$(BUILD_DIR)/test: test/test_skip_list.cpp include/skip_list.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

test: $(BUILD_DIR)/test
	@$(BUILD_DIR)/test

# Create build
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)	

clean:
	rm -f $(BUILD_DIR)/* 