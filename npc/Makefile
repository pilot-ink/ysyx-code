TOPNAME = top
NXDC_FILES = constr/top.nxdc
INC_PATH ?= $(NPC_HOME)/csrc/include/ 


VERILATOR = verilator
VERILATOR_CFLAGS += -MMD --build --trace -cc  \
				-O3 --x-assign fast --x-initial fast --noassert --timing

BUILD_DIR = ./build
OBJ_DIR = $(BUILD_DIR)/obj_dir
BIN = $(BUILD_DIR)/$(TOPNAME)

default: $(BIN)

$(shell mkdir -p $(BUILD_DIR))

FILELIST_MK = $(shell find -L ./csrc -name "filelist.mk")
include $(FILELIST_MK)

# constraint file
SRC_AUTO_BIND = $(abspath $(BUILD_DIR)/auto_bind.cpp)
$(SRC_AUTO_BIND): $(NXDC_FILES)
	python3 $(NVBOARD_HOME)/scripts/auto_pin_bind.py $^ $@

# project source
VSRCS = $(shell find $(abspath ./vsrc) -name "*.v")
CSRCS = $(shell find $(abspath ./csrc/src) -name "*.c" -or -name "*.cc" -or -name "*.cpp")
#CSRCS += $(SRC_AUTO_BIND)
ARGS ?= 


# rules for NVBoard
include $(NVBOARD_HOME)/scripts/nvboard.mk

# rules for verilator
INCFLAGS = $(addprefix -I, $(INC_PATH))
CXXFLAGS += $(INCFLAGS) -DTOP_NAME="\"V$(TOPNAME)\"" 
LDFLAGS += -lSDL2 -lSDL2_image -lreadline


LIBCAPSTONE = $(NPC_HOME)/csrc/tools/capstone/repo/libcapstone.so.5
CXXFLAGS += -I $(NPC_HOME)/csrc/tools/capstone/repo/include -DLIBCAPSTONE=\"$(LIBCAPSTONE)\"
src/utils/disasm.c: $(LIBCAPSTONE)
$(LIBCAPSTONE):
	$(MAKE) -C tools/capstone

#bond
BOND = $(shell python3 ${NVBOARD_HOME}/scripts/auto_pin_bind.py ~/ysyx-workbench/npc/constr/top.nxdc ~/ysyx-workbench/npc/build/auto_bind.cpp)

sim: $(BOND) $(VSRCS) $(CSRCS) $(NVBOARD_ARCHIVE)
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	@rm -rf $(OBJ_DIR) wave.vcd
	$(VERILATOR) $(VERILATOR_CFLAGS) \
		--top-module $(TOPNAME) $^ \
		$(addprefix -CFLAGS , $(CXXFLAGS)) $(addprefix -LDFLAGS , $(LDFLAGS)) \
		--Mdir $(OBJ_DIR) --exe -o $(abspath $(BIN)) \

$(BIN): $(VSRCS) $(CSRCS)
	@rm -rf $(OBJ_DIR) wave.vcd
	$(VERILATOR) $(VERILATOR_CFLAGS) \
		--top-module $(TOPNAME) $^ \
		$(addprefix -CFLAGS , $(CXXFLAGS)) $(addprefix -LDFLAGS , $(LDFLAGS)) \
		--Mdir $(OBJ_DIR) --exe -o $(abspath $(BIN))


run: $(BIN)
	@$^ $(ARGS)

all:default
	@echo "Write this Makefile by your self."
	echo $(ARGS)



clean:rm -rf $(BUILD_DIR) 

include ../Makefile
 
.PHONY: default all clean run
