#ifndef __UTILITY
#define __UTILITY

#include "../OpenGL/GLM/vec3.hpp"


namespace LEti { namespace Utility {

	constexpr float PI = 3.141593f;
	constexpr float HALF_PI = PI / 2.0f;
	constexpr float QUARTER_PI = HALF_PI / 2.0f;
	constexpr float DOUBLE_PI = PI * 2.0f;

	float vector_length(const glm::vec3& _vec);

	void shrink_vector_to_1(glm::vec3& _vec);

	bool is_digit(char _c);

}	/*LEti*/	}	/*Utility*/

#endif