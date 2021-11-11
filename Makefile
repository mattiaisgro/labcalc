default_target: all

.PHONY: all

all:
	g++ src/*.cpp -O2 -I./include/ -I./include/uroboro/ -o ./labcalc
