#ifndef LEVEL_H
#define LEVEL_H

#include "Container.h"

enum class Map { Tile, Entity, Container };
enum class Direction { Up, Down, Left, Right };

using TileMap = std::vector<std::vector<Tile>>;

class Level
{
public:
    Level(const int& tilesWide, const int& tilesHigh, const int& tileWidth, const int& tileHeight);
    ~Level();

    void Generate(Entity* player);
    Tile& getTile(const int& x, const int& y);
    Entity& getEntity(const int& index);
    Container& getContainer(const int& index);
    std::vector<Room>& getRooms() { return _rooms; }
    int Width() { return _width; }
    int Height() { return _height; }

    void reset(const Map& map, const int& index);


    static const int maxRooms = 9;
    static const int minRooms = 6;

private:
    Room findRectRoomLoc();
    Room findCircleRoomLoc();
    Room findRectRoomLocBSP(const Coord& topLeft, const Coord& bottomRight);
    Room findCircleRoomLocBSP(const Coord& topLeft, const Coord& bottomRight);
    void generateRoomsBSP();

    std::vector<Coord> getHallwayPath(const Coord& start, const Coord& end);

    void generateRooms();
    void generateHallways();
    void placeHallwayWalls();
    void generateEnemies();
    void placePlayer(Entity* player);
    void placeExit();
    void generateContainers();

private:
    int _width = 0, _height = 0,
        _tileWidth = 0, _tileHeight = 0;
    TileMap _envMap;
    std::vector<Container> _containerMap;
    std::vector<Entity> _entityMap;
    std::vector<Room> _rooms;
    Room* _startRoom;
    Room* _exitRoom;
};

#endif // LEVEL_H
