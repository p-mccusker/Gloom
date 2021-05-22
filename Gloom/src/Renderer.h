#ifndef RENDERER_H
#define RENDERER_H

#include "def.h"


enum class RENDERER { ACCELERATED, SOFTWARE };

class Renderer
{
public:
	Renderer(const std::string& title, const int& width=640, const int& height=480, const RENDERER& renderType=RENDERER::ACCELERATED);
	~Renderer();

	SDL_Window* getWindow() { return _gameWin; }
	SDL_Renderer* getRenderer() { return _gameRenderer; }
	SDL_Texture* getTextureFromSurface(SDL_Surface* surface);

	void Init();
	void Update();
	void Clear();

	void Draw(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_Rect* destRect);
	void DrawAndUpdate(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_Rect* destRect);
	void setDrawColor(const Color& color);
	void setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	void setDrawBlendMode(SDL_BlendMode blendMode);
	void DrawRect(const SDL_Rect* rect);
	void DrawRects(const SDL_Rect* rects, const int& count);
	void FillRect(const SDL_Rect* rect);
	void FillRects(const SDL_Rect* rects, const int& count);


private:
	int _scrnWidth, _scrnHeight;
	Uint32 _renderType;
	SDL_Window* _gameWin;
	SDL_Renderer* _gameRenderer;
};

#endif // !RENDERER_H
