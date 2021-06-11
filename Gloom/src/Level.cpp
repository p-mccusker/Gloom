#include "Level.h"


Level::Level(const int& tilesWide, const int& tilesHigh)
{
	_width = tilesWide;
	_height = tilesHigh;
	_startRoom = _exitRoom = nullptr;

	_envMap.reserve(tilesHigh);
	_entityMap.reserve(tilesHigh);
	_containerMap.reserve(tilesHigh);

	for (int y = 0; y < tilesHigh; y++) {
		std::vector<Tile>      envline;
		std::vector<Container> cline;
		std::vector<Entity>    entline;

		envline.reserve(tilesWide);
		cline.reserve(tilesWide);
		entline.reserve(tilesWide);

		for (int x = 0; x < tilesWide; x++) {
			envline.emplace_back(Tile::Uninitialized, x, y);
			cline.emplace_back(Tile::Uninitialized, x, y );
			entline.emplace_back(Tile::Uninitialized, x, y);
		}
		_envMap.push_back(envline);
		_containerMap.push_back(cline);
		_entityMap.push_back(entline);
	}
}

Level::~Level()
{
	_startRoom = _exitRoom = nullptr;
}

void Level::Generate(Player& player)
{
	std::cout << "Generate()\n";
	generateRooms();
	//std::cout << "generateRooms()\n";
	//generateHallways();
	////std::cout << "generateHallways()\n";
	//placePlayer(player);
	//std::cout << "placePlayer()\n";
	//placeExit();
	//std::cout << "placeExit()\n";
	//generateEnemies();
	//std::cout << "placeEnemies()\n";
	//generateContainers();
	//std::cout << "generateContainers()\n";
}

Tile& Level::getTile(const int& x, const int& y)
{
	return _envMap[y][x];
}

Entity& Level::getEntity(const int& x, const int& y)
{
	return _entityMap[y][x];
}

Container& Level::getContainer(const int& x, const int& y)
{
	return _containerMap[y][x];
}

void Level::reset(const Map& map, const int& x, const int& y)
{
	if (map == Map::Tile)
		_envMap[y][x] = Tile(Tile::Uninitialized, x, y);
	else if (map == Map::Entity)
		_entityMap[y][x] = Entity(Tile::Uninitialized, x, y);
	else if (map == Map::Container)
		_containerMap[y][x] = Container(Tile::Uninitialized, x, y);
}

void Level::swap(Tile& t1,  const Direction& dir) {
	if (dir == Direction::Up) {
		_envMap[t1.Y() - 1][t1.X()].setY(t1.Y());
		t1.setY(t1.Y() - 1);
		std::swap(t1, _envMap[t1.Y() - 1][t1.X()]);
	}
	else if (dir == Direction::Down) {
		_envMap[t1.Y() + 1][t1.X()].setY(t1.Y());
		t1.setY(t1.Y() + 1);
		std::swap(t1, _envMap[t1.Y() + 1][t1.X()]);
	}
	else if (dir == Direction::Left) {
		_envMap[t1.Y()][t1.X() - 1].setX(t1.X());
		t1.setX(t1.X() - 1);
		std::swap(t1, _envMap[t1.Y()][t1.X() - 1]);
	}
	else if (dir == Direction::Right) {
		_envMap[t1.Y()][t1.X() + 1].setY(t1.X());
		t1.setY(t1.X() + 1);
		std::swap(t1, _envMap[t1.Y()][t1.X() + 1]);
	}
}

void Level::swap(Entity& t1, const Direction& dir) {
	if (dir == Direction::Up) {
		_entityMap[t1.Y() - 1][t1.X()].setY(t1.Y());
		t1.setY(t1.Y() - 1);
		std::swap(t1, _entityMap[t1.Y() - 1][t1.X()]);
	}
	else if (dir == Direction::Down) {
		_entityMap[t1.Y() + 1][t1.X()].setY(t1.Y());
		t1.setY(t1.Y() + 1);
		std::swap(t1, _entityMap[t1.Y() + 1][t1.X()]);
	}
	else if (dir == Direction::Left) {
		_entityMap[t1.Y()][t1.X() - 1].setX(t1.X());
		t1.setX(t1.X() - 1);
		std::swap(t1, _entityMap[t1.Y()][t1.X() - 1]);
	}
	else if (dir == Direction::Right) {
		_entityMap[t1.Y()][t1.X() + 1].setY(t1.X());
		t1.setY(t1.X() + 1);
		std::swap(t1, _entityMap[t1.Y()][t1.X() + 1]);
	}
}

