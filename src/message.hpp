#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "constants.hpp"
#include "color.hpp"

#include <string>
#include <vector>

#include <SDL2/SDL_ttf.h>

class Message
{
private:
	std::string text;
	SDL_Color color;
	float duration;

public:
	Message(std::string messageText, SDL_Color messageColor = COLOR_WHITE, float time = MESSAGE_DURATION);
	void draw(SDL_Renderer* renderer, TTF_Font* font, int index);
	bool step();
};

class Messages : std::vector<Message>
{
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	TTF_Font* font;
public:
	Messages();
	Messages(SDL_Renderer* sdlRenderer, SDL_Texture* sdlTexture, TTF_Font* ttfFont);
	void add(std::string messageText, SDL_Color messageColor = COLOR_WHITE, float time = MESSAGE_DURATION);
	void add(Message message);
	void step();
};

#endif // MESSAGE_HPP
