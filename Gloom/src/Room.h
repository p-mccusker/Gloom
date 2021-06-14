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

	Coord Center() const { return { _centerX, _centerY }; }
	RoomHardness Hardness() const { return _hardness; }
	RoomSize Size() const { return _size; }
	RoomShape Shape() const { return _shape; }
	int getHallwayAmount() const { return _numHallways; }
	int getContainerAmount() const { return _numContainers; }
	int Radius() const { return _radius; }
	int startX() const { return _startX; }
	int startY() const { return _startY; }
	int endX() const { return _endX; }
	int endY() const { return _endY; }
	bool isPlayerStart() const { return _playerStart; }
	bool isExit() const { return _levelExit; }
	bool Intersect(Room& other) const;
	bool isInRoom(const int& x, const int& y) const;

	void Generate();
	void incrementNumHallways() { _numHallways++; }
	void setHardness(RoomHardness hardness) { _hardness = hardness; }
	void setPlayerStart() { _playerStart = true; }
	void setLevelExit() { _levelExit = true; }

	static const int maxRoomWidth = 14;
	static const int minRoomWidth = 5;
	static const int maxRoomHeight = 14;
	static const int minRoomHeight = 5;

	static const int minRoomRadius =  minRoomWidth / 2;
	static const int maxRoomRadius =  maxRoomHeight / 2;

	friend bool operator==(const Room& lhs, const Room& rhs) { return &lhs == &rhs; }
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