void Level::swap(Container& t1, const Direction& dir) {
	if (dir == Direction::Up) {
		_containerMap[t1.Y() - 1][t1.X()].setY(t1.Y());
		t1.setY(t1.Y() - 1);
		std::swap(t1, _containerMap[t1.Y() - 1][t1.X()]);
	}
	else if (dir == Direction::Down) {
		_containerMap[t1.Y() + 1][t1.X()].setY(t1.Y());
		t1.setY(t1.Y() + 1);
		std::swap(t1, _containerMap[t1.Y() + 1][t1.X()]);
	}
	else if (dir == Direction::Left) {
		_containerMap[t1.Y()][t1.X() - 1].setX(t1.X());
		t1.setX(t1.X() - 1);
		std::swap(t1, _containerMap[t1.Y()][t1.X() - 1]);
	}
	else if (dir == Direction::Right) {
		_containerMap[t1.Y()][t1.X() + 1].setY(t1.X());
		t1.setY(t1.X() + 1);
		std::swap(t1, _containerMap[t1.Y()][t1.X() + 1]);
	}
}

Room Level::findRectRoomLoc()
{
	bool overlap = false;
	bool locationFound = false;
	int numTries = 0;
	int leftX = 0, rightX = 0, topY = 0, bottomY = 0;
	
	Room newRoom;

	while (!locationFound && numTries < 150) {

		int roomWidth = GENERATOR.randNum(Room::minRoomWidth, Room::maxRoomWidth + 1);
		int roomHeight = GENERATOR.randNum(Room::minRoomHeight, Room::maxRoomHeight + 1);

		leftX = GENERATOR.randNum(1, (_width - roomWidth));
		topY = GENERATOR.randNum(1, (_height - roomHeight));

		rightX = leftX + roomWidth;
		bottomY = topY + roomHeight;

		newRoom = Room(leftX, topY, roomWidth, roomHeight);

		for (auto& room : _rooms) {
			if (newRoom.Intersect(room))
				overlap = true;
		}
		if (!overlap)
			locationFound = true;
		else {
			newRoom = Room();
		}
		numTries++;
	}
	if (locationFound) {
		for (int y = topY; y <= bottomY; y++) {
			for (int x = leftX; x <= rightX; x++) {
				if (y == topY || y == bottomY || x == leftX || x == rightX)
					_envMap[y][x].setTile(Tile::Wall);
				else
					_envMap[y][x].setTile(Tile::Floor);
			}
		}
		newRoom.Generate();
	}
	return std::move(newRoom);
}

Room Level::findCircleRoomLoc()
{
	bool overlap = false;
	bool locationFound = false;
	int numTries = 0;
	int roomRadius = 0;
	int centerX = 0, centerY = 0;
	int leftX = 0, rightX = 0, topY = 0, bottomY = 0;

	Room newRoom;

	while (!locationFound && numTries < 150) {
		roomRadius = GENERATOR.randNum(Room::minRoomRadius, Room::maxRoomRadius + 1);
		centerX = GENERATOR.randNum(roomRadius + 1, _width - roomRadius);
		centerY = GENERATOR.randNum(roomRadius + 1, _height - roomRadius);

		leftX = centerX - roomRadius;
		topY = centerY - roomRadius;
		rightX = centerX + roomRadius;
		bottomY = centerY + roomRadius;

		newRoom = Room(centerX, centerY, roomRadius); 

		for (auto& room : _rooms) {
			if (newRoom.Intersect(room))
				overlap = true;
		}
		if (!overlap)
			locationFound = true;
		else {
			newRoom = Room();
		}

		numTries++;
	}
	if (locationFound) {
		for (int y = topY; y <= bottomY; y++) {
			for (int x = leftX; x <= rightX; x++) {
				double distance = circleDistance(x, y, centerX, centerY);
				if (distance > roomRadius - 0.5 && distance < roomRadius + 0.5)
					_envMap[y][x].setTile(Tile::Wall);
				else if (distance < roomRadius - 0.5)
					_envMap[y][x].setTile(Tile::Floor);
				else
					_envMap[y][x].setTile(Tile::Uninitialized);
			}
		}
		newRoom.Generate();
	}
	return newRoom;
}

