#include "Room.h"

inline int hardnessToInt(RoomHardness hardness)
{
	switch (hardness)
	{
	case RoomHardness::Empty:
		return 0;
	case RoomHardness::Level1:
		return 1;
	case RoomHardness::Level2:
		return 2;
	case RoomHardness::Level3:
		return 3;
	case RoomHardness::Boss:
		return 4;
	default:
		return -1;
	}
}

Room::Room(const Room& other)
{
	_hardness = other._hardness;
	_size = other._size;
	_shape = other._shape;
	_numHallways = other._numHallways;
	_numContainers = other._numContainers;
	_startX = other._startX;
	_startY = other._startY;
	_centerX = other._centerX;
	_centerY = other._centerY;
	_endX = other._endX;
	_endY = other._endY;
	_width = other._width;
	_height = other._height;
	_radius = other._radius;
	_playerStart = other._playerStart;
	_levelExit = other._levelExit;
}

Room::Room(const int& startX, const int& startY, const int& width, const int& height) 
	:_startX(startX), _startY(startY), _width(width), _height(height)
{
	_endX = _startX + _width;
	_endY = _startY + _height;
	_shape = RoomShape::Rectangle;
	_centerX = int((_startX + _endX) / 2);
	_centerY = int((_startY + _endY) / 2);
	_radius = -1;
}

Room::Room(const int& centerX, const int& centerY, const int& radius)
	:_centerX(centerX), _centerY(centerY), _radius(radius)
{
	_startX = _centerX - _radius;
	_startY = _centerY - _radius;
	_endX = _centerX + _radius;
	_endY = _centerY + _radius;
	_shape = RoomShape::Circle;
	_height = 2 * _radius;
	_width = 2 * _radius;
}

bool Room::Intersect(Room& other) const
{
	if (this->_shape == RoomShape::Rectangle && other._shape == RoomShape::Rectangle)
		return _startX <= other._endX  && _endX >= other._startX 
			&& _startY <= other._endY  && _endY >= other._startY;
	else if (this->_shape == RoomShape::Rectangle && other._shape == RoomShape::Circle) {
		double xDist = abs(other._centerX - _centerX);
		double	yDist = abs(other._centerY - _centerY);

		if (xDist > ((_width / 2)  + other._radius)) return false;
		if (yDist > ((_height / 2) + other._radius)) return false;
		if (xDist <= (_width / 2))  return true;
		if (yDist <= (_height / 2)) return true;

		double cornerDist = pow((xDist - (_width / 2)), 2) +
			pow((yDist - (_height / 2)), 2);

		return cornerDist <= pow(other._radius, 2);
	}
	else if (this->_shape == RoomShape::Circle && other._shape == RoomShape::Rectangle) {
		double xDist = abs(other._centerX - _centerX);
		double	yDist = abs(other._centerY - _centerY);

		if (xDist > (((double)other._width / 2) + _radius)) return false;
		if (yDist > (((double)other._height / 2) + _radius)) return false;
		if (xDist <= ((double)other._width / 2)) return true;
		if (yDist <= ((double)other._height / 2)) return true;

		double cornerDist = pow((xDist - (other._width / 2)), 2) + pow((yDist - (other._height / 2)), 2);

		return cornerDist <= pow((double)_radius, 2);
	}
	else {
		double distance = circleDistance(_centerX, _centerY, other._centerX, other._centerY);
		return distance <= ((double)_radius + other._radius) + 0.5;
	}
}

bool Room::isInRoom(const int& x, const int& y) const 
{
	if (_shape == RoomShape::Rectangle)
		return _startX <= x && x <= _endX && _startY <= y && y <= _endY;
	else
		return circleDistance(x, y, _centerX, _centerY) <= _radius + 0.5;
}

void Room::Generate()
{
	double smallRoomAreaCutoff, area;

	if (_shape == RoomShape::Rectangle) {
		smallRoomAreaCutoff = (5 / 8) * (maxRoomWidth * maxRoomHeight);
		area = double(_width) * _height;
	}
	else {
		smallRoomAreaCutoff = (6 / 8) * (M_PI * pow(maxRoomRadius, 2));
		area = M_PI * pow(_radius, 2);
	}
	if (area < smallRoomAreaCutoff) {
		_size = RoomSize::Small;
		int hardness = GENERATOR.randNum(1, 100);
		if (hardness <= 60)
			_hardness = RoomHardness::Level1;
		else if (60 < hardness && hardness <= 85)
			_hardness = RoomHardness::Level2;
		else
			_hardness = RoomHardness::Level3;
	}
	else {
		_size = RoomSize::Large;
		int hardness = GENERATOR.randNum(1, 100);
		if (hardness <= 25)
			_hardness = RoomHardness::Level1;
		else if (25 < hardness && hardness <= 75)
			_hardness = RoomHardness::Level2;
		else
			_hardness = RoomHardness::Level3;
	}
}

