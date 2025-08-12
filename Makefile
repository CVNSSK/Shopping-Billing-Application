# -------- Project --------
TARGET      := quickbill
BUILD_DIR   := build
SRC_DIR     := src
DATA_DIR    := data

# -------- Toolchain --------
CC          := gcc
# Debug by default; toggle with `make MODE=release`
MODE        ?= debug

CWARN       := -Wall -Wextra -Wpedantic
CSTD        := -std=c11

ifeq ($(MODE),release)
  COPT     := -O2 -DNDEBUG
else
  COPT     := -O0 -g
endif

CFLAGS      := $(CSTD) $(CWARN) $(COPT)

# Windows exe suffix
ifeq ($(OS),Windows_NT)
  EXE := .exe
  RM  := del /Q
  MKDIR := mkdir
else
  EXE :=
  RM  := rm -f
  MKDIR := mkdir -p
endif

# -------- Sources / Objects --------
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))

# -------- Default --------
.PHONY: all
all: $(TARGET)$(EXE)

# -------- Link --------
$(TARGET)$(EXE): $(OBJ)
	@$(MKDIR) $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

# -------- Compile --------
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@$(MKDIR) $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# -------- Utilities --------
.PHONY: run
run: all
	./$(TARGET)$(EXE)

.PHONY: clean
clean:
	$(RM) $(BUILD_DIR)/* $(TARGET)$(EXE)

# Seed minimal runtime files if missing (safe defaults)
.PHONY: init
init:
	@$(MKDIR) $(DATA_DIR)
	@if [ ! -f "$(DATA_DIR)/cidupdate.txt" ]; then echo 1 > $(DATA_DIR)/cidupdate.txt; fi
	@if [ ! -f "$(DATA_DIR)/password.txt" ] && [ -f "$(DATA_DIR)/password_sample.txt" ]; then cp $(DATA_DIR)/password_sample.txt $(DATA_DIR)/password.txt; fi

# Optional: code format (requires clang-format if installed)
.PHONY: format
format:
	@command -v clang-format >/dev/null 2>&1 && clang-format -i $(SRC) || echo "clang-format not found; skipping"
