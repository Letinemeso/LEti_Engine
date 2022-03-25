#include "../include/Utility.h"

#include <cmath>

float LEti::Utility::vector_length(const glm::vec3& _vec)
{
	float sum = 0.0f;
	for (unsigned int i = 0; i < 3; ++i)
		sum += _vec[i] * _vec[i];
	return sqrt(sum);
}
