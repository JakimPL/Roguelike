#include "message.hpp"
#include "graphics.hpp"
#include "log.hpp"
#include "timer.hpp"

using namespace Graphics;

Message::Message(std::string messageText, SDL_Color messageColor, float time) : text(messageText), color(messageColor)
{
	duration = time;
	_LogMessage(text);
}

void Message::draw(SDL_Renderer* renderer, TTF_Font* font, int index)
{
	unsigned int size = text.size();
	drawRectangle(renderer, COLOR_BLACK, SCREEN_WIDTH / 2 - 0.5f * TILE_WIDTH * size - 4 * options.general.scale, (4.0f + 1.5f * index) * TILE_HEIGHT - 4 * options.general.scale, TILE_WIDTH * size + 8 * SCALE, TILE_HEIGHT + 8 * options.general.scale, false);
	drawRectangle(renderer, COLOR_WHITE, SCREEN_WIDTH / 2 - 0.5f * TILE_WIDTH * size - 4 * options.general.scale, (4.0f + 1.5f * index) * TILE_HEIGHT - 4 * options.general.scale, TILE_WIDTH * size + 8 * SCALE, TILE_HEIGHT + 8 * options.general.scale, true);
	drawRectangle(renderer, GUI_RECTANGLE_COLOR, SCREEN_WIDTH / 2 - 0.5f * TILE_WIDTH * size, (4.0f + 1.5f * index) * TILE_HEIGHT, TILE_WIDTH * size, TILE_HEIGHT);
	drawText(renderer, font, text, color, SCREEN_WIDTH / 2, (4.5f + 1.5f * index) * TILE_HEIGHT, Alignment::Center, Alignment::Center);
}

bool Message::step()
{
	duration -= 1 / options.general.fps;
	if (duration < 0) {
		return true;
	}

	return false;
}

Messages::Messages()
{
	SDL_RenderSetScale(renderer, options.general.scale, options.general.scale);
}

Messages::Messages(SDL_Renderer* sdlRenderer, SDL_Texture* sdlTexture, TTF_Font* ttfFont) : renderer(sdlRenderer), texture(sdlTexture), font(ttfFont)
{
	SDL_RenderSetScale(renderer, options.general.scale, options.general.scale);
}

void Messages::add(std::string messageText, SDL_Color messageColor, float time)
{
	add(Message(messageText, messageColor, time));
}

void Messages::add(Message message)
{
	if (size() >= options.messages.maxMessages) {
		pop_back();
	}

	push_back(message);
}

void Messages::step()
{
	SDL_SetRenderTarget(renderer, texture);
	SDL_RenderSetScale(renderer, options.general.scale, options.general.scale);
	SDL_RenderClear(renderer);

	int items = size();
	for (int index = items - 1; index >= 0; --index) {
		Message* message = &at(index);
		message->draw(renderer, font, items - 1 - index);
		if (message->step()) {
			erase(begin() + index);
		}
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_SetRenderTarget(renderer, NULL);
}
