#ifndef INVENTORY_H
#define INVENTORY_H

#include "Item.h"

enum class InventoryOwnerType { Uninitialized, Entity, Container };

class Inventory
{
public:
	Inventory();
	Inventory(InventoryOwnerType ownerType , void* owner = nullptr);
	~Inventory() { }//delete _owner; _owner = nullptr; }

	InventoryOwnerType ownerType() { return _ownerType; }
	//void* Owner() { return _owner; }
	//std::tuple<InventoryOwnerType, void*> ownerAndType() { return std::tuple<InventoryOwnerType, void*>(_ownerType, _owner); }


	void addItem(Weapon weapon);
	void addItem(Armor armor);
	void addItem(Potion potion);

	void removeItem(Weapon& weapon);
	void removeItem(Armor& weapon);
	void removeItem(Potion& weapon);


	std::vector<Armor>& getArmor() { return _armor; }
	std::vector<Weapon>& getWeapons() { return _weapons; }
	std::vector<Potion>& getPotions() { return _potions; }

	friend bool operator==(const Inventory& lhs, const Inventory& rhs) { return &lhs == &rhs; }
	Inventory& operator=(const Inventory& other);

private:
	std::vector<Armor> _armor;
	std::vector<Weapon> _weapons;
	std::vector<Potion> _potions;
	//void* _owner;
	InventoryOwnerType _ownerType;
};

#endif 