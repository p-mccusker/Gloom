#include "Container.h"



Container::Container(Room& room, const char& tile, const int& x, const int& y)
	:Tile(x, y, Tile::Chest)
{
	_action = Action::OPEN;
	_inv = new Inventory(InventoryOwnerType::Container, (void*)this);
	*_room = room;
	_char = tile;
}

Container::~Container()
{
	delete _inv;
	_inv = nullptr;
	delete _room;
	_room = nullptr;
}

ItemType Container::getItemType(const int& itemType)
{
	if (itemType == ItemType::UNKNOWN)
		return ItemType::UNKNOWN;
	else if (itemType > ItemType::UNKNOWN && itemType < ArmorType::ARMOR_NULL)
		return ItemType::WEAPON;
	else if (itemType >= ArmorType::ARMOR_NULL && itemType < PotionType::POTION_NULL)
		return ItemType::ARMOR;
	else
		return ItemType::POTION;
}

void Container::generateItems(std::vector<int>& possibleItems, const int& numItems)
{
	for (int i = 0; i < numItems; i++) {
		int itemChoice = GENERATOR.Choice<int>(possibleItems.data(), possibleItems.size());

		ItemType itemType = getItemType(itemChoice);

		if (itemType == ItemType::WEAPON) {
			Weapon randWeapon(itemChoice);
			addItem(randWeapon);
		}
		else if (itemChoice == ItemType::ARMOR) {
			Armor randArmor(itemChoice);
			addItem(randArmor);
		}
		else if (itemChoice == ItemType::POTION) {
			if (_room->Hardness() == RoomHardness::ROOM_EMPTY) {
				Potion randPotion(itemChoice, PotionMag::MINOR);
				addItem(randPotion);
			}
			else if (_room->Hardness() == RoomHardness::LEVEL1) {
				int prob = GENERATOR.randNum(1, 101);
				if (prob <= 50) {
					Potion randPotion(itemChoice, PotionMag::MINOR);
					addItem(randPotion);
				}
				else {
					Potion randPotion(itemChoice, PotionMag::MAJOR);
					addItem(randPotion);
				}

			}
			else if (_room->Hardness() == RoomHardness::LEVEL2) {
				int prob = GENERATOR.randNum(1, 101);
				if (prob <= 33) {
					Potion randPotion(itemChoice, PotionMag::MINOR);
					addItem(randPotion);
				}
				else {
					Potion randPotion(itemChoice, PotionMag::MAJOR);
					addItem(randPotion);
				}

			}
			else if (_room->Hardness() == RoomHardness::LEVEL3) {
				int prob = GENERATOR.randNum(1, 101);
				if (prob <= 10) {
					Potion randPotion(itemChoice, PotionMag::MINOR);
					addItem(randPotion);
				}
				else {
					Potion randPotion(itemChoice, PotionMag::MAJOR);
					addItem(randPotion);
				}

			}
			else if (_room->Hardness() == RoomHardness::LEVEL_BOSS) {
				int prob = GENERATOR.randNum(1, 101);
				Potion randPotion(itemChoice, PotionMag::MAJOR);
				addItem(randPotion);
			}

		}
	}
}

void Container::generateInventory()
{
	auto possibleItems = LevelList::getContainerLists(_room->Hardness());
	int choice = GENERATOR.randNum(1, 101);
	int numPossibleItems = 0;

	if (_room->Hardness() == RoomHardness::ROOM_EMPTY) 
		if (choice > 30) 
			numPossibleItems = 1;
	else if (_room->Hardness() == RoomHardness::LEVEL1) 
		if (choice > 20)
			numPossibleItems = GENERATOR.randNum(1, 3);
	else if (_room->Hardness() == RoomHardness::LEVEL2)
		if (choice > 10)
			numPossibleItems = GENERATOR.randNum(1, 3);
	else if (_room->Hardness() == RoomHardness::LEVEL3)
		if (choice > 20)
			numPossibleItems = 2;
	else if (_room->Hardness() == RoomHardness::LEVEL_BOSS)
			numPossibleItems = GENERATOR.randNum(2, 4);
	
	generateItems(possibleItems, numPossibleItems);
}