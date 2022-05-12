#include "../include/Utility.h"

#include <cmath>

using namespace LEti;

float Utility::vector_length(const glm::vec3& _vec)
{
	float sum = 0.0f;
	for (unsigned int i = 0; i < 3; ++i)
		sum += _vec[i] * _vec[i];
	return sqrt(sum);
}

void Utility::shrink_vector_to_1(glm::vec3& _vec)
{
	float vec_length = Utility::vector_length(_vec);
	if (vec_length < 0.000001f) return;
	_vec /= vec_length;
}

float Utility::get_distance(const glm::vec3& _first, const glm::vec3& _second)
{
	if (_first == _second)
		return 0.0f;
    return sqrt(pow(_second.x - _first.x, 2) + pow(_second.y - _first.y, 2) + pow(_second.z - _first.z, 2));
}

glm::vec3 Utility::normalize(const glm::vec3& _first, const glm::vec3& _second)
{
    return {_first.y * _second.z - _second.y * _first.z, -(_first.x * _second.z - _second.x * _first.z), _first.x * _second.y - _second.x * _first.y};
}

float Utility::mixed_vector_multiplication(const glm::vec3& _first, const glm::vec3& _second, const glm::vec3& _third)
{
    return (_first.x * _second.y * _third.z) + (_first.y * _second.z * _third.x) + (_first.z * _second.x * _third.y)
            - (_first.z * _second.y * _third.x) - (_first.y * _second.x * _third.z) - (_first.x * _second.z * _third.y);
}

bool Utility::is_digit(char _c)
{
	if (_c >= '0' && _c <= '9')
		return true;
	return false;
}

float Utility::angle_cos_between_vectors(const glm::vec3& _first, const glm::vec3& _second)
{
    ASSERT(Utility::vector_length(_first) == 0 ||  Utility::vector_length(_second) == 0);

    //  (_first * _second) / (|_first| * |_second|)
    float divident = _first.x * _second.x + _first.y * _second.y + _first.z * _second.z;
    float divider = Utility::vector_length(_first) * Utility::vector_length(_second);
    return divident / divider;
}

bool Utility::beams_cross_at_right_angle(const glm::vec3& _first, const glm::vec3& _second)
{
    ASSERT(Utility::vector_length(_first) == 0 ||  Utility::vector_length(_second) == 0);

    //  (_first * _second) / (|_first| * |_second|)
    float divident = _first.x * _second.x + _first.y * _second.y + _first.z * _second.z;

    return divident < 0.0f; // angles cos must be negative
}
