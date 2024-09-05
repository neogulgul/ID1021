# OS
OS               ?= linux
OS_SUPPORT       := linux windows
OS_RESULT := $(shell echo $(OS_SUPPORT) | tr ' ' '\n' | grep -x $(OS))

ifneq ($(OS), $(OS_RESULT))
$(error Unsupported OS ($(OS)). Supported: [$(OS_SUPPORT)])
endif

# MODE
MODE               ?= debug
MODE_SUPPORT       := debug release
MODE_RESULT := $(shell echo $(MODE_SUPPORT) | tr ' ' '\n' | grep -x $(MODE))

ifneq ($(MODE), $(MODE_RESULT))
$(error Unsupported MODE ($(MODE)). Supported: [$(MODE_SUPPORT)])
endif

#######################
# CONFIGURATION START #
#######################

CODENAME := stack-calculator

# (Major, Minor, Patch)
VERSION := 1.0.0

HEADER_DIRS := code
SOURCE_DIRS := code
TARGET_DIR  := target
BUILD_DIR   := build

# Project Configuration
PROJ_DEFINES          :=
PROJ_LD_FLAGS         :=
PROJ_HEADER_DIRS      :=
PROJ_SOURCE_DIRS      :=

######################
# CONFIGURATION STOP #
######################

# Date
DATE                     := $(shell date +"%Y %b %d %H:%M:%S")
DATE_WITH_ESCAPED_SPACES := $(shell echo $(DATE) | sed "s: :\\\ :g")

# Target
TARGET     := $(TARGET_DIR)/$(CODENAME)-$(MODE)
ifeq ($(OS), windows)
	TARGET := $(TARGET).exe
endif

# Header
HEADER_DIRS      += $(PROJ_HEADER_DIRS)
HEADER_EXTENSION := h
HEADER_FILES     := $(shell find $(HEADER_DIRS) -type f -name "*.$(HEADER_EXTENSION)")
HEADER_COUNT     := $(shell echo $(HEADER_FILES) | wc --words)

# Source
SOURCE_DIRS      += $(PROJ_SOURCE_DIRS)
SOURCE_EXTENSION := c
SOURCE_FILES     := $(shell find $(SOURCE_DIRS) -type f -name "*.$(SOURCE_EXTENSION)")
SOURCE_COUNT     := $(shell echo $(SOURCE_FILES) | wc --words)

# Build
BUILD_DIR_SPECIFIC         := $(BUILD_DIR)/$(OS)-$(MODE)
BUILD_OBJECT_EXTENSION     := o
BUILD_OBJECT_FILES         := $(patsubst %, $(BUILD_DIR_SPECIFIC)/%, $(SOURCE_FILES:.$(SOURCE_EXTENSION)=.$(BUILD_OBJECT_EXTENSION)))
BUILD_DEPENDENCY_EXTENSION := d
BUILD_DEPENDENCIES         := $(shell find $(BUILD_DIR_SPECIFIC) -type f -name "*.$(BUILD_DEPENDENCY_EXTENSION)" 2>/dev/null)

# Compilation / Linking
ifeq ($(OS), linux)
	CXX := gcc
else ifeq ($(OS), windows)
	CXX := x86_64-w64-mingw32-gcc
endif

CXX_FLAGS := -Wno-trigraphs $(patsubst %, -I%, $(HEADER_DIRS))
ifeq ($(MODE), debug)
	CXX_FLAGS += -Og
else ifeq ($(MODE), release)
	CXX_FLAGS += -O3
endif

DEFINES  := -DMAKE_DATE=\"$(DATE_WITH_ESCAPED_SPACES)\" -DMAKE_VERSION=\"$(VERSION)-$(OS)-$(MODE)\" -DMAKE_CODENAME=\"$(CODENAME)\" $(PROJ_DEFINES)
LD_FLAGS := -static-libgcc $(PROJ_LD_FLAGS)

BOLD    := $(shell tput bold)
RED     := $(shell tput setaf 1)
GREEN   := $(shell tput setaf 2)
YELLOW  := $(shell tput setaf 3)
BLUE    := $(shell tput setaf 4)
MAGENTA := $(shell tput setaf 5)
CYAN    := $(shell tput setaf 6)
RESET   := $(shell tput sgr0)

SET_BOLD    := @echo -n "$(BOLD)"
SET_RED     := @echo -n "$(RED)"
SET_GREEN   := @echo -n "$(GREEN)"
SET_YELLOW  := @echo -n "$(YELLOW)"
SET_BLUE    := @echo -n "$(BLUE)"
SET_MAGENTA := @echo -n "$(MAGENTA)"
SET_CYAN    := @echo -n "$(CYAN)"
SET_RESET   := @echo -n "$(RESET)"

.PHONY: compile recompile run clean-target clean-build clean config-info

.DEFAULT_GOAL := compile

compile: config-info $(TARGET)

recompile: clean compile

run: compile
	@echo "$(BOLD)$(BLUE)[Running]$(RESET)"
	./$(TARGET)

clean-target:
	@echo "$(BOLD)$(RED)[Cleaning target]$(RESET)"
	rm -rf $(TARGET_DIR)

clean-build:
	@echo "$(BOLD)$(RED)[Cleaning build]$(RESET)"
	rm -rf $(BUILD_DIR)

clean: clean-target clean-build

config-info:
	@echo "$(BOLD)$(BLUE)[$(OS)-$(MODE)]$(RESET)"

$(TARGET): $(BUILD_OBJECT_FILES) | $(TARGET_DIR)
	@echo "$(BOLD)$(YELLOW)[Info]$(RESET)"
	@echo "Date    : $(DATE)"
	@echo "Version : $(VERSION)"
	@echo "OS      : $(OS)"
	@echo "Mode    : $(MODE)"
	@echo "Target  : $(TARGET)"
	@echo "Files   : [ .$(SOURCE_EXTENSION): $(SOURCE_COUNT), .$(HEADER_EXTENSION): $(HEADER_COUNT) ]"

	@echo "$(BOLD)$(MAGENTA)[Linking]$(RESET)"
	$(CXX) -o $@ $^ $(LD_FLAGS)

	@echo "$(BOLD)$(CYAN)[Done]$(RESET)"

$(BUILD_DIR_SPECIFIC)/%.$(BUILD_OBJECT_EXTENSION): %.$(SOURCE_EXTENSION)
	$(SET_BOLD)
	$(SET_GREEN)
	@echo "Compiling: $*.$(SOURCE_EXTENSION)"
	$(SET_RESET)

	@mkdir -p "$(shell echo $@ | grep -P "(.*)/" -o)"
	$(CXX) -MMD -MP -c -o $@ $< $(CXX_FLAGS) $(DEFINES)

$(TARGET_DIR):
	@mkdir -p $@

-include $(BUILD_DEPENDENCIES)
