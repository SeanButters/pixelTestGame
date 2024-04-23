#pragma once
#include "SDL.h"


Uint32 rgbMult(Uint32 rgb, float factor) {
	Uint8 r = ((rgb >> 24) & 0xFF) * factor;
	Uint8 g = ((rgb >> 16) & 0xFF) * factor;
	Uint8 b = ((rgb >> 8)  & 0xFF) * factor;
	return r << 24 | g << 16 | b << 16;
}

//Calculates the  squared distance betwee
float calcDistance(int x0, int y0, int x1, int y1) {
	return abs(((x1 - x0) * (x1 - x0)) + ((y1 - y0) * (y1 - y0)));
}

//Calculate invsere of distance
float inverseSquare(float dist) {
	//scale
	float val = 1 + (dist / 5000);

	//inverse square
	val = (1 / (val));

	if (val > 1.0) val = 1.0;
	if (val < 0.0) val = 0.0;
	return val;
}

//Hold pixel color data
struct pixel {
	bool valid;
	Uint32 rgb;

	pixel() {
		valid = false;
		rgb = 0x0;
	}
};


//Keeps track of currently held down butttons
struct keylog {
	bool left;
	bool right;
	bool up;
	bool down;
	int mousex, mousey;
};

const int worldWidth = 2000;
const int worldHeight = 2000;

struct playerObject {
	int x, y;
	const int size = 5;

	playerObject() {
		x = worldWidth / 2;
		y = worldHeight / 2;
	}
};

struct cameraObject {
	int x, y;
	const int width = 426;
	const int height = 240;

	cameraObject() {
		x = 0;
		y = 0;
	}
};


/* Game Variables */
struct gameVars {
	Uint16 window_width, window_height;		  // window resolution
	bool isRunning;							  // game window running
	bool lighting;							  // game lighting
	bool FPSshow;							  // FPS counter in console
	bool painting;
	bool unpainting;
	int brushSize;
};

gameVars game;
playerObject player;
cameraObject camera;
keylog keys;
pixel foreground[worldHeight][worldWidth]; // array of foreground pixels
Uint32 background[worldHeight][worldWidth]; // array of backgroudn pixels