#include "itemobject.hpp"
#include "text.hpp"

ItemObject::ItemObject(Item initialItem, Position initialPosition) : item(initialItem)
{
	position = initialPosition;
	nameID = item.getNameID();
	color = item.getColor();
	letter = ITEM_LETTER;
}
