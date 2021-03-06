#ifndef TILE_H
#define TILE_H

#include "Texture.h"
#include "MsgQueue.h"
#include "Inventory.h"
#include "Room.h"

//extern random GENERATOR;

class Tile
{
public:
	Tile() { }
	Tile(const char& tile, const int& x, const int& y);
	Tile(const Tile& other);
	~Tile();

	//const Texture* getTexture() const { return _texture; }
	int X() const { return _x; }
	int Y() const { return _y; }
	char Char() const { return _char; }

	void setX(const int& x) { _x = x; }
	void setY(const int& y) { _y = y; }
	void setTile(const char& tile) { _char = tile; }

	bool isTile(const char& tile) { return _char == tile; }

	static const char Uninitialized = '?';
	static const char Floor = '.';
	static const char Wall = '#';
	static const char Stair_Up = '<';
	static const char Stair_Down = '>';
	static const char Space = ' ';
	static const char Chest = '$';
	static const char Player = '@';
	static const char Rat = 'r';
	static const char Rogue = 'R';
	static const char Goblin = 'g';
	static const char Ogre = 'O';
	static const char Dragon = 'D';

protected:
	//Texture* _texture;
	int _x=0, _y=0;
	char _char='?';
	//SDL_Rect _box;
};

class Entity : public Tile
{
public:
	Entity() { _currentRoom = nullptr; }
	Entity(const char& tile, const int& x = 0, const int& y = 0, const std::string& name = "", const int& hp = 0,
		const int& atk = 0, const int& def = 0, const int& lvl = 0, const int& xp = 0, const int& viewDist = 0);
	Entity(const Entity& other);
	~Entity();
	bool isAlive() { return _alive; }

	Coord Attack(Entity& defender, MsgQueue& log);

	void Unequip(Armor& armor);
	void Unequip(Weapon& weapon);
	void Equip(Weapon& weapon);
	void Equip(Armor& armor);
	void Equip(Potion& potion);

	void addItem(Armor armor);
	void addItem(Weapon weapon);
	void addItem(Potion potion);

	std::vector<Armor>& getArmor();
	std::vector<Weapon>& getWeapons();
	std::vector<Potion>& getPotions();

	// Getters
	int Atk() { return _attack; }
	int Defense() { return _defense; }
	int currentHealth() { return _currentHp; }
	int totalHealth() { return _maxHp; }
	int Level() const { return _lvl; }
	int XP() const { return _xp; }
	int viewDistance() const { return _viewDist; }
	std::string Name() const { return _name; }
	Room& currentRoom() { return *_currentRoom; }
	Inventory& Inv() { return _inv; }
	bool isPlayer() { return _char == Tile::Player; }
	bool isNull() { return _char == Tile::Uninitialized; }
	//bad
	bool isEnemy() { return _char != Tile::Uninitialized && _char != Tile::Player; }
	//Setters
	void setAttack(const int& atk) { _attack = atk; }
	void setDefense(const int& def) { _defense = def; }
	void setCurrentHealth(const int& currHP);
	void setTotalHealth(const int& totalHP) { _maxHp = totalHP; }
	void setLevel(const int& lvl) { _lvl = lvl; }
	void setXP(const int& xp) { _xp = xp; }
	void setViewDistance(const int& dist) { _viewDist = dist; }
	void setName(const std::string& name) { _name = name; }
	void setCurrentRoom(Room* room) { _currentRoom = room; }

	Entity& operator=( const Entity& rhs);

	static int const Slot_Chest = 0;
	static int const Slot_Shoulders = 1;
	static int const Slot_Hands = 2;
	static int const Slot_Head = 3;
	static int const Slot_Legs = 4;
	static int const Slot_Feet = 5;
	static int const Slot_Armor_Total = 6;
	static int const Slot_Weapon_1 = 0;
	static int const Slot_Weapon_2 = 1;

protected:
	std::string _name;
	int _attack, _defense, _currentHp, _maxHp,
		_lvl, _xp, _viewDist;
	bool _alive;
	Inventory _inv;
	std::array<Armor*, Slot_Armor_Total> _armorSlots = { nullptr };
	std::array<Weapon*, 2> _weaponSlots = { nullptr };
	Room* _currentRoom = nullptr;
};

class Enemy :
	public Entity
{
public:
	Enemy(const char& tile, const int& x, const int& y);
	~Enemy() { }
private:

};

class Player :
	public Entity
{
public:
	Player(const int& x = 0, const int& y = 0);
	~Player() {}
private:

};


#endif // TILE_H
