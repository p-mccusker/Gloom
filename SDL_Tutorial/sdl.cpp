#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>

enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};


int main(int argc, char* argv[]) {
	
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cerr << "Failed to initialize SDL!\n";
		return -1;
	}

	IMG_Init(IMG_INIT_PNG);

	SDL_Window* win = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_RESIZABLE);
	SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	SDL_Surface* winSurf = SDL_GetWindowSurface(win);
	SDL_Surface* picture = IMG_Load("C:\\Users\\games\\source\\repos\\Gloom\\Gloom\\res\\taffer.png");
	if (picture == NULL) {
		printf("Unable to load image %s! SDL Error: %s\n", "C:\\Users\\games\\source\\repos\\Gloom\\Gloom\\res\\taffer.png", SDL_GetError());
	}

	SDL_Surface* KeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];

	/*
	SDL_Rect tile;
	tile.x = 0;
	tile.y = 60;
	tile.w = 20;
	tile.h = 20;
	
	SDL_Surface* tileSurf;
	*/
	SDL_Event e;
	bool quit = false;

	//SDL_BlitSurface(picture, &tile, winSurf, NULL);

	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT)
				quit = true;
		}

		SDL_UpdateWindowSurface(win);
		SDL_Delay(2000);

	}

	


	
	

	SDL_FreeSurface(picture);

	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}