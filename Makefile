main:
	g++ -Wall -Werror -pedantic --std=c++11 -O1 src/cpu.cpp src/cpu.h src/main.cpp -lSDL2 -o skylark.exe

debug:
	g++ -Wall -Werror -pedantic --std=c++11 -O1 src/cpu.cpp src/cpu.h src/test.cpp -o test

.PHONY: clean
clean:
		rm -vrf *.exe test
		# find . -name 'test' -delete
		# find . -name '*.exe' -delete
