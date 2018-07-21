main:
	g++ -Wall -Werror -pedantic --std=c++11 -O1 src/CPU.cpp src/CPU.h src/Emulator.cpp src/Emulator.h src/main.cpp -lSDL2 -o skylark.exe

debug:
	g++ -Wall -Werror -pedantic --std=c++11 -O1 src/CPU.cpp src/CPU.h src/test.cpp -o test

.PHONY: clean
clean:
		rm -vrf *.exe test
		# find . -name 'test' -delete
		# find . -name '*.exe' -delete
