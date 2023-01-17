#ifndef DEBUG_FILE
#define DEBUG_FILE

#ifdef LETI_DEBUG 
	#undef LETI_DEBUG 
#endif
#define LETI_DEBUG true

#ifdef LETI_DEBUG 
	#if LETI_DEBUG == true

		#include <iostream>
		#include <map>

		//	USE THIS ONLY WITH MACRO BELOW
		void ___debug_create_log_level(const std::string& _level_name);
		void ___debug_remove_log_level(const std::string& _level_name);
		void ___debug_log(const std::string& _level_name, const std::string& _message);

		#ifdef LOG
			#undef LOG
		#endif
		#define LOG(level, message) ___debug_log(level, message)

		#ifdef CREATE_LOG_LEVEL
			#undef CREATE_LOG_LEVEL
		#endif
		#define CREATE_LOG_LEVEL(level) ___debug_create_log_level(level)

		#ifdef REMOVE_LOG_LEVEL
			#undef REMOVE_LOG_LEVEL
		#endif
		#define REMOVE_LOG_LEVEL(level) ___debug_remove_log_level(level)

		#ifdef DEBUG_FUNC_NOARG
			#undef DEBUG_FUNC_NOARG
		#endif
		#define DEBUG_FUNC_NOARG(func) func()

		#ifdef DEBUG_FUNC_1ARG
			#undef DEBUG_FUNC_1ARG
		#endif
		#define DEBUG_FUNC_1ARG(func, arg) func(arg)

		#ifdef DEBUG_FUNC_2ARG
			#undef DEBUG_FUNC_2ARG
		#endif
		#define DEBUG_FUNC_2ARG(func, arg1, arg2) func(arg1, arg2)

		#ifdef L_ASSERT
			#undef L_ASSERT
		#endif
		#define L_ASSERT(condition) if(!(condition)) { int a = 1; a /= 0; } 1 == 1


	#else	//LETI_DEBUG == true
		#ifdef LOG
			#undef LOG
		#endif
		#define LOG(level, message) {}

		#ifdef CREATE_LOG_LEVEL
			#undef CREATE_LOG_LEVEL
		#endif
		#define CREATE_LOG_LEVEL(level) {}

		#ifdef REMOVE_LOG_LEVEL
			#undef REMOVE_LOG_LEVEL
		#endif
		#define REMOVE_LOG_LEVEL(level) {}

		#ifdef DEBUG_FUNC_NOARG
			#undef DEBUG_FUNC_NOARG
		#endif
		#define DEBUG_FUNC_NOARG(func) 

		#ifdef DEBUG_FUNC_1ARG
			#undef DEBUG_FUNC_1ARG
		#endif
		#define DEBUG_FUNC_1ARG(func, arg) 

		#ifdef DEBUG_FUNC_2ARG
			#undef DEBUG_FUNC_2ARG
		#endif
		#define DEBUG_FUNC_2ARG(func, arg1, arg2) 

		#ifdef L_ASSERT
			#undef L_ASSERT
		#endif
		#define L_ASSERT(condition)
	#endif	//LETI_DEBUG == true
#endif	//LETI_DEBUG 

#endif	//DEBUG_FILE
