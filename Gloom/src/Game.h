#ifndef GAME_H
#define GAME_H

#include "Level.h"
#include "TileSet.h"

class Game
{
public:
    Game(const int& width, const int& height, const std::string& tilesetPath="");
    ~Game();

    void Run();

    static const int mapWidth = 100;
    static const int mapHeight = 40;
private:
    ////SDL_Surface* tileset;
    TileSet* _tileset = NULL;
    Texture* _tileTexture = NULL;
    Renderer* _renderer = NULL;
    Level* _level = NULL;
    MsgQueue* _log = NULL;

    void getTileSetTexture();
};

#endif // GAME_H