std::vector<Coord> Level::getHallwayPath(const Coord& start, const Coord& end)
{
	std::cout << "getHallwayPath()\n";

	int cornerX, cornerY;
	if (GENERATOR.randNum(1, 2 + 1) == 1) {
		cornerX = end.x;
		cornerY = start.y;
	}
	else {
		cornerX = start.x;
		cornerY = end.y;
	}
	std::vector<Coord> coords;
	for (auto& coord : bresenham(start.x, start.y, cornerX, cornerY))
		coords.push_back(coord);
	for (auto& coord : bresenham(cornerX, cornerY, end.x, end.y))
		coords.push_back(coord);

	std::cout << "~getHallwayPath()\n";

	return coords;
}

void Level::generateRooms()
{
	std::cout << "\t\t\t\tGenerating Map...\n";

	bool overlap = true;

	while (overlap) {
		int numRooms = GENERATOR.randNum(minRooms, maxRooms);

		for (int i = 0; i < numRooms; i++) {
			Room room;
			int roomProb = GENERATOR.randNum(1, 101);

			if (roomProb <= 33)
				room = findCircleRoomLoc();
			else
				room = findRectRoomLoc();

			if (room.Shape() != RoomShape::Unititialized)
				_rooms.push_back(room);
			else {
				_rooms.clear();

				for (int y = 0; y < _height; y++) {
					for (int x = 0; x < _width; x++) {
						_envMap[y][x].setTile(Tile::Uninitialized);
						_containerMap[y][x].setTile(Tile::Uninitialized);
						_entityMap[y][x].setTile(Tile::Uninitialized);
					}
				}
			}
		}
		if (_rooms.size() == numRooms)
			overlap = false;
	}
}

void Level::generateHallways()
{
	std::cout << "generateHallways()\n";
	int maxHallways = (int)_rooms.size() * 2;
	int numHallways = GENERATOR.randNum(maxHallways / 2, maxHallways + 1);
	int numZero = (int)_rooms.size();

	if (_rooms.size() <= 1)
		return;

	for (int hallNum = 0; hallNum < numHallways; hallNum++) {
		for (auto& room : _rooms) {
			if (room.getHallwayAmount() == 0) {
				Room room2 = _rooms.at(GENERATOR.randNum(0, (int)_rooms.size()));

				while (room == room2 || room2.getHallwayAmount() == 2) {
					std::cout << "generateHallways(): while: 1\n";
					std::cout << "Room1: " << room.getHallwayAmount() << " Room2: " << room2.getHallwayAmount() << '\n';
					room2 = _rooms.at(GENERATOR.randNum(0, (int)_rooms.size()));
				}

				for (auto& coord : getHallwayPath(room.Center(), room2.Center()))
					_envMap[coord.y][coord.x].setTile(Tile::Floor);
				
				numZero -= (room2.getHallwayAmount() == 0) ? 2 : 1;

				room.incrementNumHallways();
				room2.incrementNumHallways();
				break;
			}
			else if (room.getHallwayAmount() == 1 && numZero == 0) {
				Room room2 = _rooms.at(GENERATOR.randNum(0, (int)_rooms.size()));

				while (room == room2 || room2.getHallwayAmount() == 2) {
					std::cout << "generateHallways(): while: 2\n";
					std::cout << "Room1: " << room.getHallwayAmount() << " Room2: " << room2.getHallwayAmount() << '\n';
					room2 = _rooms.at(GENERATOR.randNum(0, (int)_rooms.size()));
				}

				for (auto& coord : getHallwayPath(room.Center(), room2.Center()))
					_envMap[coord.y][coord.x].setTile(Tile::Floor);

				room.incrementNumHallways();
				room2.incrementNumHallways();
				break;
			}
		}
	}
	placeHallwayWalls();
	std::cout << "~generateHallways()\n";
}

