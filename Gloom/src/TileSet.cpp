#include "TileSet.h"

TileSet::TileSet() {
    image = NULL;
    tileWidth = 0;
    tileHeight = 0;
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

SDL_Rect& TileSet::GetTileRect(const char& tile)
{
    switch (tile) {
        //bad, hardcoded
    case '?':
        return textureRects[QUESTION];
    case ' ':
        return textureRects[EMPTY];
    case '.':
        return textureRects[PERIOD];
    case '#':
        return textureRects[POUND];
    case '$':
        return textureRects[DOLLAR];
    case '@':
        return textureRects[AT];
    case 'r':
        return textureRects[R_LOW];
    case 'R':
        return textureRects[R_UP];
    case 'g':
        return textureRects[G_LOW];
    case 'O':
        return textureRects[O_UP];
    case 'D':
        return textureRects[D_UP];
    case '>':
        return textureRects[GREATER_THAN];
    default:
        return textureRects[EMPTY];
    }
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

        SDL_PixelFormat* fmt;
        SDL_Color* color;
        Uint8 red,
            green,
            blue,
            alpha;
        fmt = image->format;

        SDL_LockSurface(image);
        Uint32* pixels = (Uint32*)image->pixels;

        SDL_GetRGBA(pixels[0], fmt, &red, &green, &blue, &alpha);

        SDL_UnlockSurface(image);

        SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGBA(fmt, red, green, blue, alpha));

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