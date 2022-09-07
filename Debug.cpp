#include "Debug.h"

#if LETI_DEBUG == true

std::map<std::string, bool> ___AVAILIBLE_LOG_LEVELS;

void ___debug_create_log_level(const std::string& _level_name)
{
	if(___AVAILIBLE_LOG_LEVELS.find(_level_name) == ___AVAILIBLE_LOG_LEVELS.end())
		___AVAILIBLE_LOG_LEVELS.emplace(_level_name, false);
}

void ___debug_remove_log_level(const std::string& _level_name)
{
	std::map<std::string, bool>::const_iterator it = ___AVAILIBLE_LOG_LEVELS.find(_level_name);
	if(it != ___AVAILIBLE_LOG_LEVELS.end())
		___AVAILIBLE_LOG_LEVELS.erase(it);
}

void ___debug_log(const std::string& _level_name, const std::string& _message)
{
	if(___AVAILIBLE_LOG_LEVELS.find(_level_name) != ___AVAILIBLE_LOG_LEVELS.end())
		std::cout << '[' << _level_name << "]: " << _message << "\n";
}

#endif
