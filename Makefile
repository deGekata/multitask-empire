SRC_DIRS := src targets ecs signal player physics renderer logger geometry graphics/SFML
VPATH += $(SRC_DIRS)

INC_DIRS := . include stdlike utility geometry graphics graphics/SFML
BIN_DIR := bin
BUILD_DIR := build

APPLICATION := $(BUILD_DIR)/multitask_empire.out

SRC := $(wildcard $(addsuffix /*.cpp, $(SRC_DIRS)))
OBJ := $(addprefix $(BIN_DIR)/, $(patsubst %.cpp, %.o, $(notdir $(SRC))))

CXX := clang++

CXX_FLAGS := $(addprefix -I, $(INC_DIRS)) $(addprefix -I, $(SRC_DIRS))\
-DLOG_ALL -ggdb3 -std=c++20 -O0 -pthread -fsized-deallocation -Weverything\
-Wno-switch-enum -Wno-signed-enum-bitfield -Wno-deprecated-register\
-Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-c++11-compat-pedantic\
-Wno-nested-anon-types -Wno-gnu-anonymous-struct -Wno-missing-prototypes\
-Wno-documentation -Wno-documentation-unknown-command -Wno-weak-vtables\
-Wno-unused-const-variable -Wno-format-nonliteral -Wno-global-constructors\
-Wno-exit-time-destructors -Wno-error=padded -Wno-padded

LD_FLAGS := $(addprefix -I, $(INC_DIRS)) $(addprefix -I, $(SRC_DIRS))\
-fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow\
-fno-omit-frame-pointer -fPIE -fsanitize=address -fsanitize=undefined\
-lsfml-graphics -lsfml-window -lsfml-system \

all: prepare $(APPLICATION)

$(APPLICATION): $(OBJ)
	@$(CXX) $(LD_FLAGS) $^ -o $@ $(LD_FLAGS)

$(BIN_DIR)/%.o: %.cpp
	@$(CXX) $< -c -MD -o $@ $(CXX_FLAGS)
 
-include $(wildcard $(BIN_DIR)/*.d)

.PHONY: all prepare clean info run gdb valgrind

run: all
	@$(APPLICATION)

gdb: all
	@gdb $(APPLICATION)

valgrind: all
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
	@echo [*] DLL: $(DLL_FILES)
