#include "TileSet.h"

TileSet::TileSet() {
    image = NULL;
    tileWidth = -1;
    tileHeight = -1;
    path = "";
}

TileSet::TileSet(const std::string& path, const int& width, const int& height) {
    //Load image at specified path
    image = IMG_Load(path.c_str());

    if (image == NULL) {
        printf("IMG_Load: %s\n", IMG_GetError());
        // handle error
    }
    else {
        this->path = path;
        tileWidth = width;
        tileHeight = height;

        SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 0, 0xFF, 0xFF));

        for (int i = 0; i < 128; i++) {
            textureRects[i].x = (i % 16) * tileWidth;
            textureRects[i].y = int(i / 16) * tileHeight;
            textureRects[i].w = tileWidth;
            textureRects[i].h = tileHeight;
        }
    }
}

TileSet::~TileSet() {
    SDL_FreeSurface(image);
    image = NULL;
    path = "";
    tileWidth = tileHeight = -1;
}

void TileSet::Load(const std::string& path, const int& width, const int& height) {
    this->Free();

    image = IMG_Load(path.c_str());

    if (image == NULL) {
        printf("IMG_Load: %s\n", IMG_GetError());
        // handle error
    }
    else {
        this->path = path;
        tileWidth = width;
        tileHeight = height;

        SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 0, 0xFF, 0xFF));

        for (int i = 0; i < 128; i++) {
            textureRects[i].x = (i % 16) * tileWidth;
            textureRects[i].y = int(i / 16) * tileHeight;
            textureRects[i].w = tileWidth;
            textureRects[i].h = tileHeight;
        }
    }
}

inline void TileSet::Free()
{
    SDL_FreeSurface(image);
    image = NULL;
    path = "";
    tileWidth = tileHeight = -1;
}