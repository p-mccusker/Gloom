#ifndef RENDERER_H
#define RENDERER_H

#include "def.h"


enum class RENDERER { ACCELERATED, SOFTWARE };

enum class RenderWindow { GameWin, InvWin, MenuWin, HelpWin };

struct Message;

class Renderer
{
public:
	Renderer(const std::string& title, const int& width=640, const int& height=480, const RENDERER& renderType=RENDERER::ACCELERATED);
	~Renderer();

	SDL_Window* getWindow() { return _gameWin; }
	SDL_Renderer* getRenderer() { return _gameRenderer; }
	SDL_Texture* getTextureFromSurface(SDL_Surface* surface);

	void setDrawColor(const Color& color);
	void setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	void setDrawBlendMode(SDL_BlendMode blendMode);

	void setFont(const char* font, const int& size);
	void setDefaultFont();
	void setFontSize(const int& size);

	void Draw(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_Rect* destRect);
	void DrawAndUpdate(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_Rect* destRect);
	void DrawText(Message& msg, SDL_Window* win=nullptr);
	void DrawRect(const SDL_Rect* rect);
	void DrawRects(const SDL_Rect* rects, const int& count);
	void FillRect(const SDL_Rect* rect);
	void FillRects(const SDL_Rect* rects, const int& count);

	void Update();
	void Clear();

	static inline const char* defaultFont = "Sans.ttf";
	static const int defaultFontSize = 18;
private:
	int _scrnWidth, _scrnHeight;
	Uint32 _renderType;
	SDL_Window* _gameWin;
	SDL_Renderer* _gameRenderer;
	TTF_Font* _font;
	std::string _fontName = defaultFont;
	int _fontSize = defaultFontSize;
};

struct Message {
	Message(const std::string& msg = "", const int& x = 0, const int& y = 0,
		const std::string& fontName = Renderer::defaultFont, const int& fontSize = Renderer::defaultFontSize,
		const SDL_Color& textColor = { 255,255,255 }, const SDL_Rect* src = NULL, const SDL_Rect* dest = NULL);

	~Message() { SDL_FreeSurface(textSurface); textSurface = NULL; }

	std::string text,
		font;

	int			size,
		xPos,
		yPos;
	SDL_Surface* textSurface;
	SDL_Color color;
	SDL_Rect srcRect, destRect;
};

#endif // !RENDERER_H
