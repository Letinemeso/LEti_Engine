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
