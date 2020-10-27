#include "main.hpp"
#include "src/constants.hpp"
#include "src/game.hpp"
#include "src/log.hpp"
#include "src/text.hpp"

#include <sstream>

int main()
{
	_LogNone("Game starts");

	Game game;
	Text text;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		_LogError("Error initializing SDL: " << SDL_GetError());
	}

	SDL_Window* window = SDL_CreateWindow("Roguelike", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	Uint32 renderFlags = SDL_RENDERER_ACCELERATED;
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, renderFlags);
	SDL_Event event;

	bool quit = false;
	while (!quit) {
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) {
				quit = true;
			}
		}

		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
	}

	_LogNone("Game ends");
	return 0;
}
