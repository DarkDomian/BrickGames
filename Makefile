# =============================================================================
# Compiler Configuration
# =============================================================================
CC				::=		gcc
CFLAGS			::=		-g -std=c11 -Wall -Werror -Wextra -Wpedantic
TST_FLAG		::=		$(shell pkg-config --cflags --libs check)
COV_FLAGS		::=		-fprofile-arcs -ftest-coverage
REL_FLAG		::=		-DNDEBUG -O2

INCLUDE			=		$(addprefix -I, $(INCLUDE_DIR))

# =============================================================================
# Build Mode Configuration using MAKECMDGOALS
# =============================================================================
ifeq ($(MAKECMDGOALS),gcov_report)
    CFLAGS 		+= 		$(COV_FLAGS)
endif

# =============================================================================
# Platform-Specific Configuration
# =============================================================================
ifeq ($(shell uname),Darwin)
    OPENCMD ::= open
else ifneq ($(shell uname -r | grep -i microsoft),)
    ifneq ($(shell which wslview 2>/dev/null),)
        OPENCMD ::= wslview
    else
        OPENCMD ::= cmd.exe /c start
    endif
else
    OPENCMD ::= xdg-open
endif

# =============================================================================
# Directory Structure
# =============================================================================
INCLUDE_DIR		::=		./brick_game/tetris/include

BUILD			::=		./build
BUILD.obj		::=		./build/obj
BUILD.cov		::=		./build/coverage
BUILD.bin		::=		./build/bin
BUILD.lib		::=		./build/lib

SRC.tetris		::=		./brick_game/tetris/src
SRC.cli			::=		./gui/cli/src
SRC.tests		::=		./brick_game/tetris/tests

OBJD.tetris		::=		$(BUILD.obj)/tetris
OBJD.cli		::=		$(BUILD.obj)/cli
OBJD.tests		::=		$(BUILD.obj)/tests

