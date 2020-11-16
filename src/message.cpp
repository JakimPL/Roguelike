#include "message.hpp"
#include "graphics.hpp"
#include "log.hpp"
#include "timer.hpp"

using namespace Graphics;

Message::Message(std::string message, SDL_Color messageColor, float time) : text(message), color(messageColor)
{
	duration = time;
	_LogMessage(text);
}

void Message::draw(SDL_Renderer* renderer, TTF_Font* font, int index)
{
	unsigned int size = text.size();
	drawRectangle(renderer, COLOR_BLACK, SCREEN_WIDTH / 2 - 0.5f * TILE_WIDTH * size - 4 * SCALE, (4.0f + 1.5f * index) * TILE_HEIGHT - 4 * SCALE, TILE_WIDTH * size + 8 * SCALE, TILE_HEIGHT + 8 * SCALE, false);
	drawRectangle(renderer, COLOR_WHITE, SCREEN_WIDTH / 2 - 0.5f * TILE_WIDTH * size - 4 * SCALE, (4.0f + 1.5f * index) * TILE_HEIGHT - 4 * SCALE, TILE_WIDTH * size + 8 * SCALE, TILE_HEIGHT + 8 * SCALE, true);
	drawRectangle(renderer, GUI_RECTANGLE_COLOR, SCREEN_WIDTH / 2 - 0.5f * TILE_WIDTH * size, (4.0f + 1.5f * index) * TILE_HEIGHT, TILE_WIDTH * size, TILE_HEIGHT);
	drawText(renderer, font, text, color, SCREEN_WIDTH / 2, (4.5f + 1.5f * index) * TILE_HEIGHT, Alignment::Center, Alignment::Center);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderPresent(renderer);
}

bool Message::step()
{
	duration -= 1 / FPS;
	if (duration < 0) {
		return true;
	}

	return false;
}

Messages::Messages()
{
	SDL_RenderSetScale(renderer, SCALE, SCALE);
}

Messages::Messages(SDL_Renderer* sdlRenderer, TTF_Font* ttfFont) : renderer(sdlRenderer), font(ttfFont)
{
	SDL_RenderSetScale(renderer, SCALE, SCALE);
}

void Messages::add(Message message)
{
	if (size() >= MAX_MESSAGES) {
		pop_back();
	}

	push_back(message);
}

void Messages::step()
{
	int items = size();
	for (int index = items - 1; index >= 0; --index) {
		Message* message = &at(index);
		message->draw(renderer, font, items - 1 - index);
		if (message->step()) {
			erase(begin() + index);
		}
	}
}
