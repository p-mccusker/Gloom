#ifndef ROOM_H
#define ROOM_H

#include "def.h"

extern random GENERATOR;

enum class RoomSize { Small, Large };
enum class RoomShape { Unititialized, Rectangle, Circle };
enum RoomHardness { Empty, Level1, Level2, Level3, Boss, TotalRooms};

int hardnessToInt(RoomHardness hardness);

class Room
{
public:
	Room() { }
	Room(const Room& other);
	Room(const int& startX , const int& startY, const int& width, const int& height);
	Room(const int& centerX, const int& centerY, const int& radius);
	~Room() { }

	std::tuple<int, int> Center() { return std::tuple<int,int>(_centerX, _centerY); }
	RoomHardness Hardness() { return _hardness; }
	RoomSize Size() { return _size; }
	RoomShape Shape() { return _shape; }
	int getHallwayAmount() { return _numHallways; }
	int getContainerAmount() { return _numContainers; }
	int Radius() { return _radius; }
	int startX() { return _startX; }
	int startY() { return _startY; }
	int endX() { return _endX; }
	int endY() { return _endY; }
	bool isPlayerStart() { return _playerStart; }
	bool isExit() { return _levelExit; }
	bool Intersect(Room& other);
	bool isInRoom(const int& x, const int& y);

	void Generate();
	void incrementNumHallways() { _numHallways++; }
	void setHardness(RoomHardness hardness) { _hardness = hardness; }
	void setPlayerStart() { _playerStart = true; }
	void setLevelExit() { _levelExit = true; }

	static const int maxRoomWidth = 16;
	static const int minRoomWidth = 6;
	static const int maxRoomHeight = 16;
	static const int minRoomHeight = 6;

	static const int minRoomRadius = 3;
	static const int maxRoomRadius = 8;
protected:
	RoomHardness _hardness = RoomHardness::Empty;
	RoomSize _size = RoomSize::Small;
	RoomShape _shape = RoomShape::Unititialized;
	int _numHallways = 0;
	int _numContainers = 0;
	int _startX = 0,
		_startY = 0,
		_centerX = 0, 
		_centerY = 0,
		_endX = 0,
		_endY = 0,
		_width = 0,
		_height = 0,
		_radius = 0;
	bool _playerStart = false,
		 _levelExit = false;

};

#endif