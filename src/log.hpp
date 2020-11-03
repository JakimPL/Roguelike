#ifndef LOG_H
#define LOG_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#define dRED        "\x1b[0;31;40m"
#define dGREEN      "\x1b[0;32;40m"
#define dYELLOW     "\x1b[0;33;40m"
#define dBLUE       "\x1b[0;34;40m"
#define dMAGENTA    "\x1b[0;35;40m"
#define dCYAN       "\x1b[0;36;40m"
#define dWHITE      "\x1b[0;37;40m"

#define lRED		"\033[1;31m"
#define lGREEN		"\033[1;32m"
#define lYELLOW		"\033[1;33m"
#define lBLUE		"\033[1;34m"
#define lMAGENTA	"\033[1;35m"
#define lCYAN		"\033[1;36m"
#define lWHITE		"\033[1;37m"

#define RESET 	    "\x1b[0;7;0m"

#define _Log(msg, type)  std::cout << Log::getCurrentDateTime() << ": " << Log::typeText(type) << msg << "\n"
#define _LogNone(msg)    _Log(msg, LogType::NONE)
#define _LogMessage(msg) _Log(msg, LogType::MESSAGE)
#define _LogInfo(msg)    _Log(msg, LogType::INFO)
#define _LogWarning(msg) _Log(msg, LogType::WARNING)
#define _LogError(msg)   _Log(msg, LogType::ERROR)

enum class LogType {
	NONE,
	MESSAGE,
	INFO,
	WARNING,
	ERROR,
};

namespace Log
{

inline std::string getCurrentDateTime()
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[86];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
	return std::string(buf);
};

inline void logToFile(std::ostringstream message, bool date = true)
{
	std::string filePath = "game.log";
	std::string now = getCurrentDateTime();
	std::ofstream ofs(filePath.c_str(), std::ios_base::out | std::ios_base::app);
	if (date) {
		ofs << now << '\t';
	}
	ofs << message.str() << '\n';
	ofs.close();
}

inline std::string typeText(LogType type)
{
	std::ostringstream text;
	switch (type) {
	case LogType::NONE:
		text << "          ";
		break;
	case LogType::MESSAGE:
		text << "[" << lGREEN << "MESSAGE" << RESET << "] ";
		break;
	case LogType::INFO:
		text << "   [" << lWHITE << "INFO" << RESET << "] ";
		break;
	case LogType::WARNING:
		text << "[" << lYELLOW << "WARNING" << RESET << "] ";
		break;
	case LogType::ERROR:
		text << "  [" << lRED << "ERROR" << RESET << "] ";
		break;
	}

	return text.str();
}

};

#endif
