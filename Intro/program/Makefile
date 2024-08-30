CODENAME := app

# (Major, Minor, Patch)
VERSION := 0.0.0

# OS
OS               ?= linux
OS_SUPPORT       := linux windows
OS_RESULT := $(shell echo $(OS_SUPPORT) | tr ' ' '\n' | grep -x $(OS))

ifneq ($(OS), $(OS_RESULT))
$(error Unsupported OS ($(OS)). Supported: [$(OS_SUPPORT)])
endif

# MODE
MODE               ?= release
MODE_SUPPORT       := debug release
MODE_RESULT := $(shell echo $(MODE_SUPPORT) | tr ' ' '\n' | grep -x $(MODE))

ifneq ($(MODE), $(MODE_RESULT))
$(error Unsupported MODE ($(MODE)). Supported: [$(MODE_SUPPORT)])
endif

# Date
DATE                     := $(shell date +"%Y %b %d %H:%M:%S")
DATE_WITH_ESCAPED_SPACES := $(shell echo $(DATE) | sed "s: :\\\ :g")

# Code
CODE_DIR := code

# Target
TARGET_DIR := target
TARGET     := $(TARGET_DIR)/$(CODENAME)-$(MODE)
ifeq ($(OS), windows)
	TARGET := $(TARGET).exe
endif

# Includes
INCLUDES := $(CODE_DIR)

# Compilation Dependencies
DEPS_DIR              :=
DEPS_DEFINES          :=
DEPS_LD_FLAGS         :=
DEPS_HEADER_DIRS      :=
DEPS_SOURCE_DIRS      :=

# Compilation / Linking
ifeq ($(OS), linux)
	CXX := gcc
else ifeq ($(OS), windows)
	CXX := x86_64-w64-mingw32-gcc
endif

CXX_FLAGS := -Wno-trigraphs $(patsubst %, -I%, $(INCLUDES))
ifeq ($(MODE), debug)
	CXX_FLAGS += -Og
else ifeq ($(MODE), release)
	CXX_FLAGS += -O3
endif

DEFINES  := -DMAKE_DATE=\"$(DATE_WITH_ESCAPED_SPACES)\" -DMAKE_VERSION=\"$(VERSION)-$(OS)-$(MODE)\" -DMAKE_CODENAME=\"$(CODENAME)\" $(DEPS_DEFINES)
LD_FLAGS := -static-libgcc -lm $(DEPS_LD_FLAGS)

# Header
HEADER_DIRS      := $(CODE_DIR) $(DEPS_HEADER_DIRS)
HEADER_EXTENSION := h
HEADER_FILES     := $(shell find $(HEADER_DIR) -type f -name "*.$(HEADER_EXTENSION)")
HEADER_COUNT     := $(shell echo $(HEADER_FILES) | wc --words)

# Source
SOURCE_DIRS      := $(CODE_DIR) $(DEPS_SOURCE_DIRS)
SOURCE_EXTENSION := c
SOURCE_FILES     := $(shell find $(SOURCE_DIRS) -type f -name "*.$(SOURCE_EXTENSION)")
SOURCE_COUNT     := $(shell echo $(SOURCE_FILES) | wc --words)

# Build
BUILD_DIR                  := build
BUILD_DIR_SPECIFIC         := $(BUILD_DIR)/$(OS)-$(MODE)
BUILD_OBJECT_EXTENSION     := o
BUILD_OBJECT_FILES         := $(patsubst %, $(BUILD_DIR_SPECIFIC)/%, $(SOURCE_FILES:.$(SOURCE_EXTENSION)=.$(BUILD_OBJECT_EXTENSION)))
BUILD_DEPENDENCY_EXTENSION := d
BUILD_DEPENDENCIES         := $(shell find $(BUILD_DIR_SPECIFIC) -type f -name "*.$(BUILD_DEPENDENCY_EXTENSION)" 2>/dev/null)

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

