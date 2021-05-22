#ifndef LEVEL_H
#define LEVEL_H

#include "Container.h"

using EnvMap = std::vector<std::vector<EnvTile>>;
using ContainerMap = std::vector<std::vector<Container>>;
using EntityMap = std::vector<std::vector<Entity>>;

class Level
{
public:
    Level(const int& tilesWide, const int& tilesHigh);
    Level() { }
    ~Level() { }

    RectRoom* findSquareRoomLoc();
    CircleRoom* findCircleRoomLoc();

    static const int maxRooms = 12;
    static const int minRooms = 11;
private:
    int _width = 0, _height = 0;
    EnvMap _envMap;
    ContainerMap _containerMap;
    EntityMap _entityMap;
    std::vector<Room*> _rooms;
    Room* startRoom;
    Room* exitRoom;
    
};

#endif // LEVEL_H
