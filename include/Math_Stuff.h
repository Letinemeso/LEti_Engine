#pragma once

#include "L_Debug/L_Debug.h"

#include "vec3.hpp"
#include "mat4x4.hpp"
#include <gtx/transform.hpp>

#include <cmath>
#include <utility>


namespace LEti {

namespace Math {

    constexpr float PI = 3.141593f;
    constexpr float HALF_PI = PI / 2.0f;
    constexpr float QUARTER_PI = HALF_PI / 2.0f;
    constexpr float DOUBLE_PI = PI * 2.0f;

    float vector_length(const glm::vec3& _vec);

    void shrink_vector_to_1(glm::vec3& _vec);

    void extend_vector_to_length(glm::vec3& _vec, float _length);

    glm::vec3 extend_vector_to_length(const glm::vec3& _vec, float _length);

    float get_distance(const glm::vec3& _first, const glm::vec3& _second);

    float dot_product(const glm::vec3& _first, const glm::vec3& _second);

    glm::vec3 cross_product(const glm::vec3& _first, const glm::vec3& _second);

    glm::vec3 rotate_vector(const glm::vec3& _vector, const glm::vec3& _axis, float _angle);

    float mixed_vector_multiplication(const glm::vec3& _first, const glm::vec3& _second, const glm::vec3& _third);

    bool is_digit(char _c);

    float angle_cos_between_vectors(const glm::vec3& _first, const glm::vec3& _second);

    float angle_sin_between_vectors(const glm::vec3& _first, const glm::vec3& _second);

    float sin_or_cos_from_opposite(float _cos_or_sin);

    bool beams_cross_at_right_angle(const glm::vec3& _first, const glm::vec3& _second);

	bool floats_are_equal(float _first, float _second, float _precision = 0.0001f);

    int float_to_int(float _value);

	bool vecs_are_equal(const glm::vec3& _first, const glm::vec3& _second);

    unsigned int random_number(unsigned int _lower_limimt = 0, unsigned int _upper_limit = 0xFFFFFFFF);

}	/*Math*/

}	/*LEti*/
