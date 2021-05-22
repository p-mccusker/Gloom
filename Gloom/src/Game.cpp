#include "Game.h"

Game::Game(const int& width, const int& height, const std::string& tilesetPath)
{
    _log = new MsgQueue(15);
    _level = new Level(mapWidth, mapHeight);

    if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0 || SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
    }
    else {
        _renderer = new Renderer("Gloom", width, height, RENDERER::ACCELERATED);
	    _renderer->setDrawColor(BLACK);
    }

    if (IMG_Init(IMG_INIT_PNG) < 0)
        std::cerr << "Failed to initialize png support\n";

    if (tilesetPath != "")
        _tileset = new TileSet(tilesetPath, 20, 20);

    getTileSetTexture();
}

Game::~Game()
{
	delete _renderer;
	_renderer = nullptr;
    delete _tileset;
    _tileset = nullptr;
    delete _level;
    _level = nullptr;
    delete _tileTexture;
    _tileTexture = nullptr;
    delete _log;
    _log = nullptr;

	SDL_QuitSubSystem(SDL_INIT_EVENTS);
}

void Game::Run()
{
    bool quit = false;

    SDL_Event e;
    while (!quit) {
        //Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            //User requests quit
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            _renderer->Clear();
            for (int i = 0; i < 128; i++)
                _renderer->Draw(_tileTexture->getTexture(), &_tileset->GetTileRect((TexIndex)i), &_tileset->GetTileRect((TexIndex)i));

            _renderer->Update();

            //const auto& zeroRect = _tileset->GetTileRect(ZERO);
           // _tileTexture->Render(_renderer, zeroRect.x, zeroRect.y);

        }
    }

}

void Game::getTileSetTexture()
{
    SDL_Texture* tileTex = _renderer->getTextureFromSurface(_tileset->image);
    _tileTexture = new Texture(tileTex);
}

