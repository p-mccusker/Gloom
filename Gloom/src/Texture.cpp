#include "Texture.h"

Texture::Texture(SDL_Texture* sdlTex, SDL_Rect* rect)
{
	_tex = std::move(sdlTex);

	if (rect != nullptr) {
		_width = rect->w;
		_height = rect->h;
	}

}

Texture::~Texture()
{
	SDL_DestroyTexture(_tex);
}

void Texture::Render(Renderer* renderer, const int& x, const int& y, SDL_Rect* clip)
{
	SDL_Rect renderQuad = { x, y, _width, _height };

	if (clip != nullptr) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	renderer->Draw(_tex, clip, &renderQuad);
}

bool Texture::loadFromFile(const std::string& path)
{
	return false;
}

void Texture::Free()
{
}

