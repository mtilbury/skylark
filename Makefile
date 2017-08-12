main:
	g++ -Wall -Werror -pedantic --std=c++11 -O1 cpu.cpp cpu.h main.cpp -o skylark.exe
	./skylark.exe

test:
	g++ -Wall -Werror -pedantic --std=c++11 -O1 cpu.cpp cpu.h test.cpp -o test

.PHONY: clean
clean:
		rm -vrf *.exe test
