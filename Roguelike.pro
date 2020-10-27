QT -= gui
CONFIG -= app_bundle
OBJECTS_DIR = obj

HEADERS += \
	main.hpp \
	src/constants.hpp \
	src/functions.hpp \
	src/game.hpp \
	src/log.hpp \
	src/player.hpp \
	src/structures.hpp \
	src/text.hpp


SOURCES += \
        main.cpp \
		src/functions.cpp \
	src/game.cpp \
        src/player.cpp \
	src/structures.cpp \
	src/text.cpp

LIBS += -L/usr/lib -lSDL2 -ldl -lpthread
