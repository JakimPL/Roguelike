#include "src/constants.hpp"
#include "src/game.hpp"
#include "src/log.hpp"
#include "forms/itemeditor.hpp"

#include <sstream>

#ifdef ITEM_EDITOR
#include <QApplication>
#endif

#ifdef ITEM_EDITOR
int main(int argc, char *argv[])
{
	_LogNone("Item editor starts");
	QApplication application(argc, argv);
	ItemEditor itemEditor;
	itemEditor.show();
	_LogNone("Item editor ends");
	return application.exec();
}
#else
int main()
{
	_LogNone("Game starts");
	Game game;
	game.mainLoop();
	game.quit();
	_LogNone("Game ends");
	return OK;
}
#endif