void Level::placeHallwayWalls()
{
	std::cout << "placeHallwayWalls()\n";
	Tile tile, tileLeft, tileRight, tileUp, tileDown;

	for (size_t y = 0; y < _height; y++) {
		for (size_t x = 0; x < _width; x++) {
			tile = _envMap[y].at(x);

			if (tile.Char() == Tile::Floor) {
				tileLeft = _envMap[y].at(x - 1);
				tileRight = _envMap[y].at(x + 1);
				tileUp = _envMap[y - 1].at(x);
				tileDown = _envMap[y + 1].at(x);

				if (tileLeft.Char() == Tile::Uninitialized)
					tileLeft.setTile(Tile::Wall);
				if (tileRight.Char() == Tile::Uninitialized)
					tileRight.setTile(Tile::Wall);
				if (tileUp.Char() == Tile::Uninitialized)
					tileUp.setTile(Tile::Wall);
				if (tileDown.Char() == Tile::Uninitialized)
					tileDown.setTile(Tile::Wall);
			}
		}
	}
	std::cout << "~placeHallwayWall()\n";
}

void Level::generateEnemies()
{

	for (int i = 0; i < _rooms.size(); i++) {
		Room& room = _rooms.at(i);
		int numEnemies = 0;
		if (!room.isExit() && !room.isPlayerStart()) {
			if (room.Size() == RoomSize::Small)
				numEnemies = 1;
			else
				numEnemies = GENERATOR.randNum(2, 4);

			auto possibleEnemies = LevelList::getEnemyLists(room.Hardness());

			for (int enem = 0; enem < numEnemies; enem++) {
				int enemyX = 0, enemyY = 0;

				if (room.Shape() == RoomShape::Rectangle) {
					enemyX = GENERATOR.randNum(room.startX() + 1, room.endX());
					enemyY = GENERATOR.randNum(room.startY() + 1, room.endY());
				}
				else if (room.Shape() == RoomShape::Circle) {
					bool found = false;

					while (!found) {
						auto center = room.Center();

						enemyX = GENERATOR.randNum(center.x - room.Radius() + 1, center.x + room.Radius());
						enemyY = GENERATOR.randNum(center.y - room.Radius() + 1, center.y + room.Radius());
						double distance = circleDistance(enemyX, enemyY, center.x, center.y);
						found = (distance < room.Radius() - 0.5) ? true : false;
					}
				}
				char enemyTile = GENERATOR.Choice<char>(possibleEnemies);
				_entityMap[enemyY][enemyX] = Enemy(enemyTile, enemyX, enemyY);
			}
		}
		else if (room.isPlayerStart())
			room.setHardness(RoomHardness::Empty);
		
		else if (room.isExit()) {
			room.setHardness(RoomHardness::Boss);
			auto possibleEnemies = LevelList::getEnemyLists(room.Hardness());
			int enemyX=0, enemyY=0;
			if (room.Shape() == RoomShape::Rectangle) {
				enemyX = GENERATOR.randNum(room.startX() + 1, room.endX());
				enemyY = GENERATOR.randNum(room.startY() + 1, room.endY());
			}
			else if (room.Shape() == RoomShape::Circle) {
				auto center = room.Center();
				enemyX = GENERATOR.randNum(center.x - room.Radius() + 1, center.x + room.Radius());
				enemyY = GENERATOR.randNum(center.y - room.Radius() + 1, center.y + room.Radius());
			}
			char enemyTile = GENERATOR.Choice<char>(possibleEnemies);
			_entityMap[enemyY][enemyX] = Enemy(enemyTile, enemyX, enemyY);
		}
	}
}

void Level::placePlayer(Player& player)
{
	Room& room = _rooms.at(GENERATOR.randNum(0, (int)_rooms.size()));
	auto center = room.Center();

	while (_envMap[center.y][center.x].Char() != Tile::Floor) {
		room = _rooms.at(GENERATOR.randNum(0, (int)_rooms.size()));
		center = room.Center();
	}

	room.setPlayerStart();
	_startRoom = &room;

	_entityMap[center.y][center.x] = Player(center.x, center.y);
	_entityMap[center.y][center.x].setCurrentRoom(&room);
}

