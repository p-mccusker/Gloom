#ifndef CONTAINER_H
#define CONTAINER_H

#include "Inventory.h"
#include "LevelList.h"

enum class Action { USE, OPEN };

class Container : protected Tile
{
public:
	Container(Room& room, const char& tile, const int& x, const int& y);
	~Container();

	void addItem(Armor& armor) { _inv->addItem(&armor); }
	void addItem(Weapon& weapon) { _inv->addItem(&weapon); }
	void addItem(Potion& potion) { _inv->addItem(&potion); }

	std::vector<Armor>&  getArmor() { return _inv->getArmor(); }
	std::vector<Potion>& getPotions() { return _inv->getPotions(); }
	std::vector<Weapon>& getWeapons() { return _inv->getWeapons(); }
	ItemType getItemType(const int& itemType);

	void generateItems(std::vector<int>& possibleItems, const int& numItems);
	void generateInventory();
private:
	Action _action;
	Inventory* _inv;
	Room* _room;
};

#endif