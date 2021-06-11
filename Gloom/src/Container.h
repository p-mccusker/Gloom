#ifndef CONTAINER_H
#define CONTAINER_H

#include "LevelList.h"

enum class Action { Uninitialized, Use, Open };

enum class IObjectType { Container, Exit};

class InteractObject : public Tile {
public:
	InteractObject();
	~InteractObject();

	virtual void Use() = 0;


protected:
	
	IObjectType _objectType;
};

class Container : public Tile
{
public:
	Container(const char& tile, const int& x, const int& y, Room* room=nullptr);
	~Container();

	Container(const Container& other);

	void addItem(Armor armor) { _inv.addItem(armor); }
	void addItem(Weapon weapon) { _inv.addItem(weapon); }
	void addItem(Potion potion) { _inv.addItem(potion); }

	std::vector<Armor>&  getArmor() { return _inv.getArmor(); }
	std::vector<Potion>& getPotions() { return _inv.getPotions(); }
	std::vector<Weapon>& getWeapons() { return _inv.getWeapons(); }
	ItemType getItemType(const int& itemType);
	Room& getRoom() { return *_room; }

	void generateItems(std::vector<int>& possibleItems, const int& numItems);
	void generateInventory();

	Container& operator=(const Container& rhs);
	bool operator==(const Container& rhs) const;

private:
	Action _action = Action::Uninitialized;
	//std::unique_ptr<Inventory> _inv = nullptr;
	Inventory _inv;
	Room* _room;
	
};

class Exit {};
#endif