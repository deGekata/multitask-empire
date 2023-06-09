TARGET_DIR := targets
TEST_DIR := tests

SRC_DIRS := src ecs signal player physics renderer logger geometry\
graphics/SFML objects_configuration utility bot collision gameplay battle bars\
parsing events\
interface interface/manage interface/objects audio audio/manage

VPATH += $(SRC_DIRS) $(TEST_DIR) $(TARGET_DIR)

INC_DIRS := . include
BIN_DIR := bin
BUILD_DIR := build

DEFAULT_TARGET := game
APPLICATION := $(BUILD_DIR)/multitask_empire.out

TARGETS_SRC := $(wildcard $(addsuffix /*.cpp, $(TARGET_DIR)))
TARGETS := $(patsubst %.cpp, %, $(notdir $(TARGETS_SRC)))

SRC := $(wildcard $(addsuffix /*.cpp, $(SRC_DIRS)))
OBJ := $(addprefix $(BIN_DIR)/, $(patsubst %.cpp, %.o, $(notdir $(SRC))))

CXX := clang++

CXX_FLAGS := $(addprefix -I, $(INC_DIRS)) $(addprefix -I, $(SRC_DIRS))\
-ggdb3 -std=c++20 -O0 -pthread -fsized-deallocation -Weverything\
-Wno-switch-enum -Wno-signed-enum-bitfield -Wno-deprecated-register\
-Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-c++11-compat-pedantic\
-Wno-nested-anon-types -Wno-gnu-anonymous-struct -Wno-missing-prototypes\
-Wno-documentation -Wno-documentation-unknown-command -Wno-weak-vtables\
-Wno-unused-const-variable -Wno-format-nonliteral -Wno-global-constructors\
-Wno-deprecated-copy-with-dtor -Wno-exit-time-destructors -Wno-error=padded\
-Wno-padded

LD = ld

DYNAMIC_LIBS := -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio -lfmt

LD_FLAGS := $(addprefix -I, $(INC_DIRS)) $(addprefix -I, $(SRC_DIRS))\
-fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow\
-fno-omit-frame-pointer -fPIE $(DYNAMIC_LIBS)

.PHONY: all

all: $(DEFAULT_TARGET)

$(TARGETS): prepare $(OBJ)
	@$(eval TARGET := $(addprefix $(BIN_DIR)/, $(addsuffix .o, $@)))
	@+make $(TARGET)
	@$(CXX) $(OBJ) $(TARGET) -o $(APPLICATION) $(LD_FLAGS)
	@echo + $(LD) $(APPLICATION)

$(BIN_DIR)/%.o: %.cpp
	@$(CXX) $< -c -MD -o $@ $(CXX_FLAGS)
	@echo + $(CXX) $@

-include $(wildcard $(BIN_DIR)/*.d)

.PHONY: run gdb valgrind prepare clean info

run:
	@$(APPLICATION)

gdb:
	@gdb $(APPLICATION)

valgrind:
	@valgrind --leak-check=full $(APPLICATION)

prepare:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)

clean:
	@rm -rf $(BIN_DIR)
	@rm -rf $(BUILD_DIR)

info:
	@echo [*] OBJ: $(OBJ)
	@echo [*] SRC: $(SRC)
	@echo [*] TARGETS: $(TARGETS)
