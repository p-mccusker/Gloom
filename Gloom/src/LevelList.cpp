#include "LevelList.h"

std::map<int, std::vector<char>> LevelList::roomEnemyLists = { 
	{RoomHardness::Empty,  {Tile::Uninitialized} },
	{RoomHardness::Level1, {Tile::Rat, Tile::Goblin} },
	{RoomHardness::Level2, {Tile::Rat, Tile::Goblin, Tile::Rogue} },
	{RoomHardness::Level3, {Tile::Goblin, Tile::Rogue, Tile::Ogre} },
	{RoomHardness::Boss,   {Tile::Dragon} } 
};

std::map<int, std::vector<int>> LevelList::roomContainerLists = {
	{RoomHardness::Empty,  {UNKNOWN, DAGGER, HEALING} },
	{RoomHardness::Level1, {UNKNOWN, SWORD_SHORT, GAUNTLETS, BOOTS, HELMET, HEALING} },
	{RoomHardness::Level2, {UNKNOWN, SWORD_SHORT, AXE_WAR, GAUNTLETS, BOOTS, HELMET, GREAVES, PAULDRONS, HEALING} },
	{RoomHardness::Level3, {SWORD_SHORT, AXE_WAR, SWORD_LONG, GREAVES, PAULDRONS, CHESTPLATE, HEALING} },
	{RoomHardness::Boss,   {AXE_WAR, AXE_BATTLE, SWORD_LONG, GREAVES, PAULDRONS, CHESTPLATE, HEALING} }
};