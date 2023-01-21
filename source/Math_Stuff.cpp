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

float Math::dot_product(const glm::vec3& _first, const glm::vec3& _second)
{
	float length1 = vector_length(_first);
	float length2 = vector_length(_second);
	if(length1 < 0.0001f || length2 < 0.0001f)
		return 0.0f;
	return length1 * length2 * angle_cos_between_vectors(_first, _second);
}

float Math::cross_product(const glm::vec3& _first, const glm::vec3& _second)
{
	return normalize(_first, _second).z;
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
	L_ASSERT(!(vector_length(_first) == 0 ||  vector_length(_second) == 0));
	if(vector_length(_first) == 0 ||  vector_length(_second) == 0)
		return 0.0f;

	float divident = _first.x * _second.x + _first.y * _second.y + _first.z * _second.z;
	float divider = vector_length(_first) * vector_length(_second);
	return divident / divider;
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

bool Math::vecs_are_equal(const glm::vec3& _first, const glm::vec3& _second)
{
	return floats_are_equal(_first.x, _second.x) && floats_are_equal(_first.y, _second.y) && floats_are_equal(_first.z, _second.z);
}


//	LEti::Geometry (generic geometry functions and easy-to-use structures)

std::pair<glm::vec3, glm::vec3> Geometry::get_segments_normals(const Segment& _first, const Segment& _second)
{
	glm::vec3 f_dir = _first.direction_vector(), s_dir = _second.direction_vector();
//	glm::vec3 axis = Math::normalize(f_dir, s_dir);
	glm::vec3 axis{0.0f, 0.0f, 1.0f};
	Math::shrink_vector_to_1(axis);
	glm::mat4x4 matrix = glm::rotate(-Math::HALF_PI, axis);

	glm::vec3 f = matrix * glm::vec4(f_dir, 1.0f);
	glm::vec3 s = matrix * glm::vec4(s_dir, 1.0f);
	Math::shrink_vector_to_1(f);
	Math::shrink_vector_to_1(s);

	return { f, s };
}





Geometry::Polygon::Polygon()
{

}

Geometry::Polygon::Polygon(const Polygon& _other)
{
	m_raw_coords = _other.m_raw_coords;
	m_actual_A = _other.m_actual_A;
	m_actual_B = _other.m_actual_B;
	m_actual_C = _other.m_actual_C;
	m_center_of_mass_raw = _other.m_center_of_mass_raw;
	m_center_of_mass = _other.m_center_of_mass;
}

void Geometry::Polygon::setup(const float *_raw_coords)
{
	m_raw_coords = _raw_coords;

	glm::vec3 sum{0.0f, 0.0f, 0.0f};
	for(unsigned int i=0; i<3; ++i)
		sum += glm::vec3(_raw_coords[i*3 + 0], _raw_coords[i * 3 + 1], _raw_coords[i * 3 + 2]);
	m_center_of_mass_raw = sum / 3.0f;

	L_ASSERT(!(!m_raw_coords));
}

void Geometry::Polygon::setup(const Polygon& _other)
{
	m_raw_coords = _other.m_raw_coords;
	m_actual_A = _other.m_actual_A;
	m_actual_B = _other.m_actual_B;
	m_actual_C = _other.m_actual_C;
	m_center_of_mass_raw = _other.m_center_of_mass_raw;
	m_center_of_mass = _other.m_center_of_mass;
}

void Geometry::Polygon::update_points(const glm::mat4x4 &_translation, const glm::mat4x4 &_rotation, const glm::mat4x4 &_scale)
{
	L_ASSERT(!(!m_raw_coords));

	glm::mat4x4 result_matrix = _translation * _rotation * _scale;
	update_points_with_single_matrix(result_matrix);
}

void Geometry::Polygon::update_points_with_single_matrix(const glm::mat4x4 &_matrix)
{
	L_ASSERT(!(!m_raw_coords));

	m_actual_A = _matrix * glm::vec4(m_raw_coords[0], m_raw_coords[1], m_raw_coords[2], 1.0f);
	m_actual_B = _matrix * glm::vec4(m_raw_coords[3], m_raw_coords[4], m_raw_coords[5], 1.0f);
	m_actual_C = _matrix * glm::vec4(m_raw_coords[6], m_raw_coords[7], m_raw_coords[8], 1.0f);
	m_center_of_mass = _matrix * glm::vec4(m_center_of_mass_raw, 1.0f);
}


const glm::vec3& Geometry::Polygon::operator[](unsigned int _index) const
{
//	L_ASSERT(!(_index > 2));
	switch(_index)
	{
	case 0 : return m_actual_A;
	case 1: return m_actual_B;
	case 2: return m_actual_C;
	}
	return m_actual_A;	//will be useful for "for" loops ([i], [i + 1])
}

glm::vec3& Geometry::Polygon::operator[](unsigned int _index)
{
//	L_ASSERT(!(_index > 2));
	switch(_index)
	{
	case 0 : return m_actual_A;
	case 1: return m_actual_B;
	case 2: return m_actual_C;
	}
	return m_actual_A;	//will be useful for "for" loops ([i], [i + 1])
}

const glm::vec3& Geometry::Polygon::center_of_mass() const
{
	return m_center_of_mass;
}

const glm::vec3& Geometry::Polygon::center_of_mass_raw() const
{
	return m_center_of_mass_raw;
}





//	LEti::Geometry_2D (math functions and classes, suitable for 2d calculations)

Geometry_2D::Rectangular_Border::Rectangular_Border()
{

}

void Geometry_2D::Rectangular_Border::operator=(const Rectangular_Border& _other)
{
	left = _other.left;
	right = _other.right;
	top = _other.top;
	bottom = _other.bottom;
}


Geometry_2D::Rectangular_Border& Geometry_2D::Rectangular_Border::consider_point(const glm::vec3 &_point)
{
	if((*this) == Rectangular_Border())
	{
		left = _point.x;
		right = _point.x;
		top = _point.y;
		bottom = _point.y;
	}
	else
	{
		if(left > _point.x) left = _point.x;
		if(right < _point.x) right = _point.x;
		if(top < _point.y) top = _point.y;
		if(bottom > _point.y) bottom = _point.y;
	}

	return *this;
}


glm::vec3 Geometry_2D::Rectangular_Border::right_top() const
{
	return {right, top, 0.0f};
}

glm::vec3 Geometry_2D::Rectangular_Border::left_top() const
{
	return {left, top, 0.0f};
}

glm::vec3 Geometry_2D::Rectangular_Border::left_bottom() const
{
	return {left, bottom, 0.0f};
}

glm::vec3 Geometry_2D::Rectangular_Border::right_bottom() const
{
	return {right, bottom, 0.0f};
}


Geometry_2D::Rectangular_Border Geometry_2D::Rectangular_Border::operator&&(const Rectangular_Border &_other) const
{
	Rectangular_Border shared_space;

	bool first_on_left = left < _other.left;
	const Rectangular_Border& on_left = first_on_left ? *this : _other;
	const Rectangular_Border& on_right = first_on_left ? _other : *this;
	bool first_on_bottom = bottom < _other.bottom;
	const Rectangular_Border& on_bottom = first_on_bottom ? *this : _other;
	const Rectangular_Border& on_top = first_on_bottom ? _other : *this;

	shared_space.left = on_right.left;
	shared_space.right = on_left.right;
	shared_space.top = on_bottom.top;
	shared_space.bottom = on_top.bottom;

	if(shared_space.left > shared_space.right || shared_space.top < shared_space.bottom)
		return Rectangular_Border();

	return shared_space;
}

bool Geometry_2D::Rectangular_Border::operator==(const Rectangular_Border &_other) const
{
	return	Math::floats_are_equal(left, _other.left) &&
			Math::floats_are_equal(right, _other.right) &&
			Math::floats_are_equal(top, _other.top) &&
			Math::floats_are_equal(bottom, _other.bottom);
}

Geometry_2D::Rectangular_Border::operator bool() const
{
	bool result =	Math::floats_are_equal(left, 0.0f) &&
					Math::floats_are_equal(right, 0.0f) &&
					Math::floats_are_equal(top, 0.0f) &&
					Math::floats_are_equal(bottom, 0.0f);
	if(!result)
	{
		result  =	Math::floats_are_equal(left, right) &&
					Math::floats_are_equal(bottom, top);
	}

	return !result;
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

float Geometry_2D::point_to_axis_projection(const glm::vec3& _point, const glm::vec3& _axis)
{
	return Math::dot_product(_point, _axis);
}

float Geometry_2D::point_to_segment_distance(const glm::vec3& _point, const glm::vec3& _seg_start, const glm::vec3& _seg_end)
{
	glm::vec3 seg_perp = _seg_end - _seg_start;
	float x = seg_perp.x;
	seg_perp.x = -seg_perp.y;
	seg_perp.y = x;
	seg_perp += _point;

	Equasion_Data eq_1(_seg_start, _seg_end);
	Equasion_Data eq_2(_point, seg_perp);

	glm::vec3 itsc_point = Geometry_2D::lines_intersect(eq_1, eq_2).point;

	if(eq_1.is_horisontal())
	{
		float max_x = 0.0f;
		float min_x = 0.0f;

		if(_seg_start.x > _seg_end.x)
		{
			max_x = _seg_start.x;
			min_x = _seg_end.x;
		}
		else
		{
			max_x = _seg_end.x;
			min_x = _seg_start.x;
		}

		if(itsc_point.x < min_x || itsc_point.x > max_x)
			return -1.0f;
		return Math::vector_length(itsc_point - _point);
	}

	float y = eq_1.solve_by_x(itsc_point.x);
	if(eq_1.is_vertical())
	{
		if(!Math::floats_are_equal(eq_1.get_x_if_vertical(), itsc_point.x))
			return -1.0f;
		y = itsc_point.y;
	}

	float max_y = 0.0f;
	float min_y = 0.0f;

	if(_seg_start.y > _seg_end.y)
	{
		max_y = _seg_start.y;
		min_y = _seg_end.y;
	}
	else
	{
		max_y = _seg_end.y;
		min_y = _seg_start.y;
	}

	if(y < min_y || y > max_y)
		return -1.0f;

	return Math::vector_length(itsc_point - _point);
}

void Geometry_2D::rotate_perpendicular_ccw(glm::vec3& _vec)
{
	float x = _vec.x;
	_vec.x = -_vec.y;
	_vec.y = x;
}









































