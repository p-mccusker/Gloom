#include "Level.h"

std::vector<std::tuple<int, int>>  bresenham(int x1, int y1, int x2, int y2) {
	std::vector<std::tuple<int, int>> coords;
	int m_new = 2 * (y2 - y1);
	int slope_error_new = m_new - (x2 - x1);
	for (int x = x1, y = y1; x <= x2; x++)
	{
		// Add slope to increment angle formed
		slope_error_new += m_new;

		// Slope error reached limit, time to
		// increment y and update slope error.
		if (slope_error_new >= 0)
		{
			y++;
			slope_error_new -= 2 * (x2 - x1);
		}
		coords.emplace_back(x, y);
	}
	return coords;
}

Level::Level(const int& tilesWide, const int& tilesHigh)
{
	_width = tilesWide;
	_height = tilesHigh;
	_startRoom = _exitRoom = nullptr;

	for (int y = 0; y < tilesHigh; y++) {
		std::vector<std::unique_ptr<Tile>>      envline;
		std::vector<std::unique_ptr<Container>> cline;
		std::vector<std::unique_ptr<Entity>>    entline;

		for (int x = 0; x < tilesWide; x++) {
			envline.push_back(std::move(std::make_unique<Tile>(Tile::Uninitialized, x, y)));
			//cline.emplace_back(Tile::Uninitialized, x, y, nullptr);
			cline.push_back(std::move(std::make_unique<Container>(Tile::Uninitialized, x, y )));
			entline.push_back(std::move(std::make_unique<Entity>(Tile::Uninitialized, x, y)));
		}
		_envMap.push_back(std::move(envline));
		_containerMap.push_back(std::move(cline));
		_entityMap.push_back(std::move(entline));
	}
}

Level::~Level()
{
	_startRoom = _exitRoom = nullptr;
}

void Level::Generate(Player& player)
{
	generateRooms();
	generateHallways();
	placePlayer(player);
	placeExit();
	generateEnemies();
	generateContainers();
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
					_envMap[y][x]->setTile(Tile::Wall);
				else
					_envMap[y][x]->setTile(Tile::Floor);

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
		roomRadius = GENERATOR.randNum(Room::minRoomRadius, Room::maxRoomRadius);
		centerX = GENERATOR.randNum(roomRadius + 1, _width - roomRadius);
		centerY = GENERATOR.randNum(roomRadius + 1, _height - roomRadius);

		leftX = centerX - roomRadius;
		topY = centerY - roomRadius;
		rightX = leftX + roomRadius;
		bottomY = topY + roomRadius;

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
					_envMap[y][x]->setTile(Tile::Wall);
				else if (distance < roomRadius - 0.5)
					_envMap[y][x]->setTile(Tile::Floor);
				else
					_envMap[y][x]->setTile(Tile::Uninitialized);
			}
		}
		newRoom.Generate();
	}
	return newRoom;
}

std::vector<std::tuple<int, int>> Level::getHallwayPath(std::tuple<int, int> startPos, std::tuple<int, int> endPos)
{
	int startX = std::get<0>(startPos);
	int startY = std::get<1>(startPos);
	int endX = std::get<0>(endPos);
	int endY = std::get<1>(endPos);
	int cornerX, cornerY;
	if (GENERATOR.randNum(1, 2 + 1) == 1) {
		cornerX = endX;
		cornerY = startY;
	}
	else {
		cornerX = startX;
		cornerY = endY;
	}
	std::vector<std::tuple<int, int>> coords;
	for (auto& tuple : bresenham(startX, startY, cornerX, cornerY))
		coords.push_back(tuple);
	for (auto& tuple : bresenham(cornerX, cornerY, endX, endY))
		coords.push_back(tuple);

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

			if (roomProb <= 20)
				room = findCircleRoomLoc();
			else
				room = findRectRoomLoc();

			if (room.Shape() != RoomShape::Unititialized)
				_rooms.push_back(room);
			else {
				_rooms.clear();

				for (int y = 0; y < _height; y++) {
					for (int x = 0; x < _width; x++) {
						_envMap[y][x]->setTile(Tile::Uninitialized);
						_containerMap[y][x]->setTile(Tile::Uninitialized);
						_entityMap[y][x]->setTile(Tile::Uninitialized);
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
	int maxHallways = (int)_rooms.size() * 2;
	int numHallways = GENERATOR.randNum(maxHallways / 2, maxHallways + 1);
	int numZero = (int)_rooms.size();

	for (int hallNum = 0; hallNum < numHallways; hallNum++) {
		for (auto& room : _rooms) {
			if (room.getHallwayAmount() == 0) {
				Room& room2 = _rooms.at(GENERATOR.randNum(0, (int)_rooms.size()));

				while (&room == &room2|| room2.getHallwayAmount() == 2) {
					room2 = _rooms.at(GENERATOR.randNum(0, (int)_rooms.size()));
				}

				for (auto& tuple : getHallwayPath(room.Center(), room2.Center()))
					_envMap[std::get<1>(tuple)][std::get<0>(tuple)]->setTile(Tile::Floor);
				
				numZero -= (room2.getHallwayAmount() == 0) ? 2 : 1;

				room.incrementNumHallways();
				room2.incrementNumHallways();
				break;
			}
			else if (room.getHallwayAmount() == 1 && numZero == 0) {
				Room& room2 = _rooms.at(GENERATOR.randNum(0, (int)_rooms.size()));

				while (&room == &room2 || room2.getHallwayAmount() == 2) {
					room2 = _rooms.at(GENERATOR.randNum(0, (int)_rooms.size()));
				}

				for (auto& tuple : getHallwayPath(room.Center(), room2.Center()))
					_envMap[std::get<1>(tuple)][std::get<0>(tuple)]->setTile(Tile::Floor);

				room.incrementNumHallways();
				room2.incrementNumHallways();
				break;
			}
		}
	}

	placeHallwayWalls();
}

void Level::placeHallwayWalls()
{
	char tile, tileLeft, tileRight, tileUp, tileDown;

	for (size_t y = 0; y < _height; y++) {
		for (size_t x = 0; x < _width; x++) {
			tile = _envMap[y][x]->Char();

			if (tile == Tile::Floor) {
				tileLeft = _envMap[y][x - 1]->Char();
				tileRight = _envMap[y][x + 1]->Char();
				tileUp = _envMap[y - 1][x]->Char();
				tileDown = _envMap[y + 1][x]->Char();

				if (tileLeft == Tile::Uninitialized)
					_envMap[y][x - 1]->setTile(Tile::Wall);
				if (tileRight == Tile::Uninitialized)
					_envMap[y][x + 1]->setTile(Tile::Wall);
				if (tileUp == Tile::Uninitialized)
					_envMap[y - 1][x]->setTile(Tile::Wall);
				if (tileDown == Tile::Uninitialized)
					_envMap[y + 1][x]->setTile(Tile::Wall);
			}
		}
	}
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
						int centerX = std::get<0>(center);
						int centerY = std::get<1>(center);
						enemyX = GENERATOR.randNum(centerX - room.Radius() + 1, centerX + room.Radius());
						enemyY = GENERATOR.randNum(centerY - room.Radius() + 1, centerY + room.Radius());
						double distance = circleDistance(enemyX, enemyY, centerX, centerX);
						found = (distance < room.Radius() - 0.5) ? true : false;
					}
				}
				char enemyTile = GENERATOR.Choice<char>(possibleEnemies.data(), (int)possibleEnemies.size());
				_entityMap[enemyY][enemyX] = std::move(std::make_unique<Enemy>(enemyTile, enemyX, enemyY));
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
				enemyX = GENERATOR.randNum(std::get<0>(center) - room.Radius() + 1, std::get<0>(center) + room.Radius());
				enemyY = GENERATOR.randNum(std::get<1>(center) - room.Radius() + 1, std::get<1>(center) + room.Radius());
			}
			char enemyTile = GENERATOR.Choice<char>(possibleEnemies.data(), (int)possibleEnemies.size());
			_entityMap[enemyY][enemyX] = std::move(std::make_unique<Enemy>(enemyTile, enemyX, enemyY));
		}
	}
}

