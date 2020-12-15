#ifndef SIGN_HPP
#define SIGN_HPP

#include "gameobject.hpp"

class Sign : public GameObject
{
public:
	Sign(GameObjects& gameObjects, Color initialColor, Position initialPosition, char intialLetter, unsigned int initialNameID);
	Sign(GameObjects& gameObjects, std::ifstream& resource);

	TextPair getText() override;
};

#endif // SIGN_HPP
