#include "Game.h"

Game::Game(const int& width, const int& height, const std::string& tilesetPath)
{
	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0 || SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
	}
	else if (TTF_Init() < 0) {
		std::cout << "TTF_Init: \n" << TTF_GetError() << '\n';
	}

	if (tilesetPath != "")
		_tileset = std::make_unique<TileSet>(tilesetPath, 12, 12);

	_renderer = std::make_unique<Renderer>("Gloom", width * _tileset->tileWidth , height * _tileset->tileHeight, RENDERER::ACCELERATED);
	_renderer->setDrawColor(BLACK);


	if (IMG_Init(IMG_INIT_PNG) < 0)
		std::cerr << "Failed to initialize png support\n";

	

	getTileSetTexture();

	_log = std::make_unique<MsgQueue>(15);
	_level = std::make_unique<Level>(mapWidth, mapHeight);
	_player = std::make_unique<Player>(0, 0);
}

Game::~Game()
{
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
}

void Game::getTileSetTexture()
{
	SDL_Texture* tileTex = _renderer->getTextureFromSurface(_tileset->image);
	_tileTexture = std::make_unique<Texture>(tileTex);
}

void Game::Run()
{
	while (_restart) {
		_renderer->Clear();
		_player->setName("Player");
		_level->Generate(*_player);



		Print();

		while (_running) {
			//handleInput();
			//EntitiesTurn();
			Print();

			if (!_player->isAlive())
				_running = false;
		}
	}
}

void Game::handleInput()
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
			else if (e.type = SDL_KEYDOWN) {

			}


			

			//const auto& zeroRect = _tileset->GetTileRect(ZERO);
		   // _tileTexture->Render(_renderer, zeroRect.x, zeroRect.y);

		}
	}
}

void Game::waitForKey(const SDL_KeyCode& key)
{
	SDL_Event e;
	bool quit = false;
	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.key.keysym.sym == key)
				quit = true;
		}
	}
}

void Game::Print()
{
	_renderer->Clear();
	SDL_Rect tileRect;
	tileRect.w = _tileset->tileWidth;
	tileRect.h = _tileset->tileHeight;

	if (!_seeEntireMap) {
		auto viewableTiles = tilesInView(*_player);

		for (int row = 0; row < _level->Height(); row++) {
			tileRect.y = row * tileRect.h;
			for (int col = 0; col < _level->Width(); col++) {
				tileRect.x = col * tileRect.w;
				
				if (Contains<Coord>(viewableTiles, { col, row })) {
					if (_level->getEntity(col, row).Char() != Tile::Uninitialized) {
						_renderer->Draw(_tileTexture->getTexture(), &_tileset->GetTileRect(_level->getEntity(col, row).Char()), &tileRect);
					}
					else if (_level->getContainer(col, row).Char() != Tile::Uninitialized) {
						_renderer->Draw(_tileTexture->getTexture(), &_tileset->GetTileRect(_level->getContainer(col, row).Char()), &tileRect);
					}
					else if (_level->getTile(col, row).Char() != Tile::Uninitialized) {
						_renderer->Draw(_tileTexture->getTexture(), &_tileset->GetTileRect(_level->getTile(col, row).Char()), &tileRect);
					}
					else {
						_renderer->Draw(_tileTexture->getTexture(), &_tileset->GetTileRect(Tile::Space), &tileRect);
					}
				}
			}
		}
	}
	else {
		for (int row = 0; row < _level->Height(); row++) {
			tileRect.y = row * tileRect.h;
			for (int col = 0; col < _level->Width(); col++) {
				tileRect.x = col * tileRect.w;
				if (_level->getEntity(col, row).Char() != Tile::Uninitialized) {
					_renderer->Draw(_tileTexture->getTexture(), &_tileset->GetTileRect(_level->getEntity(col, row).Char()), &tileRect);
				}
				else if (_level->getContainer(col, row).Char() != Tile::Uninitialized) {
					_renderer->Draw(_tileTexture->getTexture(), &_tileset->GetTileRect(_level->getContainer(col, row).Char()), &tileRect);
				}
				else if (_level->getTile(col, row).Char() != Tile::Uninitialized) {
					_renderer->Draw(_tileTexture->getTexture(), &_tileset->GetTileRect(_level->getTile(col, row).Char()), &tileRect);
				}
				else {
					_renderer->Draw(_tileTexture->getTexture(), &_tileset->GetTileRect(Tile::Space), &tileRect);
				}
			}
		}
	}

	_renderer->Update();
}

void Game::Help()
{
}

void Game::Tutorial()
{
}

void Game::Log()
{
}

void Game::Death()
{
}

void Game::Restart(bool restartFromDeath)
{
}

void Game::Victory()
{
}

void Game::Exit()
{
}

void Game::Inventory()
{
}

std::vector<Coord> Game::tilesInView(const Entity& entity)
{
	std::vector<Coord> viewableTiles;
	int entX = entity.X();
	int entY = entity.Y();
	int viewDist = entity.viewDistance();

	int distLeft = entX - viewDist;
	int distRight = entX + viewDist;
	int distUp = entY - viewDist;
	int distDown = entY + viewDist;

	if (distLeft < 0)	distLeft = 0;
	if (distRight > Game::mapWidth - 1)	distRight = Game::mapWidth - 1;
	if (distUp < 0)	distUp = 0;
	if (distDown > Game::mapHeight - 1)	distDown = Game::mapHeight - 1;

	for (int y = distUp; y <= distDown; y++) {
		for (int x = distLeft; x <= distRight; x++) {
			double distFromEntity = circleDistance(entity.X(), entity.Y(), x, y);

			if (distFromEntity + 0.5 <= viewDist)
				viewableTiles.emplace_back(x, y);
		}
	}
	return viewableTiles;
}

