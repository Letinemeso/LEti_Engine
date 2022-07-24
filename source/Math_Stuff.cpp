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

float Math::get_distance(const glm::vec3& _first, const glm::vec3& _second)
{
	if (_first == _second)
		return 0.0f;
	return sqrt(pow(_second.x - _first.x, 2) + pow(_second.y - _first.y, 2) + pow(_second.z - _first.z, 2));
}

glm::vec3 Math::normalize(const glm::vec3& _first, const glm::vec3& _second)
{
	return {_first.y * _second.z - _second.y * _first.z, -(_first.x * _second.z - _second.x * _first.z), _first.x * _second.y - _second.x * _first.y};
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
	ASSERT(vector_length(_first) == 0 ||  vector_length(_second) == 0);

	//  (_first * _second) / (|_first| * |_second|)
	float divident = _first.x * _second.x + _first.y * _second.y + _first.z * _second.z;
	float divider = vector_length(_first) * vector_length(_second);
	return divident / divider;
}

bool Math::beams_cross_at_right_angle(const glm::vec3& _first, const glm::vec3& _second)
{
	ASSERT(vector_length(_first) == 0 ||  vector_length(_second) == 0);

	//  (_first * _second) / (|_first| * |_second|)
	float divident = _first.x * _second.x + _first.y * _second.y + _first.z * _second.z;

	return divident < 0.0f; // angles cos must be negative
}

bool Math::floats_are_equal(float _first, float _second)
{
	return fabs(fabs(_first)-fabs(_second)) < 0.0001f;
}



//	LEti::Geometry_2D (math functions and classes, suitable for 2d calculations)

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



Geometry::Intersection_Data Geometry_2D::lines_intersect(const Equasion_Data& _first, const Equasion_Data& _second)
{
	if(!_first.is_ok() || !_second.is_ok())
		return Geometry::Intersection_Data();

	if(_first.is_vertical() && _second.is_vertical())
	{
		if(Math::floats_are_equal(_first.get_x_if_vertical(), _second.get_x_if_vertical()))
			return Geometry::Intersection_Data(Geometry::Intersection_Data::Type::same_line);
		else
			return Geometry::Intersection_Data(Geometry::Intersection_Data::Type::none);
	}

	if((_first.is_vertical() && !_second.is_vertical()) || (!_first.is_vertical() && _second.is_vertical()))
	{
		const Equasion_Data& vertical = _first.is_vertical() ? _first : _second;
		const Equasion_Data& not_vertical = _first.is_vertical() ? _second : _first;

		return Geometry::Intersection_Data(Geometry::Intersection_Data::Type::intersection,
			{vertical.get_x_if_vertical(), not_vertical.solve_by_x(vertical.get_x_if_vertical()), 0.0f});
	}

	if(Math::floats_are_equal(_first.get_k(), _second.get_k()))
	{
		if(Math::floats_are_equal(_first.get_b(), _second.get_b()))
			return Geometry::Intersection_Data(Geometry::Intersection_Data::Type::same_line);
		else
			return Geometry::Intersection_Data(Geometry::Intersection_Data::Type::none);
	}

	float x = (_second.get_b() - _first.get_b()) / (_first.get_k() - _second.get_k());
	float y = _first.solve_by_x(x);

	return Geometry::Intersection_Data(Geometry::Intersection_Data::Type::intersection, {x, y, 0.0f});
}
#include <iostream>
Geometry::Intersection_Data Geometry_2D::segments_intersect(const glm::vec3& _point_11, const glm::vec3& _point_21, const glm::vec3& _point_12, const glm::vec3& _point_22)
{
	Equasion_Data first_eq(_point_11, _point_21);
	Equasion_Data second_eq(_point_12, _point_22);

	Geometry::Intersection_Data id = lines_intersect(first_eq, second_eq);
	if(!id)
		return id;

	float first_length = LEti::Math::get_distance(_point_11, _point_21);
	float second_length = LEti::Math::get_distance(_point_12, _point_22);

	//TODO: think about optimization: calculating vectors' lengths may be unnecessary
	if ((LEti::Math::get_distance(id.point, _point_11) < first_length) &&
			(LEti::Math::get_distance(id.point, _point_21) < first_length) &&
			(LEti::Math::get_distance(id.point, _point_12) < second_length) &&
			(LEti::Math::get_distance(id.point, _point_22) < second_length))
	{
		std::cout << "11 x: " << _point_11.x << "\ty: " << _point_11.y << '\n';
		std::cout << "21 x: " << _point_21.x << "\ty: " << _point_21.y << '\n';
		std::cout << "12 x: " << _point_12.x << "\ty: " << _point_12.y << '\n';
		std::cout << "22 x: " << _point_22.x << "\ty: " << _point_22.y << '\n';
		std::cout << "\n";
		std::cout << LEti::Math::get_distance(id.point, _point_11) << "\t" << first_length << '\n'
					<< LEti::Math::get_distance(id.point, _point_21) << "\t" << first_length << '\n'
					<< LEti::Math::get_distance(id.point, _point_12) << "\t" << second_length << '\n'
					<< LEti::Math::get_distance(id.point, _point_22) << "\t" << second_length << "\n\n";
		std::cout << "intersection point:\n\tx: " << id.point.x << "\ty: " << id.point.y << "\n\n";

		return id;
	}

	return Geometry::Intersection_Data(Geometry::Intersection_Data::Type::none);
}









































