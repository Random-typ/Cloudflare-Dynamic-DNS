#include "Logger.h"

inline std::string Logger::logfile;
inline std::ofstream Logger::fileStream;


Logger::Logger(std::string _msg, ErrorLevel _error, std::string _caller)
{
	log(_msg, _error, _caller);
}
std::string Logger::getDateTime()
{
	time_t rawtime;
	tm timeinfo;
	char buffer[80];

	time(&rawtime);

	localtime_s(&timeinfo, &rawtime);

	strftime(buffer, sizeof(buffer), "%d-%m-%Y %H.%M.%S", &timeinfo);
	return buffer;
}

void Logger::log(std::string _msg, ErrorLevel _error, std::string _caller)
{
	// [DATE TIME][CALLER][ERROR LEVEL] Message.

	std::string msg = "[" + getDateTime() + "][" + _caller + "]";
	
	switch (_error)
	{
	case Info:
		msg += "[" GetVarName(Info) "]";
		break;
	case Warning:
		msg += "[" GetVarName(Warning) "]";
		break;
	case Error:
		msg += "[" GetVarName(Error) "]";
		break;
	case Fatal:
		msg += "[" GetVarName(Fatal) "]";
		break;
	case Unknown:
	default:
		msg += "[" GetVarName(Unknown) "]";
		break;
	}
	msg += " " + _msg + "\n";

	if (fileStream.is_open())
	{
		fileStream << msg;
		fileStream.flush();
	}
#ifndef NOWINDOW
	fwrite(msg.data(), msg.size(), 1, stdout);
#endif //NOWINDOW
}

void Logger::setupLogfile(std::string _directory)
{
	if (!std::filesystem::is_directory(_directory))
	{
		std::filesystem::create_directory(_directory);
	}

	logfile = _directory + "/" + getDateTime() + ".txt";
	fileStream.open(logfile);
}
