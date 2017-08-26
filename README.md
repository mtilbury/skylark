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
