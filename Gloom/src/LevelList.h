#ifndef LEVELLIST_H
#define LEVELLIST_H

#include "Tile.h"

struct LevelList {
    LevelList() {
        roomEnemyLists[RoomHardness::ROOM_EMPTY] = { };
        roomEnemyLists[RoomHardness::LEVEL1] = { Tile::Rat, Tile::Goblin };
        roomEnemyLists[RoomHardness::LEVEL2] = { Tile::Rat, Tile::Goblin, Tile::Rogue };
        roomEnemyLists[RoomHardness::LEVEL3] = { Tile::Goblin, Tile::Rogue, Tile::Ogre };
        roomEnemyLists[RoomHardness::LEVEL_BOSS] = { Tile::Dragon };

        roomContainerLists[RoomHardness::ROOM_EMPTY] = { UNKNOWN, DAGGER, HEALING };
        roomContainerLists[RoomHardness::LEVEL1] = { UNKNOWN, SWORD_SHORT, GAUNTLETS, BOOTS, HELMET, HEALING };
        roomContainerLists[RoomHardness::LEVEL2] = { UNKNOWN, SWORD_SHORT, AXE_WAR, GAUNTLETS, BOOTS, HELMET, GREAVES, PAULDRONS, HEALING };
        roomContainerLists[RoomHardness::LEVEL3] = { SWORD_SHORT, AXE_WAR, SWORD_LONG, GREAVES, PAULDRONS, CHESTPLATE, HEALING };
        roomContainerLists[RoomHardness::LEVEL_BOSS] = { AXE_WAR, AXE_BATTLE, SWORD_LONG, GREAVES, PAULDRONS, CHESTPLATE, HEALING };
    }
    ~LevelList() { }

    static std::vector<char> getEnemyLists(RoomHardness hardness) { return roomEnemyLists[hardness]; }
    static std::vector<int>  getContainerLists(RoomHardness hardness) { return roomContainerLists[hardness]; }

    static std::map<RoomHardness, std::vector<char>> roomEnemyLists;
    static std::map<RoomHardness, std::vector<int>> roomContainerLists;
};

#endif
