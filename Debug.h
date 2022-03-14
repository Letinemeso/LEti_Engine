#ifndef DEBUG_FILE
#define DEBUG_FILE

#ifdef LETI_DEBUG 
	#undef LETI_DEBUG 
#endif
#define LETI_DEBUG true

#ifdef LETI_DEBUG 
	#if LETI_DEBUG == true
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

		#ifdef ASSERT
			#undef ASSERT
		#endif
		#define ASSERT(condition) if(condition) { int a = 1; a /= 0; } 1 == 1
	#else	//LETI_DEBUG == true
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

		#ifdef ASSERT
			#undef ASSERT
		#endif
		#define ASSERT(condition)
	#endif	//LETI_DEBUG == true
#endif	//LETI_DEBUG 

#endif	//DEBUG_FILE
