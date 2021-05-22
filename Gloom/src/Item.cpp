#include "Item.h"


Armor::Armor(int aType)
	:Item()
{
	ArmorType atype = (ArmorType)aType;
	_armorType = atype;
	switch (_armorType) {
	case ARMOR_NULL:
		_magnitude = 0;
		break;
	case CHESTPLATE:
		_magnitude = GENERATOR.randNum(3, 4);
		break;
	case GAUNTLETS:
		_magnitude = GENERATOR.randNum(1, 2);
		break;
	case PAULDRONS:
		_magnitude = GENERATOR.randNum(2, 3);
		break;
	case GREAVES:
		_magnitude = GENERATOR.randNum(2, 3);
		break;
	case BOOTS:
		_magnitude = GENERATOR.randNum(1, 2);
		break;
	case HELMET:
		_magnitude = GENERATOR.randNum(1, 2);
		break;
	default:

		break;
	}
}

bool Armor::operator==(const Armor& rhs)
{
	return _armorType == rhs._armorType &&
		_magnitude == rhs._magnitude &&
		_lvl == rhs._lvl &&
		_worth == rhs._worth;
}

std::ostream& operator<<(std::ostream& os, const Armor& a)
{
	switch (a._armorType) {
	case ARMOR_NULL:
		os << "";
		break;
	case CHESTPLATE:
		os << "Chestplate";
		break;
	case GAUNTLETS:
		os << "Gauntlets";
		break;
	case PAULDRONS:
		os << "Pauldrons";
		break;
	case GREAVES:
		os << "Greaves";
		break;
	case BOOTS:
		os << "Boots";
		break;
	case HELMET:
		os << "Helmet";
		break;
	default:
		os << "Unknown Armor";
		break;
	}
	return os;
}

Potion::Potion(int pType, int pMag)
	:Item()
{
	_potionType = (PotionType)pType;
	_potionMag = (PotionMag)pMag;

	if (pType == HEALING) {
		if (pMag == MINOR)
			_magnitude = 10;
		else if (pMag == MAJOR)
			_magnitude = 20;
	}

	_worth = _magnitude * 2;
}

bool Potion::operator==(const Potion& rhs)
{
	return _potionType == rhs._potionType &&
		_potionMag == rhs._potionMag;
}

std::ostream& operator<<(std::ostream& os, const Potion& p)
{
	os << "Potion of ";

	if (p._potionMag == MINOR)
		os << "Minor ";
	else
		os << "Major ";

	switch (p._potionType) {
	case POTION_NULL:
		os << "";
		break;
	case HEALING:
		os << "Healing";
		break;
	default:
		os << "Unknown Effect";
		break;
	}

	return os;
}

Weapon::Weapon(int wType)
	:Item()
{
	WeaponType wtype = (WeaponType)wType;
	switch (wtype) {
	case WEAPON_NULL:
		_magnitude = 0;
		break;
	case DAGGER:
		_magnitude = GENERATOR.randNum(3, 5);
		_lvl = 1;
		break;
	case SWORD_SHORT:
		_magnitude = GENERATOR.randNum(5, 6);
		_lvl = 2;
		break;
	case SWORD_LONG:
		_magnitude = GENERATOR.randNum(6, 11);
		_lvl = 3;
		break;
	case AXE_BATTLE:
		_magnitude = GENERATOR.randNum(11, 13);
		_lvl = 4;
		break;
	case AXE_WAR:
		_magnitude = GENERATOR.randNum(7, 9);
		_lvl = 3;
		break;
	default:
		//add error handling
		break;
	}

	_weaponType = wtype;
	_worth = _magnitude * 3;
}

bool Weapon::operator==(const Weapon& rhs)
{
	return _weaponType == rhs._weaponType &&
		_magnitude == rhs._magnitude &&
		_lvl == rhs._lvl &&
		_worth == rhs._worth;
}

std::ostream& operator<<(std::ostream& os, const Weapon& w)
{
	switch (w._weaponType) {
	case WEAPON_NULL:
		os << "";
		break;
	case DAGGER:
		os << "Dagger";
		break;
	case SWORD_SHORT:
		os << "Short Sword";
		break;
	case SWORD_LONG:
		os << "Long Sword";
		break;
	case AXE_BATTLE:
		os << "Battle Axe";
		break;
	case AXE_WAR:
		os << "War Axe";
		break;
	default:
		os << "Unknown Weapon";
		break;
	}
	return os;
}

