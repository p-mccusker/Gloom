#include "Inventory.h"

Inventory::Inventory(InventoryOwnerType ownerType, void* owner)
{
	_owner = owner;
	_ownerType = ownerType;
}

void Inventory::addItem(Weapon* weapon)
{
	_weapons.push_back(std::move(*weapon));
}

void Inventory::addItem(Armor* armor)
{
	_armor.push_back(std::move(*armor));
}

void Inventory::addItem(Potion* potion)
{
	_potions.push_back(std::move(*potion));
}

void Inventory::removeItem(Weapon& weapon)
{
	int itemIndex = -1;
	for (unsigned int i = 0; i < _weapons.size(); i++) {
		if (_weapons[i] == weapon) {
			itemIndex = i;
			break;
		}
	}
	if (itemIndex > -1)
		_weapons.erase(_weapons.begin() + itemIndex);
}

void Inventory::removeItem(Armor& armor)
{
	int itemIndex = -1;
	for (unsigned int i = 0; i < _armor.size(); i++) {
		if (_armor[i] == armor) {
			itemIndex = i;
			break;
		}
	}
	if (itemIndex > -1)
		_armor.erase(_armor.begin() + itemIndex);
}

void Inventory::removeItem(Potion& potion)
{
	int itemIndex = -1;
	for (unsigned int i = 0; i < _potions.size(); i++) {
		if (_potions[i] == potion) {
			itemIndex = i;
			break;
		}
	}
	if (itemIndex > -1)
		_potions.erase(_potions.begin() + itemIndex);
}
