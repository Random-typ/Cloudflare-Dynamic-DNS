#pragma once
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>

#define GetVarName(_var) (#_var)
class Logger
{
public:
	enum ErrorLevels
	{
		Unknown,
		Info,
		Warning,
		Error,
		Fatal
	} typedef ErrorLevel;

	Logger(std::string _msg, ErrorLevel _error, std::string _caller);
	static std::string getDateTime();
	static void log(std::string _msg, ErrorLevel _error, std::string _caller);
	static void setupLogfile(std::string _directory);

	static std::string logfile;

} typedef LOG;

