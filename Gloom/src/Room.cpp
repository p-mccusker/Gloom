#include "Room.h"

double circleDistance(const double& x0, const double& y0, const double& x1, const double& y1) {
	return sqrt(pow((x1 - x0), 2) + pow((y1 - y0), 2));
}

RectRoom::RectRoom(const int& startX, const int& startY, const int& width, const int& height) 
	:_startX(startX), _startY(startY), _width(width), _height(height), Room()
{
	_endX = _startX + _width;
	_endY = _startY + _height;
	_shape = RoomShape::RECTANGLE;
	_centerX = int((_startX + _endX) / 2);
	_centerX = int((_startY + _endY) / 2);
}

RectRoom::~RectRoom()
{
}

bool RectRoom::Intersect(const RectRoom& other)
{
	return _startX <= other._endX 
		&& _endX >= other._startX 
		&& _startY <= other._endY 
		&& _endY >= other._startY;
}

bool RectRoom::Intersect(const CircleRoom& other)
{
	int xDist = abs(other._centerX - _centerX);
	int	yDist = abs(other._centerY - _centerY);

	if (xDist > ((_width / 2) + other._radius) || yDist > ((_height / 2) + other._radius)) return false;
	if (xDist <= (_width / 2) || yDist <= (_height / 2)) return true;

	double cornerDist = pow((xDist - _width / 2), 2) +
		pow((yDist - _height / 2), 2);

	return cornerDist <= pow(other._radius, 2);
}

bool RectRoom::isInRoom(const int& x, const int& y)
{
	return _startX <= x && x <= _endX && _startY <= y && y <= _endY;
}

void RectRoom::Generate()
{
	double smallRoomAreaCutoff = (5 / 8) * (maxRoomWidth * maxRoomHeight);
	double area = double(_width) * _height;
	if (area < smallRoomAreaCutoff) {
		_size = RoomSize::SMALL;
		int hardness = GENERATOR.randNum(1, 100);
		if (hardness <= 60)
			_hardness = RoomHardness::LEVEL1;
		else if (60 < hardness && hardness <= 85)
			_hardness = RoomHardness::LEVEL2;
		else
			_hardness = RoomHardness::LEVEL3;
	}
	else {
		_size = RoomSize::LARGE;
		int hardness = GENERATOR.randNum(1, 100);
		if (hardness <= 25)
			_hardness = RoomHardness::LEVEL1;
		else if (25 < hardness && hardness <= 75)
			_hardness = RoomHardness::LEVEL2;
		else
			_hardness = RoomHardness::LEVEL3;
	}
}

CircleRoom::CircleRoom(const int& centerX, const int& centerY, const int& radius) 
	:Room()
{
	_centerX = centerX;
	_centerY = centerY;
	_radius = radius;
	_shape = RoomShape::CIRCLE;
}

bool CircleRoom::Intersect(const RectRoom& other)
{
	int xDist = abs(other._centerX - _centerX);
	int	yDist = abs(other._centerY - _centerY);

	if (xDist > ((other._width / 2) + _radius) || yDist > ((other._height / 2) + _radius)) return false;
	if (xDist <= (other._width / 2) || yDist <= (other._height / 2)) return true;

	double cornerDist = pow((xDist - other._width / 2), 2) + pow((yDist - other._height / 2), 2);

	return cornerDist <= pow(_radius, 2);
}

bool CircleRoom::Intersect(const CircleRoom& other)
{
	double distance = circleDistance(_centerX, _centerY, other._centerX, other._centerY);
	return distance <= (double(_radius) + double(other._radius)) + 0.5;
}

bool CircleRoom::isInRoom(const int& x, const int& y)
{
	return circleDistance(x, y, _centerX, _centerY) <= _radius + 0.5;
}

void CircleRoom::Generate()
{
	double smallRoomAreaCutoff = (6/ 8) * (M_PI * pow(maxRoomRadius, 2));
	double area = M_PI * pow(_radius, 2);
	int hardness = GENERATOR.randNum(1, 100);

	if (area < smallRoomAreaCutoff) {
		_size = RoomSize::SMALL;
		if (hardness <= 60)
			_hardness = RoomHardness::LEVEL1;
		else if (60 < hardness && hardness <= 85)
			_hardness = RoomHardness::LEVEL2;
		else
			_hardness = RoomHardness::LEVEL3;
	}
	else {
		_size = RoomSize::LARGE;
		if (hardness <= 25)
			_hardness = RoomHardness::LEVEL1;
		else if (25 < hardness && hardness <= 75)
			_hardness = RoomHardness::LEVEL2;
		else
			_hardness = RoomHardness::LEVEL3;
	}
}

