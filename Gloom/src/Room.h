#ifndef ROOM_H
#define ROOM_H

#include "def.h"

extern random GENERATOR;

enum class RoomSize { SMALL, LARGE };
enum class RoomShape { RECTANGLE, CIRCLE };
enum class RoomHardness { ROOM_EMPTY, LEVEL1, LEVEL2, LEVEL3, LEVEL_BOSS};

class Room
{
public:
	Room() { }
	~Room() { }

	std::tuple<int, int> Center() { return std::tuple<int,int>(_centerX, _centerY); }
	RoomHardness Hardness() { return _hardness; }
	virtual void Generate() = 0;

protected:
	RoomHardness _hardness = RoomHardness::ROOM_EMPTY;
	RoomSize _size = RoomSize::SMALL;
	RoomShape _shape = RoomShape::RECTANGLE;
	int _numHallways = 0;
	int _numContainers = 0;
	int _centerX = 0, _centerY = 0;
	bool _playerExit = false,
		 _levelExit = false;

};

class CircleRoom;

class RectRoom : protected Room {
public:
	RectRoom(const int& startX=0, const int& startY=0, const int& width=0, const int& height=0);
	~RectRoom();

	bool Intersect(const RectRoom& other);
	bool Intersect(const CircleRoom& other);
	bool isInRoom(const int& x, const int& y);
	void Generate();


	static const int maxRoomWidth = 16;
	static const int minRoomWidth = 6;
	static const int maxRoomHeight = 16;
	static const int minRoomHeight = 6;

	friend class CircleRoom;
private:
	int _startX,
		_startY,
		_endX,
		_endY,
		_width,
		_height;
};

class CircleRoom : protected Room {
public:
	CircleRoom(const int& centerX, const int& centerY, const int& radius);
	~CircleRoom() { }

	bool Intersect(const RectRoom& other);
	bool Intersect(const CircleRoom& other);
	bool isInRoom(const int& x, const int& y);
	void Generate();

	static const int minRoomRadius = 3;
	static const int maxRoomRadius = 8;

	friend class RectRoom;
private:
	int _radius;

};

#endif