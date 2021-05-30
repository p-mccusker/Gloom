#ifndef GAME_H
#define GAME_H

#include "Level.h"
#include "TileSet.h"

enum class Direction { Up, Down, Left, Right};
enum class MoveAction { Nothing, Move, Attack, Interact };

class Game
{
public:
    Game(const int& width, const int& height, const std::string& tilesetPath="");
    ~Game();

    void Run();

    void waitForKey(const SDL_KeyCode& key);
    static const int mapWidth = 100;
    static const int mapHeight = 40;
    static const int LogLength = 15;
    
private:
    ////SDL_Surface* tileset;
    std::unique_ptr<TileSet> _tileset = nullptr;
    std::unique_ptr<Texture> _tileTexture = nullptr;
    std::unique_ptr<Renderer> _renderer = nullptr;
    std::unique_ptr<Level> _level = nullptr;
    std::unique_ptr<MsgQueue> _log = nullptr;
    std::unique_ptr<Player> _player = nullptr;
    bool _running = true,
        _seeEntireMap = true,
        _showTutorial = false,
        _exitEntered = false,
        _restart = true,
        _firstRun = true;

    void getTileSetTexture();
};

#endif // GAME_H
