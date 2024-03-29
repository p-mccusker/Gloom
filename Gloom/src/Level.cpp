#include "Level.h"


Level::Level(const int& tilesWide, const int& tilesHigh, const int& tileWidth, const int& tileHeight)
{
	_width = tilesWide;
	_height = tilesHigh;
	_tileWidth = tileWidth;
	_tileHeight = tileHeight;
	_startRoom = _exitRoom = nullptr;

	_envMap.reserve(tilesHigh);
	_entityMap.reserve(tilesHigh * tilesWide);
	_containerMap.reserve(tilesHigh * tilesWide);

	for (int y = 0; y < tilesHigh; y++) {
		std::vector<Tile> line;
		line.reserve(tilesWide);
		for (int x = 0; x < tilesWide; x++) {
			line.emplace_back(Tile::Uninitialized, x * tileWidth , y * tileHeight);
			_containerMap.emplace_back(Tile::Uninitialized, x * tileWidth, y * tileHeight);
			_entityMap.emplace_back(Tile::Uninitialized, x * tileWidth, y * tileHeight);
		}
		_envMap.push_back(line);
	}
}

Level::~Level()
{
	_startRoom = _exitRoom = nullptr;
}

void Level::Generate(Entity* player)
{
	std::cout << "Generate()\n";
	//generateRooms();
	generateRoomsBSP();
	std::cout << "generateRoomsBSP()\n";
	generateHallways();
	std::cout << "generateHallways()\n";
	placePlayer(player);
	std::cout << "placePlayer()\n";
	placeExit();
	std::cout << "placeExit()\n";
	generateEnemies();
	std::cout << "placeEnemies()\n";
	generateContainers();
	std::cout << "generateContainers()\n";
}

Tile& Level::getTile(const int& x, const int& y)
{
	return _envMap.at(y).at(x);
}

Entity& Level::getEntity(const int& index)
{
	return _entityMap.at(index);
}

Container& Level::getContainer(const int& index)
{
	return _containerMap.at(index);
}

void Level::reset(const Map& map, const int& index)
{
	int x, y;
	if (map == Map::Tile) {
		Tile oldTile = _envMap[index];
		x = oldTile.X();
		y = oldTile.Y();
		_envMap.at(index) = Tile(Tile::Uninitialized, x, y);
	}
	else if (map == Map::Entity) {
		Entity oldEntity = _entityMap[index];
		x = oldEntity.X();
		y = oldEntity.Y();
		_entityMap.at(index) = Entity(Tile::Uninitialized, x, y);
	}
	else if (map == Map::Container) {
		Container oldContainer = _containerMap[index];
		x = oldContainer.X();
		y = oldContainer.Y();
		_containerMap.at(index) = Container(Tile::Uninitialized, x, y);
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

		int roomWidth = GENERATOR.randNum(Room::minRoomWidth, Room::maxRoomWidth + 1) * _tileWidth;
		int roomHeight = GENERATOR.randNum(Room::minRoomHeight, Room::maxRoomHeight + 1) * _tileHeight;

		leftX = GENERATOR.randNum(1, (_width - roomWidth)) * _tileWidth;;
		topY = GENERATOR.randNum(1, (_height - roomHeight)) * _tileHeight;

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
		for (auto& tile : _envMap) {
			
			if (tile.Y() >= topY && tile.Y() <= bottomY && tile.X() >= leftX && tile.X() <= rightX) {
				//If tile is on the edge, make it a wall, might be broke
				if (tile.Y() == topY || tile.Y() == bottomY || tile.X() == leftX || tile.X() == rightX)
					tile.setTile(Tile::Wall);
				//if tile is within room bounds, make it a floor
				else if (tile.Y() > topY && tile.Y() < bottomY && tile.X() > leftX && tile.X() < rightX)
					tile.setTile(Tile::Floor);
			}
		}

		newRoom.Generate();
	}
	return newRoom;
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
		roomRadius = GENERATOR.randNum(Room::minRoomRadius, Room::maxRoomRadius + 1) * _tileWidth;
		centerX = GENERATOR.randNum(roomRadius + 1, _width - roomRadius) * _tileWidth;
		centerY = GENERATOR.randNum(roomRadius + 1, _height - roomRadius) * _tileHeight;

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
		for (auto& tile : _envMap) {
			double distance = circleDistance(tile.X(), tile.Y(), centerX, centerY);
			if (tile.Y() >= topY && tile.Y() <= bottomY && tile.X() >= leftX && tile.X() <= rightX) {
				if (distance > roomRadius - 0.5 && distance < roomRadius + 0.5)
					tile.setTile(Tile::Wall);
				else if (distance < roomRadius - 0.5)
					tile.setTile(Tile::Floor);
				else
					tile.setTile(Tile::Uninitialized);
			}
		}
		newRoom.Generate();
	}
	return newRoom;
}

