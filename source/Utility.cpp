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
	return sqrt(pow(_second.x - _first.x, 2) + pow(_second.y - _first.y, 2) + pow(_second.y - _first.y, 2));
}

bool Utility::is_digit(char _c)
{
	if (_c >= '0' && _c <= '9')
		return true;
	return false;
}