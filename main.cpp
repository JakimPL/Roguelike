#include "src/constants.hpp"
#include "src/functions.hpp"
#include "src/game.hpp"
#include "src/log.hpp"
#include "forms/itemeditor.hpp"

#include <sstream>

#include <QApplication>

int main(int argc, char *argv[])
{
	Mode mode = Functions::parseProgramArguments(argc, argv);

	switch (mode) {
	case Mode::Help:
	case Mode::Version: {
		return OK;
	}
	case Mode::ItemEditor: {
		_LogNone("Item editor starts");
		QApplication application(argc, argv);
		ItemEditor itemEditor;
		itemEditor.show();
		_LogNone("Item editor ends");
		return application.exec();
	}
	default: {
		_LogNone("Game starts");
		Game game;
		game.mainLoop();
		game.quit();
		_LogNone("Game ends");
		return OK;
	}
	}
}
