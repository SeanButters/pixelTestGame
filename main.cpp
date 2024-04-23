#include "globals.h"
#include <iostream> 


void generateWorld() {
	pixel temp;
	temp.valid = true;
	temp.rgb = 0x30303000;

	for (int y = 0; y < worldHeight; y++) {
		for (int x = 0; x < worldWidth; x++) {
			if (y < 10 || y > worldHeight - 11 || x < 10 || x > worldWidth - 11) {
				foreground[y][x] = temp;
			}
			else {
				foreground[y][x].valid = false;
			}
			background[y][x] = rgbMult(0xFCA51000, 1);			
		}
	}

}

/* Game Loop */

void handleEvents() {
	SDL_GetMouseState(&keys.mousex, &keys.mousey);													//get mouse locaiton
	keys.mousex = (keys.mousex / ((double)game.window_width  / camera.width)) + camera.x;		
	keys.mousey = (keys.mousey / ((double)game.window_height  / camera.height)) + camera.y;

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_QUIT:			// Exit application
			game.isRunning = false;
			break;
		case SDL_KEYDOWN:		// Key down
			if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w) {		 // Up / W
				keys.up = true;
			}
			else if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s) {	 // Down / S
				keys.down = true;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d) { // Right / D
				keys.right = true;
			}
			else if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a) {  // Left / A
				keys.left = true;
			}
			else if (event.key.keysym.sym == SDLK_l) {										 // Toggle lighting
				game.lighting = !game.lighting;
			}
			else if (event.key.keysym.sym == SDLK_c) {
				generateWorld();
			}
			break;
		case SDL_KEYUP:			// Key up
			if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w) {		 // Up / W
				keys.up = false;
			}
			else if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s) {	 // Down / S
				keys.down = false;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d) { // Right / D
				keys.right = false;
			}
			else if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a) {  // Left / A
				keys.left = false;
			}
			break;
		case SDL_MOUSEBUTTONDOWN: // Mouse button down
			if (event.button.button == SDL_BUTTON_LEFT) {		 // Left mouse button
				game.painting = true;
			}
			else if (event.button.button == SDL_BUTTON_RIGHT) {	 // Right mouse button
				game.unpainting = true;
			}
			break;
		case SDL_MOUSEBUTTONUP: // Mouse button up
			if (event.button.button == SDL_BUTTON_LEFT) {		 // Left mouse button
				game.painting = false;
			}
			else if (event.button.button == SDL_BUTTON_RIGHT) {	 // Right mouse button
				game.unpainting = false;
			}
			break;
		case SDL_MOUSEWHEEL:	//Mouse scroll wheel
			if (event.wheel.y > 0) {		//scroll up
				if (game.brushSize < 15) game.brushSize++;
				std::cout << game.brushSize << std::endl;
			}
			else if (event.wheel.y < 0) { //scroll down
				if (game.brushSize > 5) game.brushSize--;
				std::cout << game.brushSize << std::endl;
			}
			break;
		default:
			break;
		}
	}
}

void playerMove(){
	bool validMove = true;
	if (keys.up) {
		validMove = true;
		if (player.y - 1 < 0) validMove = false;
		else {
			for (int i = 0; i < player.size; i++) {
				if (foreground[player.y - 1][player.x + i].valid) {
					validMove = false;
					break;
				}
			}
		}

		if (validMove) player.y -= 1;
	}
	if (keys.down) {
		validMove = true;
		if (player.y + player.size + 1 > worldHeight - 1) validMove = false;
		else {
			for (int i = 0; i < player.size; i++) {
				if (foreground[player.y + player.size][player.x + i].valid) {
					validMove = false;
					break;
				}
			}
		}

		if (validMove) player.y += 1;
	}
	if (keys.right) {
		validMove = true;
		if (player.x + player.size + 1 > worldWidth - 1) validMove = false;
		else {
			for (int i = 0; i < player.size; i++) {
				if (foreground[player.y + i][player.x + player.size].valid) {
					validMove = false;
					break;
				}
			}
		}

		if (validMove) player.x += 1;
	}
	if (keys.left) {
		validMove = true;
		if (player.x - 1 < 0) validMove = false;
		else {
			for (int i = 0; i < player.size; i++) {
				if (foreground[player.y + i][player.x  - 1].valid) {
					validMove = false;
					break;
				}
			}
		}

		if (validMove) player.x -= 1;
	}

	camera.x = player.x - (camera.width / 2) + (player.size / 2);
	camera.y = player.y - (camera.height / 2) + (player.size / 2);
}