MoveAction Game::checkUp(const int& x, const int& y)
{
	char nextTile = _level->getTile(x, y - 1).Char();

	if (nextTile == Tile::Wall)	
		return MoveAction::Nothing;
	else {
		char nextEntity = _level->getEntity(x, y - 1).Char();
		if (nextEntity != Tile::Uninitialized) 
			return MoveAction::Attack;
		else	
			return MoveAction::Move;
	}
}

MoveAction Game::checkDown(const int& x, const int& y)
{
	char nextTile = _level->getTile(x, y + 1).Char();

	if (nextTile == Tile::Wall)
		return MoveAction::Nothing;
	else {
		char nextEntity = _level->getEntity(x, y + 1).Char();
		if (nextEntity != Tile::Uninitialized)
			return MoveAction::Attack;
		else
			return MoveAction::Move;
	}
}

MoveAction Game::checkLeft(const int& x, const int& y)
{
	char nextTile = _level->getTile(x - 1, y).Char();

	if (nextTile == Tile::Wall)
		return MoveAction::Nothing;
	else {
		char nextEntity = _level->getEntity(x - 1, y).Char();
		if (nextEntity != Tile::Uninitialized)
			return MoveAction::Attack;
		else
			return MoveAction::Move;
	}
}

MoveAction Game::checkRight(const int& x, const int& y)
{
	char nextTile = _level->getTile(x + 1, y).Char();

	if (nextTile == Tile::Wall)
		return MoveAction::Nothing;
	else {
		char nextEntity = _level->getEntity(x + 1, y).Char();
		if (nextEntity != Tile::Uninitialized)
			return MoveAction::Attack;
		else
			return MoveAction::Move;
	}
}

MoveAction Game::moveAction(const int& x, const int& y, const Direction& dir)
{
	switch (dir) {
		case Direction::Up:
			return checkUp(x, y);
		case Direction::Down:
			return checkDown(x, y);
		case Direction::Left:
			return checkLeft(x, y);
		case Direction::Right:
			return checkRight(x, y);
	}
}

void Game::Move(Entity& entity, const Direction& dir)
{
	int entX = entity.X(), 
		entY = entity.Y();

	if (dir == Direction::Up) {
		auto action = moveAction(entX, entY, Direction::Up);

		if (action == MoveAction::Move) {
			_level->swap(entity, Direction::Up);
		}
		else if (action == MoveAction::Attack) {
			Entity& entityAbove = _level->getEntity(entity.X(), entity.Y() - 1);
			if (entity.Char() == Tile::Player || entityAbove.Char() == Tile::Player) {
				auto result = entity.Attack(entityAbove, *_log);
				if (result.x > -1 && result.y > - 1)
					_level->reset(Map::Entity, result.x, result.y);
			}
		}
	}
	if (dir == Direction::Down) {
		auto action = moveAction(entX, entY, Direction::Down);

		if (action == MoveAction::Move) {
			_level->swap(entity, Direction::Down);
		}
		else if (action == MoveAction::Attack) {
			Entity& entityBelow = _level->getEntity(entity.X(), entity.Y() + 1);
			if (entity.Char() == Tile::Player || entityBelow.Char() == Tile::Player) {
				auto result = entity.Attack(entityBelow, *_log);
				if (result.x > -1 && result.y > - 1)
					_level->reset(Map::Entity, result.x, result.y);
			}
		}
	}
	if (dir == Direction::Left) {
		auto action = moveAction(entX, entY, Direction::Left);

		if (action == MoveAction::Move) {
			_level->swap(entity, Direction::Left);
		}
		else if (action == MoveAction::Attack) {
			Entity& entityLeft = _level->getEntity(entity.X() - 1, entity.Y());
			if (entity.Char() == Tile::Player || entityLeft.Char() == Tile::Player) {
				auto result = entity.Attack(entityLeft, *_log);
				if (result.x > -1 && result.y > -1)
					_level->reset(Map::Entity, result.x, result.y);
			}
		}
	}
	if (dir == Direction::Right) {
		auto action = moveAction(entX, entY, Direction::Right);

		if (action == MoveAction::Move) {
			_level->swap(entity, Direction::Right);
		}
		else if (action == MoveAction::Attack) {
			Entity& entityRight = _level->getEntity(entity.X() + 1, entity.Y());
			if (entity.Char() == Tile::Player || entityRight.Char() == Tile::Player) {
				auto result = entity.Attack(entityRight, *_log);
				if (result.x > -1 && result.y > - 1)
					_level->reset(Map::Entity, result.x, result.y);
			}
		}
	}

	Room* playerRoom = nullptr;
	for (auto& room : _level->getRooms()) {
		if (room.isInRoom(_player->X(), _player->Y())) {
			playerRoom = &room;
			_player->setCurrentRoom(&room);
		}
	}

	if (playerRoom == nullptr)
		_player->setCurrentRoom(nullptr);
	else {
		if (_player->currentRoom().isExit() && !_exitEntered) {
			//print message
		}
	}

}

void Game::EntitiesTurn()
{
	int act = 0, doNothing = 1;
	std::vector<int> decisions = { act, doNothing };
	std::vector<Direction> directions = { Direction::Up, Direction::Down, Direction::Left, Direction::Right };

	for (int y = 0; y < _level->Height(); y++) {
		for (int x = 0; x < _level->Width(); x++) {
			auto entity = _level->getEntity(x, y);
			if (entity.Char() != Tile::Uninitialized && entity.Char() != Tile::Player) {
				int decision = GENERATOR.Choice<int>(decisions);

				if (decision == 0) {
					Direction dir = GENERATOR.Choice<Direction>(directions);
					Move(entity, dir);
				}
			}
		}
	}
}

