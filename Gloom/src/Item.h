#ifndef ITEM_H
#define ITEM_H

#include "def.h"

enum ItemType { UNKNOWN, WEAPON, ARMOR, POTION };

enum WeaponType { WEAPON_NULL, DAGGER, SWORD_SHORT, SWORD_LONG, AXE_BATTLE, AXE_WAR, TOTAL_WEAPON_TYPES };

enum ArmorType { ARMOR_NULL = TOTAL_WEAPON_TYPES + 1, CHESTPLATE, GAUNTLETS, PAULDRONS, GREAVES, BOOTS, HELMET, TOTAL_ARMOR_TYPES };

enum PotionType { POTION_NULL = TOTAL_ARMOR_TYPES + 1, HEALING, TOTAL_POTION_TYPES };

enum PotionMag { MINOR = TOTAL_POTION_TYPES + 1, MAJOR };

class Item
{
public:
	Item() { }
	~Item() { }

	void toggleEquip() { _equipped = !_equipped; }

	int Worth() { return _worth; }

	int Magnitude() { return _magnitude; }

	ItemType itemType() { return _itemType; }

	bool isEquipped() { return _equipped; }

protected:
	int _worth = 0;
	int _lvl = 1;
	ItemType _itemType = UNKNOWN;
	bool _equipped = false;
	int _magnitude = 0;
};

extern random GENERATOR;

class Armor :
	protected Item
{
public:
	Armor(int aType = (int)ARMOR_NULL);
	~Armor() { }

	void toggleEquip() { _equipped = !_equipped; }
	int Defense() { return _magnitude; }
	ArmorType armorType() { return _armorType; }

	friend std::ostream& operator<<(std::ostream& os, const Armor& a);
	bool operator==(const Armor& rhs);
private:
	ArmorType _armorType;
};

class Potion : protected Item
{
public:
	Potion(int pType, int pMag);
	~Potion() { }

	PotionType potionType() { return _potionType; }
	PotionMag potionMag() { return _potionMag; }

	std::tuple<PotionType, int> Use() { return std::tuple<PotionType, int>(_potionType, _magnitude); }

	friend std::ostream& operator<<(std::ostream& os, const Potion& p);
	bool operator==(const Potion& rhs);

private:
	PotionType _potionType;
	PotionMag _potionMag;
};

class Weapon : protected Item
{
public:
	Weapon(int wType = (int)WEAPON_NULL);
	~Weapon() { }

	void toggleEquip() { _equipped = !_equipped; }
	int Damage() { return _magnitude; }
	WeaponType weaponType() { return _weaponType; }

	friend std::ostream& operator<<(std::ostream& os, const Weapon& w);
	bool operator==(const Weapon& rhs);
private:
	WeaponType _weaponType;
};
#endif