# Plots
PLOT_LOOPS_TO_FIND_MIN                 := 100
PLOT_MEASURMENT_LOOPS                  := 100
PLOT_MEASUREMENTS_PER_OPERATION        := 100
PLOT_SEQUENTIAL_ACCESS_SIZE_MULTIPLIER := 1000
PLOT_RANDOM_ACCESS_SIZE_MULTIPLIER     := 1000
PLOT_SEARCH_SIZE_MULTIPLIER            := 100
PLOT_DUPLICATES_SIZE_MULTIPLIER        := 10
PLOT_DIR                               := plots
PLOT_DATA_DIR                          := $(PLOT_DIR)/data
PLOT_PDFS_DIR                          := $(PLOT_DIR)/pdfs

.PHONY: all compile run plot-prepare plot clean-target clean-build clean-plots clean config-info

.DEFAULT_GOAL := all

all: plot-prepare run plot

compile: config-info $(TARGET)

run: compile
	@echo "$(BOLD)$(BLUE)[Running]$(RESET)"
	./$(TARGET) \
	$(PLOT_LOOPS_TO_FIND_MIN) \
	$(PLOT_MEASURMENT_LOOPS) \
	$(PLOT_MEASUREMENTS_PER_OPERATION) \
	$(PLOT_SEQUENTIAL_ACCESS_SIZE_MULTIPLIER) \
	$(PLOT_RANDOM_ACCESS_SIZE_MULTIPLIER) \
	$(PLOT_SEARCH_SIZE_MULTIPLIER) \
	$(PLOT_DUPLICATES_SIZE_MULTIPLIER)

plot-prepare:
	@echo "$(BOLD)$(BLUE)[Preparing for plotting]$(RESET)"
	@mkdir -p $(PLOT_DIR)
	@mkdir -p $(PLOT_DATA_DIR)

plot:
	@echo "$(BOLD)$(BLUE)[Making plots]$(RESET)"
	@mkdir -p $(PLOT_PDFS_DIR)
	gnuplot -e "name='plot1'" -e "dataDir='$(PLOT_DATA_DIR)'" -e "pdfsDir='$(PLOT_PDFS_DIR)'" plot.gp
	gnuplot -e "name='plot2'" -e "dataDir='$(PLOT_DATA_DIR)'" -e "pdfsDir='$(PLOT_PDFS_DIR)'" plot.gp
	gnuplot -e "name='plot3'" -e "dataDir='$(PLOT_DATA_DIR)'" -e "pdfsDir='$(PLOT_PDFS_DIR)'" plot.gp
	gnuplot -e "name='plot4'" -e "dataDir='$(PLOT_DATA_DIR)'" -e "pdfsDir='$(PLOT_PDFS_DIR)'" plot.gp

clean-target:
	@echo "$(BOLD)$(RED)[Cleaning target]$(RESET)"
	rm -rf $(TARGET_DIR)

clean-build:
	@echo "$(BOLD)$(RED)[Cleaning build]$(RESET)"
	rm -rf $(BUILD_DIR)

clean-plots:
	@echo "$(BOLD)$(RED)[Cleaning plots]$(RESET)"
	rm -rf $(PLOT_DIR)

clean: clean-target clean-build clean-plots

config-info:
	@echo "$(BOLD)$(BLUE)[$(OS)-$(MODE)]$(RESET)"

$(TARGET): $(BUILD_OBJECT_FILES) | $(TARGET_DIR)
	@echo "$(BOLD)$(YELLOW)[Info]$(RESET)"
	@echo "Date   : $(DATE)"
	@echo "OS     : $(OS)"
	@echo "Mode   : $(MODE)"
	@echo "Target : $(TARGET)"
	@echo "Files  : [ .$(SOURCE_EXTENSION): $(SOURCE_COUNT), .$(HEADER_EXTENSION): $(HEADER_COUNT) ]"

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