Room Level::findRectRoomLocBSP(const Coord& topLeft, const Coord& bottomRight)
{
	std::cout << "findRectRoomLocBSP()\n";
	bool overlap = false;
	bool locationFound = false;
	int leftX = 0, rightX = 0, topY = 0, bottomY = 0;

	Room newRoom;
	//Set roomDims to a size too large for the current 
	int roomWidth = 0;
	int roomHeight = 0;

	const int maxRoomWidth =  ((bottomRight.x - topLeft.x) - 2 > Room::maxRoomWidth)  ? Room::maxRoomWidth * _tileWidth   
																					  :	((bottomRight.x - topLeft.x) - 2) * _tileWidth;
	const int maxRoomHeight = ((bottomRight.y - topLeft.y) - 2 > Room::maxRoomHeight) ? Room::maxRoomHeight * _tileHeight 
																					  : ((bottomRight.y - topLeft.y) - 2) * _tileHeight;

	while (!locationFound) {
		// make sure room can fit in side the current dimension
		do {
			roomWidth = GENERATOR.randNum(Room::minRoomWidth, maxRoomWidth + 1) * _tileWidth;
			roomHeight = GENERATOR.randNum(Room::minRoomHeight, maxRoomHeight + 1) * _tileHeight;
			std::cout << "Width: " << roomWidth << " Height: " << roomHeight << '\n';
		} while (roomWidth > maxRoomWidth && roomHeight > maxRoomHeight);

		leftX = GENERATOR.randNum(topLeft.x + 1, (bottomRight.x - roomWidth));
		topY = GENERATOR.randNum(topLeft.y + 1, (bottomRight.y - roomHeight));

		rightX = leftX + roomWidth;
		bottomY = topY + roomHeight;

		newRoom = Room(leftX, topY, roomWidth, roomHeight);

		for (auto& room : _rooms) {
			if (newRoom.Intersect(room))
				overlap = true;
		}

		if (!overlap)
			locationFound = true;
		else 
			newRoom = Room();
	}

	std::cout << "Rect Room: (" << leftX << ',' << topY << ") : (" << rightX << ',' << bottomY << ")\n";

	//Inscribe room onto tile map
	for (auto& tile : _envMap) {
		if (tile.Y() >= topY && tile.Y() <= bottomY && tile.X() >= leftX && tile.X() <= rightX) {
			//If tile is on the edge, make it a wall, might be broke
			if (tile.Y() == topY || tile.Y() == bottomY || tile.X() == leftX || tile.X() == rightX)
				tile.setTile(Tile::Wall);
			//if tile is within room bounds, make it a floor
			else if (tile.Y() > topY && tile.Y() < bottomY && tile.X() > leftX && tile.X() < rightX)
				tile.setTile(Tile::Floor);
		}
	}
	newRoom.Generate();
	
	return newRoom;
}

Room Level::findCircleRoomLocBSP(const Coord& topLeft, const Coord& bottomRight)
{
	std::cout << "findCircleRoomLocBSP()\n";
	bool overlap = false;
	bool locationFound = false;
	//Find the shortest dimension (x or y) to prevent creating a radius too large for the space
	int shortestCoord = std::min((bottomRight.x - topLeft.x), (bottomRight.y - topLeft.y));
	const int maxRadius =  ((shortestCoord - 2) / 2 > Room::maxRoomRadius) ? Room::maxRoomRadius * _tileWidth : ((shortestCoord - 2) / 2) * _tileWidth;
	int roomRadius = 0;

	int centerX = 0, centerY = 0;
	int leftX = 0, rightX = 0, topY = 0, bottomY = 0;

	Room newRoom;

	while (!locationFound) {
		// make sure room can fit in side the current dimension
		do {
			roomRadius = GENERATOR.randNum(Room::minRoomRadius, maxRadius + 1) * _tileWidth;
			std::cout << "Radius: " << roomRadius << '\n';
		} while (roomRadius > maxRadius);

		centerX = GENERATOR.randNum(topLeft.x + roomRadius + 1, bottomRight.x - roomRadius + 1) * _tileWidth;
		centerY = GENERATOR.randNum(topLeft.y + roomRadius + 1, bottomRight.y - roomRadius + 1) * _tileHeight;

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
		else
			newRoom = Room();
	}
	//Inscribe room onto tile map
	for (auto& tile : _envMap) {
		double distance = circleDistance(tile.X(), tile.Y(), centerX, centerY);
		if (tile.Y() >= topY && tile.Y() <= bottomY && tile.X() >= leftX && tile.X() <= rightX) {
			if (distance > roomRadius - 0.5 && distance < roomRadius + 0.5)
				tile.setTile(Tile::Wall);
			else if (distance < roomRadius - 0.5)
				tile.setTile(Tile::Floor);
			else
				tile.setTile(Tile::Uninitialized);
		}
	}
	newRoom.Generate();
	return newRoom;
}

