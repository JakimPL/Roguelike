QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG -= app_bundle
OBJECTS_DIR = obj

HEADERS += \
	forms/areaeditor.hpp \
	forms/creatureeditor.hpp \
	forms/editorfunctions.hpp \
	forms/itemeditor.hpp \
	src/area.hpp \
	src/color.hpp \
	src/constants.hpp \
	src/creature.hpp \
	src/functions.hpp \
	src/game.hpp \
	src/gameobject.hpp \
	src/graphics.hpp \
	src/item.hpp \
	src/itemobject.hpp \
	src/keyboard.hpp \
	src/log.hpp \
	src/message.hpp \
	src/player.hpp \
	src/structures.hpp \
	src/text.hpp \
	src/timer.hpp


SOURCES += \
	main.cpp \
	forms/areaeditor.cpp \
	forms/creatureeditor.cpp \
	forms/editorfunctions.cpp \
	forms/itemeditor.cpp \
	src/area.cpp \
	src/creature.cpp \
	src/functions.cpp \
	src/game.cpp \
	src/gameobject.cpp \
	src/graphics.cpp \
	src/item.cpp \
	src/itemobject.cpp \
	src/keyboard.cpp \
	src/message.cpp \
	src/player.cpp \
	src/structures.cpp \
	src/text.cpp \
	src/timer.cpp
	
FORMS += \
    forms/areaeditor.ui \
    forms/creatureeditor.ui \
    forms/itemeditor.ui

LIBS += -L/usr/lib -lSDL2 -lSDL2_ttf -lboost_program_options -ldl -lpthread 

DEFINES += \
	COURIER_NEW \
	CONVERT_TXT_TO_STR
