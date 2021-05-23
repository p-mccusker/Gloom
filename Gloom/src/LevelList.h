#ifndef LEVELLIST_H
#define LEVELLIST_H

#include "Tile.h"

struct LevelList {
    LevelList() { }
    ~LevelList() { }

    static std::vector<char> getEnemyLists(RoomHardness hardness) { return roomEnemyLists[hardness]; }
    static std::vector<int>  getContainerLists(RoomHardness hardness) { return roomContainerLists[hardness]; }

    static std::map<int, std::vector<char>> roomEnemyLists;
    static std::map<int, std::vector<int>> roomContainerLists;
};

#endif