void Level::generateRoomsBSP()
{
	//int numIters = numRooms;
	//Decrement the total number of rooms until it is possible to place all the rooms, based off the min room dimensions
	//while ((_width / pow(2, numIters) < Room::minRoomWidth || (_height / pow(2, numIters) < Room::minRoomHeight)))
	//	numIters--;

	int numRooms = 4;//GENERATOR.randNum(minRooms, maxRooms + 1);

	Coord topLeft;
	Coord bottomRight;
	topLeft.x = 0;
	topLeft.y = 0;
	bottomRight.x = (_width - 1)  * _tileWidth;
	bottomRight.y = (_height - 1) * _tileHeight;

	const int VERTICAL = 0, HORIZONTIAL = 1;
	const int UP = 0, DOWN = 1;
	int splitDir;
	int upOrDown;

	Room newRoom = Room();

	int currRoomWidth =  (bottomRight.x - topLeft.x) * _tileWidth;
	int currRoomHeight = (bottomRight.y - topLeft.y) * _tileHeight;

	for (int i = 0; i < numRooms; i++) {

		while (currRoomWidth >= Room::maxRoomWidth && currRoomHeight >= Room::maxRoomHeight ) {

			splitDir = GENERATOR.randNum(VERTICAL, HORIZONTIAL + 1);
			upOrDown = GENERATOR.randNum(UP, DOWN + 1);

			//if (currRoomHeight <= Room::maxRoomHeight || currRoomWidth <= Room::maxRoomWidth)
			//	break;

			//else if (currRoomHeight)

			if (splitDir == VERTICAL) {
				// UP == LEFT ROOM
				if (upOrDown == UP) {
					bottomRight.x -= (currRoomWidth / 2) ;
					std::cout << "[V LEFT]\n";
				}
				// DOWN == RIGHT ROOM
				else {
					topLeft.x += (currRoomWidth / 2);
					std::cout << "[V RIGHT]\n";
				}
			}
			else {
				if (upOrDown == UP) {
					bottomRight.y -= currRoomHeight / 2;
					std::cout << "[H UP]\n";
				}
				else {
					topLeft.y += currRoomHeight / 2;
					std::cout << "[H DOWN]\n";
				}
			}

			std::cout << "Current Room Dims: (" << topLeft.x << ',' << topLeft.y << ") : (" << bottomRight.x << ',' << bottomRight.y << ")\n"
					 << "\t" << bottomRight.x - topLeft.x << 'x' << bottomRight.y - topLeft.y << "\n";

			currRoomWidth = (bottomRight.x - topLeft.x);
			currRoomHeight = (bottomRight.y - topLeft.y);
		}

		RoomShape shape = (GENERATOR.randNum(0, 100) <= 25) ? RoomShape::Circle : RoomShape::Rectangle;

		newRoom = (shape == RoomShape::Circle) ? findCircleRoomLocBSP(topLeft, bottomRight) : findRectRoomLocBSP(topLeft, bottomRight);

		_rooms.push_back(newRoom);
		// Reset bsp map
		topLeft.x = 0;
		topLeft.y = 0;
		bottomRight.x = _width - 1;
		bottomRight.y = _height - 1;
	}
}

std::vector<Coord> Level::getHallwayPath(const Coord& start, const Coord& end)
{
	std::cout << "getHallwayPath()\n";

	int cornerX, cornerY;

	if (GENERATOR.randNum(0, 2) == 0) {
		cornerX = end.x;
		cornerY = start.y;
	}
	else {
		cornerX = start.x;
		cornerY = end.y;
	}
	std::vector<Coord> coords;
	for (auto& coord : dda(start.x, start.y, cornerX, cornerY)) {
		coords.push_back(coord);
		//std::cout << "pushed back (1)\n";
	}
	for (auto& coord : dda(cornerX, cornerY, end.x, end.y)) {
		coords.push_back(coord);
		//std::cout << "pushed back (2)\n";
	}
	std::cout << "~getHallwayPath()\n";

	return coords;
}

