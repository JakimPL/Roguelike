#include "src/constants.hpp"
#include "src/functions.hpp"
#include "src/game.hpp"
#include "src/log.hpp"
#include "forms/areaeditor.hpp"
#include "forms/creatureeditor.hpp"
#include "forms/dialogeditor.hpp"
#include "forms/itemeditor.hpp"

#include <sstream>

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication application(argc, argv);
	std::vector<Mode> modes = Functions::parseProgramArguments(argc, argv);

	for (Mode mode : modes) {
		switch (mode) {
		case Mode::Help:
		case Mode::Version: {
			break;
		}
		case Mode::AreaEditor: {
			_LogNone("Area editor starts");
			AreaEditor areaEditor;
			areaEditor.show();
			application.exec();
			break;
		}
		case Mode::CreatureEditor: {
			_LogNone("Creature editor editor starts");
			CreatureEditor creatureEditor;
			creatureEditor.show();
			application.exec();
			break;
		}
		case Mode::DialogEditor: {
			_LogNone("Dialog editor editor starts");
			DialogEditor dialogEditor;
			dialogEditor.show();
			application.exec();
			break;
		}
		case Mode::ItemEditor: {
			_LogNone("Item editor starts");
			ItemEditor itemEditor;
			itemEditor.show();
			application.exec();
			break;
		}
		case Mode::Game: {
			_LogNone("Game starts");
			Game game;
			game.mainLoop();
			game.quit();
			_LogNone("Game ends");
			break;
		}
		default:
			break;
		}
	}

	return 0;
}
