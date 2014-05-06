MAKE_CMD = make
CMAKE_CMD = cmake

BUILD_DIR = build

# Tasks which do not correspond to files:
.PHONY: all cmake clean force

all: cmake
	@$(MAKE_CMD) -C $(BUILD_DIR)

clean:
	rm -r $(BUILD_DIR)

# Create the build directory
$(BUILD_DIR):
	mkdir $(BUILD_DIR)

# Run cmake command in build
cmake: $(BUILD_DIR)
	cd build && $(CMAKE_CMD) ..

# Forward everything else to makefile in build
%:
	@$(MAKE_CMD) -C $(BUILD_DIR) $@
