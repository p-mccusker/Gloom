#include <iostream>
#include <SDL.h>
#include "Game.h"

using namespace std;

int main(int argc, char* argv[])
{
	/*
	TextureLoader t(20,20, "C:\\Users\\games\\source\\repos\\Gloom\\Gloom\\res\\taffer.png");

	//SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* win = SDL_CreateWindow("Image Loader", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
	//SDL_Renderer* _renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	SDL_Surface* winSurf = SDL_GetWindowSurface(win);
   // SDL_Texture* textures[128];
	//SDL_Rect destRects[128];
	bool quit = false;
	/*
	for (int i = 0; i < 128; i++) {
		//SDL_Surface* tile = SDL_CreateRGBSurface(0, tileWidth, tileHeight, 8, rmask, gmask, bmask, amask);
		SDL_Rect newRect;
		newRect.x = (i % 16) * t.tileWidth;
		newRect.y = int((i / 16)) * t.tileHeight;
		newRect.w = t.tileWidth;
		newRect.h = t.tileHeight;
		destRects[i] = newRect;
		textures[i] = SDL_CreateTextureFromSurface(_renderer, t.getTileSurface((TexIndex)i));

		

		//SDL_BlitSurface(t._map[i], NULL, tile, &newRect);;
	}
	SDL_Event e;
	while (! quit) {
		//Handle events on queue
		while( SDL_PollEvent( &e ) != 0 )
		{
			//User requests quit
			if( e.type == SDL_QUIT )
			{
				quit = true;
			}

		}
	   // SDL_SetRenderDrawColor( _renderer, 0, 0, 0, 255 );
		//SDL_RenderClear( _renderer );

		SDL_BlitSurface(t.getTileSurface(ZERO), NULL, winSurf, NULL);
			//SDL_RenderCopy(_renderer, textures[i], NULL, &destRects[i]);

		SDL_UpdateWindowSurface(win);
		//SDL_RenderPresent(_renderer);
		SDL_Delay(50);

	}


   // SDL_DestroyRenderer(_renderer);
	//SDL_FreeSurface(tileset);

	SDL_Quit();
	*/

	Game* game = new Game(100, 100, "C:\\Users\\games\\source\\repos\\Gloom\\Gloom\\res\\Alloy_12x12_new.png");
	game->Run();


	//for (int i = 0; i < 50; i++)
	//std::cout << GENERATOR.Choice<int>(std::vector<int> {1, 2, 3, 4, 5, 6 }) << '\n';

	delete game;

	return 0;
}
