#include "Renderer.h"

Renderer::Renderer(const std::string& title, const int& width, const int& height, const RENDERER& renderType)
{
	_scrnHeight = height;
	_scrnWidth = width;
	_gameWin = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _scrnWidth, _scrnHeight, SDL_WINDOW_RESIZABLE);
	_gameRenderer = SDL_CreateRenderer(_gameWin, -1, _renderType);

	if (renderType == RENDERER::ACCELERATED)
		_renderType = SDL_RENDERER_ACCELERATED;
	else if (renderType == RENDERER::SOFTWARE)
		_renderType = SDL_RENDERER_SOFTWARE;
}

Renderer::~Renderer()
{
	SDL_DestroyRenderer(_gameRenderer);
	SDL_DestroyWindow(_gameWin);
	_gameRenderer = nullptr;
	_gameWin = nullptr;
}

SDL_Texture* Renderer::getTextureFromSurface(SDL_Surface* surface)
{
	return SDL_CreateTextureFromSurface(_gameRenderer, surface);
}

void Renderer::Draw(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_Rect* destRect)
{
	SDL_RenderCopy(_gameRenderer, texture, srcRect, destRect);
}

void Renderer::DrawAndUpdate(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_Rect* destRect)
{
	this->Clear();
	this->Draw(texture, srcRect, destRect);
	this->Update();
}

void Renderer::setDrawColor(const Color& color)
{
	SDL_SetRenderDrawColor(_gameRenderer, color.r, color.g, color.b, color.a);
}

void Renderer::setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_SetRenderDrawColor(_gameRenderer, r, g, b, a);
}

void Renderer::setDrawBlendMode(SDL_BlendMode blendMode)
{
	SDL_SetRenderDrawBlendMode(_gameRenderer, blendMode);
}

void Renderer::DrawRect(const SDL_Rect* rect)
{
	SDL_RenderDrawRect(_gameRenderer, rect);
}

void Renderer::DrawRects(const SDL_Rect* rects, const int& count)
{
	SDL_RenderDrawRects(_gameRenderer, rects, count);
}

void Renderer::FillRect(const SDL_Rect* rect)
{
	SDL_RenderFillRect(_gameRenderer, rect);
}

void Renderer::FillRects(const SDL_Rect* rects, const int& count)
{
	SDL_RenderFillRects(_gameRenderer, rects, count);
}

void Renderer::Update()
{
	SDL_RenderPresent(_gameRenderer);
}

void Renderer::Clear()
{
	SDL_RenderClear(_gameRenderer);
}
