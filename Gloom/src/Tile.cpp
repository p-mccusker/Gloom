#include "Tile.h"

Tile::Tile(const int& x, const int& y, const char& tile)
{
    _x = x;
    _y = y;
    _char = tile;
}

Tile::~Tile()
{

}

EnvTile::EnvTile(const int& x, const int& y, const char& tile)
	:Tile(x, y, tile)
{

}

Entity::Entity(const char& tile, const int& x, const int& y, const std::string& name, const int& hp, 
			   const int& atk, const int& def, const int& lvl, const int& xp, const int& viewDist)
	:Tile(x, y, tile)
{
	_name = name;
	_currentHp = hp;
	_maxHp = _currentHp;
	_attack = atk;
	_defense = def;
	_lvl = lvl;
	_xp = xp;
	_viewDist = viewDist;
	_alive = true;
	_inv = new Inventory(InventoryOwnerType::Entity, (void*)this);
	for (auto slot : _armorSlots)
		slot = new Armor(CHESTPLATE);
	for (auto weapSlot : _weaponSlots)
		weapSlot = new Weapon(WEAPON_NULL);

}

std::tuple<int, int> Entity::Attack(Entity& defender, MsgQueue& log)
{
	int dmg = GENERATOR.randNum(0, _attack);
	int dmgBlocked = GENERATOR.randNum(0, defender.Defense());
	int dmgDealt = dmg - dmgBlocked;

	if (dmgDealt > 0) {
		std::string msg = _name + " attacks " + defender.Name() + " for " + std::to_string(dmgDealt) + " damage.";
		log.Enqueue(msg);
		defender.setCurrentHealth(defender.currentHealth() - dmgDealt);
		if (!defender.isAlive()) {
			std::string deathMsg = defender.Name() + " has died!";
			std::string xpMsg = _name + " has gained " + std::to_string(defender.XP()) + " experience!";
			log.Enqueue(deathMsg);
			log.Enqueue(xpMsg);
			_xp += defender.XP();
			return std::tuple<int, int>(defender.X(), defender.Y());
		}
	}
	else {
		std::string miss = _name + " misses " + defender.Name();
		log.Enqueue(miss);
	}

	return std::tuple<int, int>(-1,-1);
}

void Entity::setCurrentHealth(const int& currHP)
{
	_currentHp = currHP;
	if (currHP < 0)
		_alive = false;
}

void Entity::Unequip(Armor& armor)
{
	//chestplate starts at 8
	int itemIndex = (int)armor.armorType() - 8;
	if (itemIndex > 0) {
		Armor* oldItem = _armorSlots[itemIndex];
		if (*oldItem == armor) {
			_defense -= oldItem->Defense();
			oldItem->toggleEquip();
			_armorSlots[itemIndex] = new Armor(ARMOR_NULL);
		}

	}

}

void Entity::Unequip(Weapon& weapon)
{
	Weapon* oldItem = _weaponSlots[0];
	if (*oldItem == weapon) {
		_attack -= oldItem->Damage();
		oldItem->toggleEquip();
		_weaponSlots[0] = new Weapon(WEAPON_NULL);
	}
}

void Entity::Equip(Weapon& weapon)
{
	if (weapon == *_weaponSlots[0]) 
		Unequip(weapon);
	else {
		Unequip(*_weaponSlots[0]);
		*_weaponSlots[0] = weapon;
		_weaponSlots[0]->toggleEquip();
		_attack += weapon.Damage();
	}
}

void Entity::Equip(Armor& armor)
{
	int itemIndex = (int)armor.armorType() - 8;
	if (armor == *_armorSlots[itemIndex])
		Unequip(armor);
	else {
		Unequip(*_weaponSlots[0]);
		*_armorSlots[itemIndex] = armor;
		_weaponSlots[0]->toggleEquip();
		_defense += armor.Defense();
	}
}

void Entity::Equip(Potion& potion)
{
	auto potionStats = potion.Use();

	if (std::get<0>(potionStats) == HEALING) {

		if (_currentHp == _maxHp)
			return;
		else if (_currentHp + std::get<1>(potionStats) > _maxHp)
			_currentHp = _maxHp;
		else
			_currentHp += std::get<1>(potionStats);
	}
	auto potions = _inv->getPotions();
	_inv->removeItem(potion);
}

void Entity::addItem(Armor& armor)
{
	_inv->addItem(&armor);
}

void Entity::addItem(Weapon& weapon)
{
	_inv->addItem(&weapon);
}

void Entity::addItem(Potion& potion)
{
	_inv->addItem(&potion);
}

std::vector<Armor>& Entity::getArmor()
{
	return _inv->getArmor();
}

std::vector<Weapon>& Entity::getWeapons()
{
	return _inv->getWeapons();
}

std::vector<Potion>& Entity::getPotions()
{
	return _inv->getPotions();
}

Enemy::Enemy(const char& tile, const int& x, const int& y)

{
	switch (tile) {
	case Tile::Rat:
		_char = tile;
		_x = x;
		_y = y;
		_name = "rat";
		_currentHp = _maxHp = 10;
		_attack = 2;
		_defense = 0;
		_lvl = 1;
		_xp = 5;
		break;
	case Tile::Rogue:
		_char = tile;
		_x = x;
		_y = y;
		_name = "Rouge";
		_currentHp = _maxHp = 20;
		_attack = 5;
		_defense = 2;
		_lvl = 2;
		_xp = 15;
		break;
	case Tile::Goblin:
		_char = tile;
		_x = x;
		_y = y;
		_name = "Goblin";
		_currentHp = _maxHp = 20;
		_attack = 4;
		_defense = 2;
		_lvl = 2;
		_xp = 15;
		break;
	case Tile::Ogre:
		_char = tile;
		_x = x;
		_y = y;
		_name = "Ogre";
		_currentHp = _maxHp = 40;
		_attack = 8;
		_defense = 4;
		_lvl = 4;
		_xp = 40;
		break;
	case Tile::Dragon:
		_char = tile;
		_x = x;
		_y = y;
		_name = "Dragon";
		_currentHp = _maxHp = 70;
		_attack = 18;
		_defense = 7;
		_lvl = 10;
		_xp = 100;
		break;
	}
	_viewDist = 5;
}

Player::Player(const int& x, const int& y)
	:Entity(Tile::Player, x, y, "Player", 100, 1, 0, 1, 0, 5)
{
	_currentRoom = nullptr;
}