void Level::placePlayer(Player& player)
{
	Room& room = _rooms.at(GENERATOR.randNum(0, (int)_rooms.size()));
	auto center = room.Center();

	while (_envMap[std::get<1>(center)][std::get<0>(center)]->Char() != Tile::Floor) {
		room = _rooms.at(GENERATOR.randNum(0, (int)_rooms.size()));
		center = room.Center();
	}

	room.setPlayerStart();
	_startRoom = &room;

	_entityMap[std::get<1>(center)][std::get<0>(center)] = std::move(std::make_unique<Player>(std::get<0>(center), std::get<1>(center)));
	_entityMap[std::get<1>(center)][std::get<0>(center)]->setCurrentRoom(room);
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
	int startCenterX = std::get<0>(startCenter);
	int startCenterY = std::get<1>(startCenter);

	for (size_t i = 0; i < _rooms.size(); i++) {
		if (!_rooms[i].isPlayerStart()) {
			std::tuple<int, int> roomCenter = _rooms[i].Center();
			distances.emplace_back(i, circleDistance(startCenterX, startCenterY, std::get<0>(roomCenter), std::get<1>(roomCenter)));
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
	_envMap[std::get<1>(center)][std::get<0>(center)]->setTile(Tile::Stair_Up);

}

void Level::generateContainers()
{
	int numRooms = (int)_rooms.size();
	int numContainers = numRooms;

	for (auto& room : _rooms) {
		if (room.isPlayerStart() || room.isExit()) {
			auto center = room.Center();
			int centerX = std::get<0>(center);
			int centerY = std::get<1>(center);
			auto chestPos = center;
			int chestX = centerX;
			int chestY = centerY;
			if (room.Shape() == RoomShape::Rectangle) {
				while (chestX == centerX && chestY == centerY) {
					chestX = GENERATOR.randNum(room.startX() + 1, room.endX());
					chestY = GENERATOR.randNum(room.startY() + 1, room.endY());
				}
			}
			else if (room.Shape() == RoomShape::Circle) {
				while (chestX == centerX && chestY == centerY) {
					chestX = GENERATOR.randNum(centerX - room.Radius() + 1, centerX + room.Radius());
					chestY = GENERATOR.randNum(centerY - room.Radius() + 1, centerY + room.Radius());
				}
			}

			_containerMap[std::get<1>(chestPos)][std::get<0>(chestPos)] = std::move(std::make_unique<Container>(Tile::Chest, chestX, chestY, &room));
			(room.isPlayerStart()) ? _containerMap[std::get<1>(chestPos)][std::get<0>(chestPos)]->addItem(Potion(HEALING, MINOR)) 
									: _containerMap[std::get<1>(chestPos)][std::get<0>(chestPos)]->generateInventory();
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

			while (circleDistance(chestX, chestY, std::get<0>(room.Center()), std::get<1>(room.Center())) > room.Radius() - 0.5) {
				chestX = GENERATOR.randNum(room.startX() + 1, room.endX());
				chestY = GENERATOR.randNum(room.startY() + 1, room.endY());
			}
		}

		_containerMap[chestY][chestX] = std::move(std::make_unique<Container>(Tile::Chest, chestX, chestY, &room));
		_containerMap[chestY][chestX]->generateInventory();
		room.incrementNumHallways();
	}
}
