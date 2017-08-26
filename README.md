# CHIP-8 Skylark

<p>
CHIP-8 Skylark is an emulator/interpretor written in C++ for the CHIP-8 programming language.
Originally developed in the 70s, the CHIP-8 was a virtual machine meant to run
simple games for early microcomputers like the Telmac 1800.
</p>

## Getting Started

<p>
As of now, CHIP-8 Skylark has only been tested on Linux.
</p>

### Prerequisites

<p>
Because Skylark uses the SDL development library, you will need to install
it in order to run the emulator.
</p>

```
sudo apt-get install libsdl2-dev
```

### Compiling and Running

<p>
There is a Makefile included in this repo to help with compilation assuming
you have gcc/g++ installed. Just run "make" in bash in the skylark directory.
</p>

```
make
```
<p>
When you run the program, make sure to specify the ROM you'll be playing.
There are plenty of public domain ROMs online, but we'll use the included
demo ROM to test it out.
</p>

```
./skylark.exe demo.ch8
```

<img src="http://i.imgur.com/tOe8RmA.png">

<p>
At this splash screen, you can press the w key to progress to the next screen.
Here, you can use the wasd keys to move the smiley face around the screen.
</p>

### Controls

<p>
The CHIP-8 uses a 4x4 keypad to operate. This is mapped to the following keys on
a standard QWERTY keyboard:
</p>

|   |   |   |   |
|:-:|:-:|:-:|:-:|
| 1 | 2 | 3 | 4 |
| Q | W | E | R |
| A | S | D | F |
| Z | X | C | V |


### Debugging

<p>
As of now, the debugger has been lagging far behind the progress of the
actual emulator. It supports going through emulation one step at a time
and looking at the values of the registers, index, etc. Eventually I would like
it to include breakpoints and feature graphics (it's command line only now). To
compile the debugger, run "make debug".
</p>

```
make debug
```

<p>
Then run the debugger the same way you'd run the actual emulator.
</p>

```
./test demo.ch8
```
