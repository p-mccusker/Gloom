#ifndef LEVEL_H
#define LEVEL_H

#include "Container.h"

using EnvMap = std::vector<std::vector<Tile>>;
using ContainerMap = std::vector<std::vector<Container>>;
using EntityMap = std::vector<std::vector<Entity>>;

class Level
{
public:
    Level(const int& tilesWide, const int& tilesHigh);
    ~Level();

    Room* findRectRoomLoc();
    Room* findCircleRoomLoc();
    std::vector<std::tuple<int, int>> getHallwayPath(std::tuple<int, int> startPos, std::tuple<int, int> endPos);

    void generateRooms();
    void generateHallways();
    void placeHallwayWalls();
    void generateEnemies();
    void placePlayer(Player& player);
    void placeExit();
    void generateContainers();


    static const int maxRooms = 12;
    static const int minRooms = 11;
private:
    int _width = 0, _height = 0;
    EnvMap _envMap;
    ContainerMap _containerMap;
    EntityMap _entityMap;
    std::vector<Room*> _rooms;
    Room* _startRoom;
    Room* _exitRoom;
};

#endif // LEVEL_H