# =============================================================================
# Source and Object Files
# =============================================================================
OBJ.tetris		=		$(patsubst $(SRC.tetris)/%.c, $(OBJD.tetris)/%.o, $(wildcard $(SRC.tetris)/*.c))
OBJ.cli			=		$(patsubst $(SRC.cli)/%.c, $(OBJD.cli)/%.o, $(wildcard $(SRC.cli)/*.c))
OBJ.tests		=		$(patsubst $(SRC.tests)/%.c, $(OBJD.tests)/%.o, $(wildcard $(SRC.tests)/*.c))

# =============================================================================
# Main Targets
# =============================================================================
LIB.tetris			::=		$(BUILD.lib)/libtetris.a

BIN.cli				::=		$(BUILD.bin)/cli.o

APP.tetris_cli		::=		./build/tetris_cli

TEST.tetris			::=		./build/tetris_test

.PHONY: all debug release style_format style_check gcov_report clean rebuild

all: $(APP.tetris_cli)

# =============================================================================
# Build Rules
# =============================================================================
$(APP.tetris_cli): $(LIB.tetris) $(OBJ.cli)
	$(info Linking and running the $@ app...)
	@$(CC) $(CFLAGS) $(INCLUDE) -L./$(BUILD.lib) $(OBJ.cli) -ltetris -lncurses -o $@
	@ln -s $@ ./tetris
	@$@

$(LIB.tetris): $(OBJ.tetris) | $(BUILD.lib)
	$(info Create BrickGame library $@...)
	@ar rcs $@ $^
	@ranlib $@

$(OBJD.tetris)/%.o: $(SRC.tetris)/%.c | $(OBJD.tetris)
	$(info Compiing the $@ file...)
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(OBJD.cli)/%.o: $(SRC.cli)/%.c | $(OBJD.cli)
	$(info Compiing the $@ file...)
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

# =============================================================================
# Testing Rules
# =============================================================================
test: $(TEST.tetris)

# possible run direct suite with CK_RUN_SUITE env
$(TEST.tetris): $(OBJ.tests) $(OBJ.tetris) $(OBJ.cli)
	$(info Compile tests and running with valgrind...)
	@$(CC) $(CFLAGS) $(INCLUDE) $^ $(TST_FLAG) -o $@
	@CK_FORK=no valgrind --tool=memcheck --leak-check=full --track-origins=yes $@

$(OBJD.tests)/%.o: $(SRC.tests)/%.c | $(OBJD.tests)
	$(info Building the $@ object file...)
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< $(TST_FLAG) -o $@

# =============================================================================
# Assemble Coverage Data to Web-Page
# =============================================================================
gcov_report: test | $(BUILD.cov)
	$(info Generating coverage report...)
	@lcov --test-name "brick-game/tetris" -v --output-file $(COV_REPORT_DIR)/coverage.info --capture -d $(OBJD.tetris) -d $(OBJD.cli)
	@genhtml $(BUILD.cov)/coverage.info --show-navigation --dark-mode --legend --output-directory $(BUILD.cov)
	@$(OPENCMD) $(BUILD.cov)/index.html || true

# =============================================================================
# Code Quality Rules
# =============================================================================
style_format:
	$(info Formatting code with clang-format...)
	@find . -name "*.h" -o -name "*.c" | xargs clang-format -i --verbose --style="{BasedOnStyle: Google}"

style_check:
	$(info Checking style with clang-format and cppcheck...)
	@clang-format -n --style="{BasedOnStyle: Google}" --Werror ./*/*.[h,c]
	@cppcheck --enable=all --force --suppress=missingIncludeSystem --check-level=exhaustive --error-exitcode=1 $(SRC.tetris) $(SRC.cli) $(SRC.tests)
	@echo "Style check passed successfully!"

# =============================================================================
# Build Mode Rules
# =============================================================================
clean:
	$(info Cleaning the build artifacts...)
	@rm -rf $(BUILD)

rebuild: clean all

# =============================================================================
# Directory creation
# =============================================================================
$(OBJD.tetris):
	$(info Prepairing the $@ for work...)
	@mkdir -p $@

$(OBJD.cli):
	$(info Prepairing the $@ for work...)
	@mkdir -p $@

$(OBJD.tests):
	$(info Prepairing the $@ for work...)
	@mkdir -p $@

$(BUILD.cov):
	$(info Prepairing the $@ for work...)
	@mkdir -p $@

$(BUILD.lib):
	$(info Preparing the $@ for work...)
	@mkdir -p $@


.PHONY: all install uninstall clean dvi dist test gcov_report help

# =============================================================================
# Documentary section
# =============================================================================
help:
	@echo "================================================================================"
	@echo "                     Brick Game: Modular Game Platform"
	@echo "================================================================================"
	@echo "Project:    Brick Game - Modular gaming platform with backward compatibility"
	@echo "Current:    Tetris implementation for CLI interface"
	@echo "Version:    1.0.0"
	@echo "Author:     Demian \"trelawnm\" Domozhirov 2025"
	@echo "Architecture: Multi-interface, cross-platform game engine"
	@echo ""
	@echo "DESCRIPTION:"
	@echo "  This is a modular game platform where each game (starting with Tetris)"
	@echo "  can run on multiple interfaces (CLI, Desktop GUI, etc.) while maintaining"
	@echo "  backward compatibility. The architecture allows:"
	@echo "  - Running Tetris on future desktop interfaces"
	@echo "  - Running future games on current CLI interface"
	@echo "  - Consistent API across all game implementations"
	@echo ""
	@echo "BUILD TARGETS:"
	@echo "  all          - Build and run the main Tetris application (default)"
	@echo "  debug        - Build with debug symbols (implied in default build)"
	@echo "  release      - Build with optimizations (-O2, NDEBUG defined)"
	@echo "  test         - Build and run unit tests with valgrind memory checking"
	@echo "  gcov_report  - Generate code coverage report (opens in browser)"
	@echo ""
	@echo "CODE QUALITY:"
	@echo "  style_format - Format all source code using clang-format (Google style)"
	@echo "  style_check  - Validate code style and run static analysis (cppcheck)"
	@echo ""
	@echo "MAINTENANCE:"
	@echo "  clean        - Remove all build artifacts and temporary files"
	@echo "  rebuild      - Clean build and rebuild everything"
	@echo "  install      - Install application to system (not implemented)"
	@echo "  uninstall    - Remove installed files (not implemented)"
	@echo ""
	@echo "PLATFORM SUPPORT:"
	@echo "  macOS:      Native support via open command"
	@echo "  Linux:      Native support via xdg-open"
	@echo "  WSL:        Support via wslview or Windows start"
	@echo ""
	@echo "DIRECTORY STRUCTURE:"
	@echo "  brick_game/tetris/ - Core Tetris game logic"
	@echo "  gui/cli/           - Command-line interface implementation"
	@echo "  tests/             - Unit and integration tests"
	@echo "  build/             - Build artifacts and coverage reports"
	@echo "  include/           - Public header files"
	@echo ""
	@echo "COMPILER: $(CC) $(CFLAGS)"
	@echo "================================================================================"