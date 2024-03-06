#include "../include/Math_Stuff.h"

using namespace LEti;


//	LEti::Math (just some universal math functions suitable for bot 2d and 3d calculations)

float Math::vector_length(const glm::vec3& _vec)
{
	float sum = 0.0f;
	for (unsigned int i = 0; i < 3; ++i)
		sum += _vec[i] * _vec[i];
	return sqrt(sum);
}

void Math::shrink_vector_to_1(glm::vec3& _vec)
{
	float vec_length = vector_length(_vec);
	if (vec_length < 0.000001f) return;
	_vec /= vec_length;
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
	float length1 = vector_length(_first);
	float length2 = vector_length(_second);
	if(length1 < 0.0001f || length2 < 0.0001f)
		return 0.0f;
	return length1 * length2 * angle_cos_between_vectors(_first, _second);
}

glm::vec3 Math::cross_product(const glm::vec3& _first, const glm::vec3& _second)
{
    return {_first.y * _second.z - _second.y * _first.z, -(_first.x * _second.z - _second.x * _first.z), _first.x * _second.y - _second.x * _first.y};
}

glm::vec3 Math::rotate_vector(const glm::vec3& _vector, const glm::vec3& _axis, float _angle)
{
	glm::mat4x4 rotation_matrix = glm::rotate(_angle, _axis);

	return rotation_matrix * glm::vec4(_vector, 1.0f);
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

bool Math::vecs_are_equal(const glm::vec3& _first, const glm::vec3& _second)
{
	return floats_are_equal(_first.x, _second.x) && floats_are_equal(_first.y, _second.y) && floats_are_equal(_first.z, _second.z);
}

unsigned int Math::random_number(unsigned int _lower_limimt, unsigned int _upper_limit)
{
    L_ASSERT(_upper_limit > _lower_limimt);

    unsigned int limit = _upper_limit - _lower_limimt;
    return rand() % limit + _lower_limimt;
}


//	LEti::Geometry (generic geometry functions and easy-to-use structures)

std::pair<glm::vec3, glm::vec3> Geometry::get_segments_normals(const Segment& _first, const Segment& _second)
{
    glm::vec3 f_dir = _first.direction_vector(), s_dir = _second.direction_vector();
	glm::vec3 axis{0.0f, 0.0f, 1.0f};
	Math::shrink_vector_to_1(axis);
	glm::mat4x4 matrix = glm::rotate(-Math::HALF_PI, axis);

	glm::vec3 f = matrix * glm::vec4(f_dir, 1.0f);
	glm::vec3 s = matrix * glm::vec4(s_dir, 1.0f);
	Math::shrink_vector_to_1(f);
	Math::shrink_vector_to_1(s);

	return { f, s };
}





Geometry_2D::Equasion_Data::Equasion_Data(const glm::vec3& _point_1, const glm::vec3& _point_2)
{
	glm::vec3 substr = _point_1 - _point_2;

	if(Math::floats_are_equal(substr.x, 0.0f) && Math::floats_are_equal(substr.y, 0.0f))
	{
		m_is_ok = false;
		return;
	}

	if(Math::floats_are_equal(substr.x, 0.0f))
	{
		m_vertical = true;
		m_x_if_vertical = _point_1.x;
		return;
	}

	m_k = substr.y / substr.x;
	if(Math::floats_are_equal(m_k, 0.0f))
		m_horisontal = true;

	m_b = _point_1.y - _point_1.x * m_k;

	m_goes_left = _point_2.x < _point_1.x;
}



Geometry::Simple_Intersection_Data Geometry_2D::lines_intersect(const Equasion_Data& _first, const Equasion_Data& _second)
{
	if(!_first.is_ok() || !_second.is_ok())
		return Geometry::Simple_Intersection_Data();

	if(_first.is_vertical() && _second.is_vertical())
	{
		if(Math::floats_are_equal(_first.get_x_if_vertical(), _second.get_x_if_vertical()))
			return Geometry::Simple_Intersection_Data(Geometry::Simple_Intersection_Data::Type::same_line);
		else
			return Geometry::Simple_Intersection_Data(Geometry::Simple_Intersection_Data::Type::none);
	}

	if(_first.is_vertical() ^ _second.is_vertical())
	{
		const Equasion_Data& vertical = _first.is_vertical() ? _first : _second;
		const Equasion_Data& not_vertical = _first.is_vertical() ? _second : _first;

		return Geometry::Simple_Intersection_Data(Geometry::Simple_Intersection_Data::Type::intersection,
			{vertical.get_x_if_vertical(), not_vertical.solve_by_x(vertical.get_x_if_vertical()), 0.0f});
	}

	if(Math::floats_are_equal(_first.get_k(), _second.get_k()))
	{
		if(Math::floats_are_equal(_first.get_b(), _second.get_b()))
			return Geometry::Simple_Intersection_Data(Geometry::Simple_Intersection_Data::Type::same_line);
		else
			return Geometry::Simple_Intersection_Data(Geometry::Simple_Intersection_Data::Type::none);
	}

	float x = (_second.get_b() - _first.get_b()) / (_first.get_k() - _second.get_k());
	float y = _first.solve_by_x(x);

	return Geometry::Simple_Intersection_Data(Geometry::Simple_Intersection_Data::Type::intersection, {x, y, 0.0f});
}

Geometry::Simple_Intersection_Data Geometry_2D::segments_intersect(const Geometry::Segment& _first, const Geometry::Segment& _second)
{
	Equasion_Data first_eq(_first.start, _first.end);
	Equasion_Data second_eq(_second.start, _second.end);

	Geometry::Simple_Intersection_Data id = lines_intersect(first_eq, second_eq);
	if(!id)
		return id;

	const glm::vec3& first_higher = _first.start.y > _first.end.y ? _first.start : _first.end;
	const glm::vec3& first_lower = _first.start.y > _first.end.y ? _first.end : _first.start;
	const glm::vec3& second_higher = _second.start.y > _second.end.y ? _second.start : _second.end;
	const glm::vec3& second_lower = _second.start.y > _second.end.y ? _second.end : _second.start;

	if(id.type == Geometry::Simple_Intersection_Data::Type::same_line)
	{
		if(first_eq.is_horisontal() && second_eq.is_horisontal())
		{
			if((_first.start.x <= _second.start.x && _first.start.x >= _second.end.x) || (_first.start.x >= _second.start.x && _first.start.x <= _second.end.x))
				id.point = _first.start;
			else if((_first.end.x <= _second.start.x && _first.end.x >= _second.end.x) || (_first.end.x >= _second.start.x && _first.end.x <= _second.end.x))
				id.point = _first.end;

			else
				return Geometry::Simple_Intersection_Data();
		}

		if((first_eq.is_vertical() && second_eq.is_vertical()) || Math::floats_are_equal(first_eq.get_k(), second_eq.get_k()))
		{
			if(first_higher.y > second_higher.y)
			{
				if(second_higher.y >= first_lower.y)
					id.point = first_lower;
				else
					return Geometry::Simple_Intersection_Data();
			}
			else if(first_higher.y < second_higher.y)
			{
				if(first_higher.y >= second_lower.y)
					id.point = second_lower;
				else
					return Geometry::Simple_Intersection_Data();
			}
			else
				return Geometry::Simple_Intersection_Data();
		}

		return Geometry::Simple_Intersection_Data();
	}
	else if(first_eq.is_horisontal() ^ second_eq.is_horisontal())
	{
		bool fh = first_eq.is_horisontal();	// fh - first horisontal
		if(fh)
		{
			if(!(((id.point.x <= _first.start.x && id.point.x >= _first.end.x) || (id.point.x >= _first.start.x && id.point.x <= _first.end.x))
					&& id.point.y <= second_higher.y && id.point.y >= second_lower.y))
				return Geometry::Simple_Intersection_Data();
		}
		else
		{
			if(!(((id.point.x <= _second.start.x && id.point.x >= _second.end.x) || (id.point.x >= _second.start.x && id.point.x <= _second.end.x))
					&& id.point.y <= first_higher.y && id.point.y >= first_lower.y))
				return Geometry::Simple_Intersection_Data();
		}
	}
	else if(!((id.point.y < first_higher.y || Math::floats_are_equal(id.point.y, first_higher.y))
			&& (id.point.y > first_lower.y || Math::floats_are_equal(id.point.y, first_lower.y))
			&& (id.point.y < second_higher.y || Math::floats_are_equal(id.point.y, second_higher.y))
			&& (id.point.y > second_lower.y || Math::floats_are_equal(id.point.y, second_lower.y))))
	{
		return Geometry::Simple_Intersection_Data();
	}

	id.first = _first;
	id.second = _second;

	return id;
}

void Geometry_2D::rotate_perpendicular_ccw(glm::vec3& _vec)
{
	float x = _vec.x;
	_vec.x = -_vec.y;
	_vec.y = x;
}

bool Geometry_2D::vec_points_left(const glm::vec3& _vec, const glm::vec3& _from)
{
    return (_vec.x * _from.y - _vec.y * _from.x) < 0.0f;
}









































