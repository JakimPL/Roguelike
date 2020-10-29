#include "main.hpp"
#include "src/constants.hpp"
#include "src/game.hpp"
#include "src/log.hpp"
#include "src/text.hpp"

#include <sstream>

int main()
{
	_LogNone("Game starts");

	// init SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		_LogError("Error initializing SDL: " << SDL_GetError());
		return SDL_ERROR;
	}

	SDL_Window* window = SDL_CreateWindow("Roguelike", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	Uint32 renderFlags = SDL_RENDERER_ACCELERATED;
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, renderFlags);

	// init TTF
	if (TTF_Init() == -1) {
		_LogError("Error initializing TTF: " << TTF_GetError());
		return TTF_ERROR;
	}

	// load a font
	_LogInfo("Opening font file " << PATH_FONT);
	TTF_Font* font = TTF_OpenFont(PATH_FONT, FONT_SIZE);
	if (font == NULL) {
		_LogError("Failed to load font file!");
		return FONT_ERROR;
	}

	// initialize game objects
	Game game;
	Text text;

	// main game loop
	bool quit = false;
	SDL_Event event;
	while (!quit) {
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) {
				quit = true;
			}
		}

		SDL_RenderClear(renderer);

		// draw game map
		size_t width = game.currentArea.getWidth();
		size_t height = game.currentArea.getHeight();
		for (size_t y = 0; y < height; ++y) {
			for (size_t x = 0; x < width; ++x) {
				Tile tile = game.currentArea.getTile(x, y);
				Functions::drawLetter(renderer, font, tile.letter, tile.color, x, y);
			}
		}

		// draw player
		Position position = game.player.getPosition();
		Functions::drawLetter(renderer, font, game.player.creature.getLetter(), game.player.creature.getColor(), position.x, position.y);

		// draw GUI
		Functions::drawRectangle(renderer, GUI_X_OFFSET, GUI_Y_OFFSET, SCREEN_WIDTH - 2 * GUI_X_OFFSET, TILE_HEIGHT * 3, GUI_RECTANGLE);
		Functions::drawRectangle(renderer, GUI_X_OFFSET, SCREEN_HEIGHT - GUI_Y_OFFSET - 3 * TILE_HEIGHT, SCREEN_WIDTH - 2 * GUI_X_OFFSET, TILE_HEIGHT * 3, GUI_RECTANGLE);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderPresent(renderer);
	}

	// free SDL and TTF
	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	_LogNone("Game ends");
	return OK;
}
