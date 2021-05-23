#ifndef TEXTURE_H
#define TEXTURE_H

#include "Renderer.h"

class Texture
{
public:
	Texture(SDL_Texture* sdlTex=nullptr, SDL_Rect* rect=NULL);
	~Texture();

	void Render(Renderer* renderer, const int& x, const int& y, SDL_Rect* clip=nullptr);

	bool loadFromFile(const std::string& path);

	void Free();

	SDL_Texture* getTexture() const { return _tex; }

	int Width()  { return _width;  }
	int Height() { return _height; }
private:
	SDL_Texture* _tex;
	int _width, _height;

};

#endif // TEXTURE_H