all:
	g++ -o test main.cc Keypad.h Keypad.cc PixelMap.h PixelMap.cc `sdl2-config --cflags --libs`