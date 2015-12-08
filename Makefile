CC := clang++
CXX_FLAGS := -std=c++11 -I.

SRC_FILES := $(shell find . -name "*.cpp")
HEADER_FILES := $(shell find . -name "*.h")

ALL_FILES := $(SRC_FILES)
ALL_FILES += $(HEADER_FILES)

all-tests: $(ALL_FILES)
	$(CC) $(CXX_FLAGS) $(SRC_FILES) -o $@

clean:
	rm -f all-tests
