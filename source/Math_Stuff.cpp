#include <Math_Stuff.h>

#include <cmath>

#include "L_Debug/L_Debug.h"

using namespace LEti;


//	LEti::Math (just some universal math functions suitable for both 2d and 3d calculations)

float Math::vector_length(const glm::vec2& _vec)
{
    float sum = vector_length_squared(_vec);
    return sqrtf(sum);
}

float Math::vector_length(const glm::vec3& _vec)
{
    float sum = vector_length_squared(_vec);
    return sqrtf(sum);
}

float Math::vector_length_squared(const glm::vec2& _vec)
{
    float sum = 0.0f;
    for (unsigned int i = 0; i < 2; ++i)
        sum += _vec[i] * _vec[i];
    return sum;
}

float Math::vector_length_squared(const glm::vec3& _vec)
{
    float sum = 0.0f;
    for (unsigned int i = 0; i < 3; ++i)
        sum += _vec[i] * _vec[i];
    return sum;
}

void Math::shrink_vector_to_1(glm::vec2& _vec)
{
    float vec_length = vector_length(_vec);
    if (vec_length < 0.000001f)
        return;
    _vec /= vec_length;
}

void Math::shrink_vector_to_1(glm::vec3& _vec)
{
    float vec_length = vector_length(_vec);
    if (vec_length < 0.000001f)
        return;
    _vec /= vec_length;
}

void Math::extend_vector_to_length(glm::vec2& _vec, float _length)
{
    shrink_vector_to_1(_vec);
    _vec *= _length;
}

void Math::extend_vector_to_length(glm::vec3& _vec, float _length)
{
	shrink_vector_to_1(_vec);
	_vec *= _length;
}

glm::vec3 Math::extend_vector_to_length(const glm::vec3& _vec, float _length)
{
    glm::vec3 result = _vec;
    shrink_vector_to_1(result);
    result *= _length;
    return result;
}

float Math::get_distance(const glm::vec3& _first, const glm::vec3& _second)
{
	if (_first == _second)
		return 0.0f;
	return sqrt(pow(_second.x - _first.x, 2) + pow(_second.y - _first.y, 2) + pow(_second.z - _first.z, 2));
}

float Math::dot_product(const glm::vec3& _first, const glm::vec3& _second)
{
    float result = 0.0f;

    for(unsigned int i=0; i<3; ++i)
        result += _first[i] * _second[i];

    return result;
}

float Math::dot_product(const glm::vec2& _first, const glm::vec2& _second)
{
    float result = 0.0f;

    for(unsigned int i=0; i<2; ++i)
        result += _first[i] * _second[i];

    return result;
}

glm::vec3 Math::cross_product(const glm::vec3& _first, const glm::vec3& _second)
{
    return {
        _first.y * _second.z - _first.z * _second.y,
        _first.z * _second.x - _first.x * _second.z,
        _first.x * _second.y - _first.y * _second.x
    };
}

glm::vec3 Math::rotate_vector(const glm::vec3& _vector, const glm::vec3& _axis, float _angle)
{
	glm::mat4x4 rotation_matrix = glm::rotate(_angle, _axis);

	return rotation_matrix * glm::vec4(_vector, 1.0f);
}

glm::vec3 Math::calculate_projection(const glm::vec3& _project_what, const glm::vec3& _project_on)
{
    float project_on_length_squared = vector_length_squared(_project_on);
    if(project_on_length_squared < 0.0001f)
        return {0.0f, 0.0f, 0.0f};

    float project_on_length = sqrtf(project_on_length_squared);
    float dot = dot_product(_project_what, _project_on);
    float projection_length = dot / project_on_length;

    return _project_on * projection_length;
}

float Math::mixed_vector_multiplication(const glm::vec3& _first, const glm::vec3& _second, const glm::vec3& _third)
{
	return (_first.x * _second.y * _third.z) + (_first.y * _second.z * _third.x) + (_first.z * _second.x * _third.y)
			- (_first.z * _second.y * _third.x) - (_first.y * _second.x * _third.z) - (_first.x * _second.z * _third.y);
}

bool Math::is_digit(char _c)
{
	if (_c >= '0' && _c <= '9')
		return true;
	return false;
}

float Math::angle_cos_between_vectors(const glm::vec3& _first, const glm::vec3& _second)
{
    float length_1 = vector_length(_first),
          length_2 = vector_length(_second);

    if(length_1 == 0 || length_2 == 0)
		return 0.0f;

	float divident = _first.x * _second.x + _first.y * _second.y + _first.z * _second.z;
    float divider = length_1 * length_2;

	float result = divident / divider;

	if(result < -1.0f)
		result = -1.0f;
	else if(result > 1.0f)
		result = 1.0f;

	return result;
}

float Math::angle_sin_between_vectors(const glm::vec3& _first, const glm::vec3& _second)
{
	return sin_or_cos_from_opposite(angle_cos_between_vectors(_first, _second));
}

float Math::sin_or_cos_from_opposite(float _cos_or_sin)
{
	return sqrt(1 - (_cos_or_sin * _cos_or_sin));
}

bool Math::beams_cross_at_right_angle(const glm::vec3& _first, const glm::vec3& _second)
{
	L_ASSERT(!(vector_length(_first) == 0 ||  vector_length(_second) == 0));

	float divident = _first.x * _second.x + _first.y * _second.y + _first.z * _second.z;

	return divident < 0.0f; // angles cos must be negative
}

bool Math::floats_are_equal(float _first, float _second, float _precision)
{
	return fabs(_first-_second) < _precision;
}

int Math::float_to_int(float _value)
{
    int result = (int)_value;

    if(fabs(_value - (float)result) > 0.5f)
        result += _value > 0 ? 1 : -1;

    return result;
}

bool Math::vecs_are_equal(const glm::vec2& _first, const glm::vec2& _second)
{
    return floats_are_equal(_first.x, _second.x) && floats_are_equal(_first.y, _second.y);
}

bool Math::vecs_are_equal(const glm::vec3& _first, const glm::vec3& _second)
{
	return floats_are_equal(_first.x, _second.x) && floats_are_equal(_first.y, _second.y) && floats_are_equal(_first.z, _second.z);
}

unsigned int Math::random_number(unsigned int _lower_limimt, unsigned int _upper_limit)
{
    L_ASSERT(_upper_limit >= _lower_limimt);

    if(_upper_limit == _lower_limimt)
        return _lower_limimt;

    unsigned int limit = _upper_limit - _lower_limimt;
    return rand() % limit + _lower_limimt;
}

float Math::random_number_float(float _lower_limimt, float _upper_limit)
{
    L_ASSERT(_upper_limit >= _lower_limimt);

    if(_upper_limit == _lower_limimt)
        return _lower_limimt;

    float difference = _upper_limit - _lower_limimt;
    float ratio = (float)random_number(0, 10000) / 10000.0f;

    return _lower_limimt + (difference * ratio);
}

bool Math::random_bool()
{
    return rand() % 2 == 0;
}
