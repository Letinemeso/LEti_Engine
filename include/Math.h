#ifndef __UTILITY
#define __UTILITY

#include "../Debug.h"

#include "../OpenGL/GLM/vec3.hpp"


namespace LEti { namespace Math {

	constexpr float PI = 3.141593f;
	constexpr float HALF_PI = PI / 2.0f;
	constexpr float QUARTER_PI = HALF_PI / 2.0f;
	constexpr float DOUBLE_PI = PI * 2.0f;

	float vector_length(const glm::vec3& _vec);

	void shrink_vector_to_1(glm::vec3& _vec);

	float get_distance(const glm::vec3& _first, const glm::vec3& _second);

    glm::vec3 normalize(const glm::vec3& _first, const glm::vec3& _second);

    float mixed_vector_multiplication(const glm::vec3& _first, const glm::vec3& _second, const glm::vec3& _third);

	bool is_digit(char _c);

    float angle_cos_between_vectors(const glm::vec3& _first, const glm::vec3& _second);

    bool beams_cross_at_right_angle(const glm::vec3& _first, const glm::vec3& _second);

	bool floats_are_equal(float _first, float _second);

}	/*LEti*/	}	/*Math*/

#endif