void Level::generateRooms()
{
	std::cout << "\t\t\t\tGenerating Map...\n";

	bool overlap = true;

	while (overlap) {
		int numRooms = GENERATOR.randNum(minRooms, maxRooms + 1);

		for (int i = 0; i < numRooms; i++) {
			Room room;
			int roomProb = GENERATOR.randNum(1, 101);

			if (roomProb <= 25)
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
	int numHallways = GENERATOR.randNum(maxHallways, maxHallways);
	int numZero = (int)_rooms.size();

	if (_rooms.size() <= 1)
		return;

	for (int hallNum = 0; hallNum < numHallways; hallNum++) {
		for (auto& room : _rooms) {
			if (room.getHallwayAmount() == 0) {
				Room room2 = _rooms.at(GENERATOR.randNum(0, (int)_rooms.size()));

				while (room == room2 || room2.getHallwayAmount() == 2) {
					//std::cout << "generateHallways(): while: 1\n";
					//std::cout << "Room1: " << room.getHallwayAmount() << " Room2: " << room2.getHallwayAmount() << '\n';
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
					//std::cout << "generateHallways(): while: 2\n";
					//std::cout << "Room1: " << room.getHallwayAmount() << " Room2: " << room2.getHallwayAmount() << '\n';
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

	for (size_t y = 0; y < _height; y++) {
		for (size_t x = 0; x < _width; x++) {
			Tile& tile = _envMap[y].at(x);

			if (tile.Char() == Tile::Floor) {
				Tile& tileLeft = _envMap[y].at(x - 1);
				Tile& tileRight = _envMap[y].at(x + 1);
				Tile& tileUp = _envMap[y - 1].at(x);
				Tile& tileDown = _envMap[y + 1].at(x);

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
				_entityMap[enemyY][enemyX] = Entity(enemyTile, enemyX, enemyY);
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
			_entityMap[enemyY][enemyX] = Entity(enemyTile, enemyX, enemyY);
		}
	}
}

void Level::placePlayer(Entity* player)
{
	Room& room = _rooms.at(GENERATOR.randNum(0, (int)_rooms.size()));
	auto center = room.Center();

	while (_envMap[center.y][center.x].Char() != Tile::Floor) {
		room = _rooms.at(GENERATOR.randNum(0, (int)_rooms.size()));
		center = room.Center();
	}

	room.setPlayerStart();
	_startRoom = &room;

	_entityMap[center.y][center.x] = Entity(Tile::Player, center.x, center.y);
	*player = _entityMap.at(center.y).at(center.x);
	player->setX(center.x);
	player->setY(center.y);
	player->setCurrentRoom(&room);
}

void Level::placeExit()
{
	std::vector<std::tuple<int,double>> distances;

	if (_startRoom == nullptr) {
		for (size_t i = 0; i < _rooms.size(); i++) {
			if (_rooms[i].isPlayerStart()) {
				_startRoom = &_rooms[i];
				break;
			}
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

	Room& farthestRoom = _rooms.at(farthestRoomIndex);
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

			int chestX;
			int chestY
				;
			if (room.Shape() == RoomShape::Rectangle) {
				 do  {
					chestX = GENERATOR.randNum(room.startX() + 1, room.endX());
					chestY = GENERATOR.randNum(room.startY() + 1, room.endY());
				 } while (chestX == center.x && chestY == center.y);
			}
			else if (room.Shape() == RoomShape::Circle) {
				do  {
					chestX = GENERATOR.randNum(center.x - room.Radius() + 1, center.x + room.Radius());
					chestY = GENERATOR.randNum(center.y - room.Radius() + 1, center.y + room.Radius());
				} while (chestX == center.x && chestY == center.y);
			}

			_containerMap[chestPos.y][chestPos.x] = Container(Tile::Chest, chestX, chestY, &room);
			(room.isPlayerStart()) ? _containerMap[chestPos.y][chestPos.x].addItem(Potion(HEALING, MINOR)) 
									: _containerMap[chestPos.y][chestPos.x].generateInventory();
			room.incrementNumHallways();
		}
	}

	for (int i = 0; i < (numContainers - 2); i++) {
		Room& room = _rooms.at(0);

		while (room.isPlayerStart() || room.isExit() || room.getContainerAmount() > 0)
			room = _rooms[GENERATOR.randNum(0, (int)_rooms.size())];

		int chestX=0, chestY=0;

		chestX = GENERATOR.randNum(room.startX() + 1, room.endX());
		chestY = GENERATOR.randNum(room.startY() + 1, room.endY());

		if (room.Shape() == RoomShape::Circle) {
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