void Level::placeExit()
{
	std::vector<std::tuple<int,double>> distances;

	if (_startRoom == nullptr) {
		for (size_t i = 0; i < _rooms.size(); i++) {
			if (_rooms[i].isPlayerStart())
				_startRoom = &_rooms[i];
		}
	}
	
	auto startCenter = _startRoom->Center();

	for (size_t i = 0; i < _rooms.size(); i++) {
		if (!_rooms[i].isPlayerStart()) {
			Coord roomCenter = _rooms[i].Center();
			distances.emplace_back(i, circleDistance(startCenter.x, startCenter.y, roomCenter.x, roomCenter.y));
		}
	}
	double maxDistance = 0.0;
	int farthestRoomIndex = 0;

	for (auto& tuple : distances) {
		if (std::get<1>(tuple) > maxDistance) {
			maxDistance = std::get<1>(tuple);
			farthestRoomIndex = std::get<0>(tuple);
		}
	}

	Room& farthestRoom = _rooms[farthestRoomIndex];
	farthestRoom.setLevelExit();
	_exitRoom = &farthestRoom;
	auto center = _exitRoom->Center();
	_envMap[center.y][center.x].setTile(Tile::Stair_Up);

}

void Level::generateContainers()
{
	int numRooms = (int)_rooms.size();
	int numContainers = numRooms;

	for (auto& room : _rooms) {
		if (room.isPlayerStart() || room.isExit()) {
			auto center = room.Center();
			auto chestPos = center;

			int chestX = center.x;
			int chestY = center.y;
			if (room.Shape() == RoomShape::Rectangle) {
				while (chestX == center.x && chestY == center.y) {
					chestX = GENERATOR.randNum(room.startX() + 1, room.endX());
					chestY = GENERATOR.randNum(room.startY() + 1, room.endY());
				}
			}
			else if (room.Shape() == RoomShape::Circle) {
				while (chestX == center.x && chestY == center.y) {
					chestX = GENERATOR.randNum(center.x - room.Radius() + 1, center.x + room.Radius());
					chestY = GENERATOR.randNum(center.y - room.Radius() + 1, center.y + room.Radius());
				}
			}

			_containerMap[chestPos.y][chestPos.x] = Container(Tile::Chest, chestX, chestY, &room);
			(room.isPlayerStart()) ? _containerMap[chestPos.y][chestPos.x].addItem(Potion(HEALING, MINOR)) 
									: _containerMap[chestPos.y][chestPos.x].generateInventory();
			room.incrementNumHallways();
		}
	}

	for (int i = 0; i < (numContainers - 2); i++) {
		Room& room = _rooms.at(GENERATOR.randNum(0, (int)_rooms.size()));

		while (room.isPlayerStart() || room.isExit() || room.getContainerAmount() > 0)
			room = _rooms[GENERATOR.randNum(0, (int)_rooms.size())];

		int chestX=0, chestY=0;

		if (room.Shape() == RoomShape::Rectangle) {
			chestX = GENERATOR.randNum(room.startX() + 1, room.endX());
			chestY = GENERATOR.randNum(room.startY() + 1, room.endY());
		}
		else if (room.Shape() == RoomShape::Circle) {
			chestX = GENERATOR.randNum(room.startX() + 1, room.endX());
			chestY = GENERATOR.randNum(room.startY() + 1, room.endY());
			auto roomCenter = room.Center();

			while (circleDistance(chestX, chestY, roomCenter.x, roomCenter.y) > room.Radius() - 0.5) {
				chestX = GENERATOR.randNum(room.startX() + 1, room.endX());
				chestY = GENERATOR.randNum(room.startY() + 1, room.endY());
			}
		}
		_containerMap[chestY][chestX] = Container(Tile::Chest, chestX, chestY, &room);
		_containerMap[chestY][chestX].generateInventory();
		room.incrementNumHallways();
	}
}
