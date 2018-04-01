#pragma once

#include <utility>
#include "sdl2/SDL.h"

template<typename T, typename... Args>
void cleanup_sdl(T *t, Args&&... args)
{
	// cleanup first argument
	cleanup(t);
	// compile time recursive cleanup
	cleanup(std::forward<Agrs>(args)...);
}

template<>
inline void cleanup_sdl<SDL_Window>(SDL_Window* window)
{
	if (!window) 
		return;
	SDL_DestroyWindow(window);
}

template<>
inline void cleanup_sdl<SDL_Renderer>(SDL_Renderer *renderer)
{
	if (!renderer)
		return;
	SDL_DestroyRenderer(renderer);
}

template<>
inline void cleanup_sdl<SDL_Texture>(SDL_Texture *texture)
{
	if (!texture)
		return;
	SDL_DestroyTexture(texture);
}

template<>
inline void cleanup_sdl<SDL_Surface>(SDL_Surface *surface)
{
	if (!surface)
		return;
	SDL_FreeSurface(surface);
}