void paint() {
	if (game.painting) {
		pixel temp;
		temp.rgb = 0x40404000;
		temp.valid = true;
		for (int x = -(game.brushSize / 2); x < game.brushSize - (game.brushSize / 2); x++) {
			for (int y = -(game.brushSize / 2); y < game.brushSize - (game.brushSize / 2); y++) {
				int tempx = keys.mousex + x;
				int tempy = keys.mousey + y;
				if (tempy >= 0 && tempy < worldHeight && tempx >= 0 && tempx < worldWidth) foreground[keys.mousey + y][keys.mousex + x] = temp;
			}
		}
	}

	if (game.unpainting) {
		for (int x = -(game.brushSize / 2); x < game.brushSize - (game.brushSize / 2); x++) {
			for (int y = -(game.brushSize / 2); y < game.brushSize - (game.brushSize / 2); y++) {
				int tempx = keys.mousex + x;
				int tempy = keys.mousey + y;
				if (tempy >= 0 && tempy < worldHeight && tempx >= 0 && tempx < worldWidth) foreground[keys.mousey + y][keys.mousex + x].valid = false;;
			}
		}
	}
}

void update() {

	playerMove();
	paint();
}


bool validLine(int x0, int y0, int x1, int y1) {
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int x = x0;
	int y = y0;
	int n = 1 + dx + dy;
	int x_inc = (x1 > x0) ? 1 : -1;
	int y_inc = (y1 > y0) ? 1 : -1;
	int error = dx - dy;
	dx *= 2;
	dy *= 2;

	for (; n > 0; --n){
		if (foreground[y][x].valid) return false;

		if (error > 0){
			x += x_inc;
			error -= dy;
		}
		else{
			y += y_inc;
			error += dx;
		}
	}
	return true;
}


float raytrace(int x0, int y0, int x1, int y1) {

	float dist = calcDistance(x0, y0, x1, y1);
	if (validLine(x0, y0, x1, y1)) return inverseSquare(dist);
	return 0;
}


SDL_Window* window;						  // SDL2 window
SDL_Renderer* renderer;					  // SDL2 renderer
SDL_Texture* frame;					  // array of rgb pixel data

void render() {
	void* data;
	int pitch;
	SDL_LockTexture(frame, NULL, &data, &pitch);
		
	// clear to black background
	SDL_memset(data, 0, pitch * camera.height);

	// transfer pixel data
	int yPixel, xPixel;
	for (int y = 0; y < camera.height; y++){

		unsigned int*  row = (unsigned int*)((char*)data + pitch * y);

		for (int x = 0; x < camera.width; x++) {

			yPixel = y + camera.y;
			if (yPixel < 0 || yPixel > worldHeight - 1) { //out of bounds
				row[x] = 0x0;
				continue;
			}

			xPixel = x + camera.x;
			if (xPixel < 0 || xPixel > worldWidth - 1) { // out of bounds
				row[x] = 0x0;
				continue;
			}

			if (xPixel >= player.x && xPixel < player.x + player.size && yPixel >= player.y && yPixel < player.y + player.size) {
				row[x] = 0xBF600000;
				continue;
			}

			Uint32 p = foreground[yPixel][xPixel].valid ? foreground[yPixel][xPixel].rgb : background[yPixel][xPixel];

			if (game.lighting) {
				p = rgbMult(p, raytrace(player.x + (player.size/2), player.y + (player.size / 2), xPixel, yPixel));
			}
			row[x] = p;
		}
	}
		
	SDL_UnlockTexture(frame);

	// copy to window
	SDL_RenderCopy(renderer, frame, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void init(const char* title, int width, int height, bool fullscreen = false, bool hardwareAcceleration = false, bool vsync = false) {
	game.window_width = width;
	game.window_height = height;

	int wflags = 0;	// Window flags														
	if (fullscreen) wflags |= SDL_WINDOW_FULLSCREEN;				//enable fullscreen

	int rflags = 0; // renderer flags
	if (hardwareAcceleration) rflags |= SDL_RENDERER_ACCELERATED;	// enable hardware acceleration
	if (vsync) rflags |= SDL_RENDERER_PRESENTVSYNC;					// enable vsync which matches framerate to dislplay refresh rate

	// create sdl window and renderer
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, game.window_width, game.window_height, wflags);
	if (window) renderer = SDL_CreateRenderer(window, -1, rflags);
	if (renderer) frame = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBX8888, SDL_TEXTUREACCESS_STREAMING, camera.width, camera.height);
	if (frame) game.isRunning = true;

	//instantiate game variables
	game.lighting = false;
	game.painting = false;
	game.unpainting = false;
	game.brushSize = 10;

	generateWorld();
}

/*
*	Main method
*/
int main(int argc, char* argv[]) {
	// init
	if (SDL_Init(SDL_INIT_VIDEO) != 0) return -1;
	init("test", 1960, 1080, false, true, true);

	// fps data
	Uint32 start_time, frame_time;
	float fps;

	// game running
	while (game.isRunning) {
		start_time = SDL_GetTicks();

		// game loop
		handleEvents();
		update(); 
		render();

		// fps check
		frame_time = SDL_GetTicks() - start_time;
		fps = (frame_time > 0) ? 1000.0f / frame_time : 0.0f;

		//display data
		//std::cout << keys.mousex << ", " << keys.mousey<< std::endl;
		std::cout << fps << std::endl;
	}
	
	// exit
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyTexture(frame);
	SDL_Quit();

	return 0;
}