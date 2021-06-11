#include "Container.h"



Container::Container(const char& tile, const int& x, const int& y, Room* room)
	:Tile(x, y, tile)
{
	_action = Action::Open;
	_inv = Inventory(InventoryOwnerType::Container, (void*)this); //std::make_unique<Inventory>(InventoryOwnerType::Container, (void*)this);
	_room = room;
}

Container::~Container()
{
	_room = nullptr;
}

Container::Container(const Container& other)
{
	_action = other._action;
	//_inv.reset();
	//_inv = std::make_unique<Inventory>(std::move(other._inv));
	_inv = other._inv;
	_room = std::move(other._room);
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
		int itemChoice = GENERATOR.Choice<int>(possibleItems);

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
			if (_room->Hardness() == RoomHardness::Empty) {
				Potion randPotion(itemChoice, PotionMag::MINOR);
				addItem(randPotion);
			}
			else if (_room->Hardness() == RoomHardness::Level1) {
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
			else if (_room->Hardness() == RoomHardness::Level2) {
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
			else if (_room->Hardness() == RoomHardness::Level3) {
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
			else if (_room->Hardness() == RoomHardness::Boss) {
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

	if (_room->Hardness() == RoomHardness::Empty)
		if (choice > 30) 
			numPossibleItems = 1;
	else if (_room->Hardness() == RoomHardness::Level1)
		if (choice > 20)
			numPossibleItems = GENERATOR.randNum(1, 3);
	else if (_room->Hardness() == RoomHardness::Level2)
		if (choice > 10)
			numPossibleItems = GENERATOR.randNum(1, 3);
	else if (_room->Hardness() == RoomHardness::Level3)
		if (choice > 20)
			numPossibleItems = 2;
	else if (_room->Hardness() == RoomHardness::Boss)
			numPossibleItems = GENERATOR.randNum(2, 4);
	
	generateItems(possibleItems, numPossibleItems);
}

Container& Container::operator=(const Container& rhs)
{
	_action = rhs._action;
	//_inv.reset();
	_inv = rhs._inv; // std::make_unique<Inventory>(std::move(rhs._inv));
	_room = rhs._room;

	return *this;
}

bool Container::operator==(const Container& rhs) const
{
	return _action == rhs._action
		&& _inv == rhs._inv
		&& _room == rhs._room;
}
