#ifndef __UTILITY
#define __UTILITY

#include "../OpenGL/GLM/vec3.hpp"


namespace LEti { namespace Utility {

	constexpr float HALF_PI = 3.141593f / 2.0f;
	constexpr float PI = 3.141593f;
	constexpr float DOUBLE_PI = 3.141593f * 2.0f;

	float vector_length(const glm::vec3& _vec);

}	/*LEti*/	}	/*Utility*/

#endif