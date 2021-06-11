#pragma once
#ifndef TILESET_H
#define TILESET_H

#include "def.h"

struct TileSet {
public:
    TileSet();
    TileSet(const std::string& path, const int& width, const int& height);
    ~TileSet();

    SDL_Rect& GetTileRect(const TexIndex& index) { return textureRects[index]; }
    SDL_Rect& GetTileRect(const char& tile);
    void Load(const std::string& path, const int& width, const int& height);
    void Free();

public:
    SDL_Surface* image;
    int tileWidth, tileHeight;
    std::string path;
    std::array<SDL_Rect, 128> textureRects;
};

#endif