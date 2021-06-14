#ifndef GAME_H
#define GAME_H

#include "Level.h"
#include "TileSet.h"

enum class MoveAction { Nothing, Move, Attack, Interact };

class Game
{
public:
    Game(const int& width, const int& height, const std::string& tilesetPath="");
    ~Game();

    static const int mapWidth = 100;
    static const int mapHeight = 40;
    static const int LogLength = 15;

    void Run();
    void handleInput(SDL_Event& e);
    void waitForKey(const SDL_KeyCode& key);
    void Print();
    void Help();
    void Tutorial();
    void Log();
    void Death();
    void Restart(bool restartFromDeath=true);
    void Victory();
    void Exit();
    void InventoryScreen(Inventory* inv=nullptr);
    std::vector<Coord> tilesInView(const Entity& entity);
    
private:
    ////SDL_Surface* tileset;
    std::unique_ptr<TileSet> _tileset = nullptr;
    std::unique_ptr<Texture> _tileTexture = nullptr;
    std::unique_ptr<Renderer> _renderer = nullptr;
    std::unique_ptr<Level> _level = nullptr;
    std::unique_ptr<MsgQueue> _log = nullptr;
    Entity* _player = nullptr;
    bool _running = true,
        _seeEntireMap = true,
        _showTutorial = false,
        _exitEntered = false,
        _restart = true,
        _firstRun = true;

    void getTileSetTexture();
    MoveAction checkUp(const int& x, const int& y);
    MoveAction checkDown(const int& x, const int& y);
    MoveAction checkLeft(const int& x, const int& y);
    MoveAction checkRight(const int& x, const int& y);
    MoveAction moveAction(const int& x, const int& y, const Direction& dir);
    void Move(Entity& entity, const Direction& dir);
    void EntitiesTurn();

};

#endif // GAME_H
