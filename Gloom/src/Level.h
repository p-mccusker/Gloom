#ifndef LEVEL_H
#define LEVEL_H

#include "Container.h"

enum class Map { Tile, Entity, Container };
enum class Direction { Up, Down, Left, Right };


using EnvMap = std::vector<std::vector<Tile>>;
using ContainerMap = std::vector<std::vector<Container>>;
using EntityMap = std::vector<std::vector<Entity>>;

class Level
{
public:
    Level(const int& tilesWide, const int& tilesHigh);
    ~Level();

    void Generate(Player& player);
    Tile& getTile(const int& x, const int& y);
    Entity& getEntity(const int& x, const int& y);
    Container& getContainer(const int& x, const int& y);
    std::vector<Room>& getRooms() { return _rooms; }
    int Width() { return _width; }
    int Height() { return _height; }

    void reset(const Map& map, const int& x, const int& y);

    void swap(Tile& t1, const Direction& dir);
    void swap(Entity& t1,const Direction& dir);
    void swap(Container& t1, const Direction& dir);

    static const int maxRooms = 11;
    static const int minRooms = 8;

private:
    Room findRectRoomLoc();
    Room findCircleRoomLoc();
    std::vector<Coord> getHallwayPath(const Coord& start, const Coord& end);

    void generateRooms();
    void generateHallways();
    void placeHallwayWalls();
    void generateEnemies();
    void placePlayer(Player& player);
    void placeExit();
    void generateContainers();

private:
    int _width = 0, _height = 0;
    EnvMap _envMap;
    ContainerMap _containerMap;
    EntityMap _entityMap;
    std::vector<Room> _rooms;
    Room* _startRoom;
    Room* _exitRoom;
};

#endif // LEVEL_H
