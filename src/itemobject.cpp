#include "itemobject.hpp"
#include "text.hpp"

ItemObject::ItemObject(Item initialItem) : item(initialItem)
{
	nameID = item.getNameID();
	color = item.getColor();
	letter = ITEM_LETTER;
}
