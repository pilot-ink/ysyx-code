VERILATOR_FLAG = --cc --exe --build --trace


all:
	@echo "Write this Makefile by your self."

sim:
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	@echo "Write this Makefile by your self."
	verilator $(VERILATOR_FLAG) top.v main.cpp
wave:
	obj_dir/Vtop
	run gtkwave wave.vcd

clean:rm -rf obj_dir

include ../Makefile
