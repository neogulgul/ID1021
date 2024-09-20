SRC_DIR := source
NAME    := report
TARGET  := $(NAME).pdf

FOLDERS_TO_CLEAN := _minted-report
FOLDERS_TO_CLEAN := $(patsubst %, $(SRC_DIR)/%, $(FOLDERS_TO_CLEAN))
FILES_TO_CLEAN   := aux log pyg out
FILES_TO_CLEAN   := $(patsubst %, $(SRC_DIR)/$(NAME).%, $(FILES_TO_CLEAN))

.PHONY: all clean

all: $(TARGET)

clean:
	@rm -f $(TARGET)

$(TARGET): source/report.tex
	@cd $(SRC_DIR); pdflatex -halt-on-error -file-line-error -shell-escape $(NAME).tex
	@cp $(SRC_DIR)/$(TARGET) $(TARGET)
	@rm -f $(SRC_DIR)/$(TARGET)
	@rm -f $(FILES_TO_CLEAN)
	@rm -rf $(FOLDERS_TO_CLEAN)
