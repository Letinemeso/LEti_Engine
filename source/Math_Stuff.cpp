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
	return fabs(_first-_second) < 0.0001f;
}



//	LEti::Geometry_2D (math functions and classes, suitable for 2d calculations)

Geometry_2D::Polygon::Polygon()
{

}

Geometry_2D::Polygon::Polygon(const Polygon& _other)
{
	m_raw_coords = _other.m_raw_coords;
	m_actual_A = _other.m_actual_A;
	m_actual_B = _other.m_actual_B;
	m_actual_C = _other.m_actual_C;
	m_center_of_mass_raw = _other.m_center_of_mass_raw;
	m_center_of_mass = _other.m_center_of_mass;
}

void Geometry_2D::Polygon::setup(const float *_raw_coords)
{
	m_raw_coords = _raw_coords;

	glm::vec3 sum{0.0f, 0.0f, 0.0f};
	for(unsigned int i=0; i<3; ++i)
		sum += glm::vec3(_raw_coords[i*3 + 0], _raw_coords[i * 3 + 1], _raw_coords[i * 3 + 2]);
	m_center_of_mass_raw = sum / 3.0f;

	ASSERT(!m_raw_coords);
}

void Geometry_2D::Polygon::setup(const Polygon& _other)
{
	m_raw_coords = _other.m_raw_coords;
	m_actual_A = _other.m_actual_A;
	m_actual_B = _other.m_actual_B;
	m_actual_C = _other.m_actual_C;
	m_center_of_mass_raw = _other.m_center_of_mass_raw;
	m_center_of_mass = _other.m_center_of_mass;
}

void Geometry_2D::Polygon::update_points(const glm::mat4x4 &_translation, const glm::mat4x4 &_rotation, const glm::mat4x4 &_scale)
{
	ASSERT(!m_raw_coords);

	glm::mat4x4 result_matrix = _translation * _rotation * _scale;
	update_points_with_single_matrix(result_matrix);
}

void Geometry_2D::Polygon::update_points_with_single_matrix(const glm::mat4x4 &_matrix)
{
	ASSERT(!m_raw_coords);

	m_actual_A = _matrix * glm::vec4(m_raw_coords[0], m_raw_coords[1], m_raw_coords[2], 1.0f);
	m_actual_B = _matrix * glm::vec4(m_raw_coords[3], m_raw_coords[4], m_raw_coords[5], 1.0f);
	m_actual_C = _matrix * glm::vec4(m_raw_coords[6], m_raw_coords[7], m_raw_coords[8], 1.0f);
	m_center_of_mass = _matrix * glm::vec4(m_center_of_mass_raw, 1.0f);
}



Geometry::Intersection_Data Geometry_2D::Polygon::point_belongs_to_triangle(const glm::vec3& _point) const
{
	Geometry_2D::Equasion_Data AB_eq(m_actual_A, m_actual_B);
	Geometry_2D::Equasion_Data BC_eq(m_actual_B, m_actual_C);
	Geometry_2D::Equasion_Data CA_eq(m_actual_C, m_actual_A);

	float AB_y_proj = AB_eq.solve_by_x(_point.x);
	float BC_y_proj = BC_eq.solve_by_x(_point.x);
	float CA_y_proj = CA_eq.solve_by_x(_point.x);

	bool AB_right_side = AB_eq.is_vertical() ? ( m_actual_B.y < m_actual_A.y ? _point.x >= m_actual_A.x : _point.x <= m_actual_A.x ) : ( AB_eq.goes_left() ? AB_y_proj > _point.y : AB_y_proj < _point.y );
	bool BC_right_side = BC_eq.is_vertical() ? ( m_actual_C.y < m_actual_B.y ? _point.x >= m_actual_B.x : _point.x <= m_actual_B.x ) : ( BC_eq.goes_left() ? BC_y_proj > _point.y : BC_y_proj < _point.y );
	bool CA_right_side = CA_eq.is_vertical() ? ( m_actual_A.y < m_actual_C.y ? _point.x >= m_actual_C.x : _point.x <= m_actual_C.x ) : ( CA_eq.goes_left() ? CA_y_proj > _point.y : CA_y_proj < _point.y );

	//	return (AB_goes_left() ? AB_y_proj < _point.y : AB_y_proj > _point.y) &&
	//		(BC_goes_left() ? BC_y_proj < _point.y : BC_y_proj > _point.y) &&
	//		(CA_goes_left() ? CA_y_proj < _point.y : CA_y_proj > _point.y);
	if (AB_right_side && BC_right_side && CA_right_side)
		return Geometry::Intersection_Data(Geometry::Intersection_Data::Type::intersection, _point);
	return Geometry::Intersection_Data(Geometry::Intersection_Data::Type::none);
}


Geometry::Intersection_Data Geometry_2D::Polygon::segment_intersecting_polygon(const glm::vec3 &_point_1, const glm::vec3 &_point_2) const
{
	Rectangular_Border segment_rb;
	segment_rb.consider_point(_point_1).consider_point(_point_2);
	Rectangular_Border polygon_rb;
	polygon_rb.consider_point(m_actual_A).consider_point(m_actual_B).consider_point(m_actual_C);

	if(!(segment_rb && polygon_rb))
		return Geometry::Intersection_Data(Geometry::Intersection_Data::Type::none);

	Geometry::Intersection_Data _0 = Geometry_2D::segments_intersect(m_actual_A, m_actual_B, _point_1, _point_2);
	if(_0)
		return _0;
	Geometry::Intersection_Data _1 = Geometry_2D::segments_intersect(m_actual_B, m_actual_C, _point_1, _point_2);
	if(_1)
		return _1;
	Geometry::Intersection_Data _2 = Geometry_2D::segments_intersect(m_actual_C, m_actual_A, _point_1, _point_2);
	if(_2)
		return _2;
	return Geometry::Intersection_Data(Geometry::Intersection_Data::Type::none);
}

Geometry::Intersection_Data Geometry_2D::Polygon::intersects_with_another_polygon(const Polygon& _other) const
{
	Rectangular_Border this_rb;
	this_rb.consider_point(m_actual_A).consider_point(m_actual_B).consider_point(m_actual_C);
	Rectangular_Border other_rb;
	other_rb.consider_point(_other.m_actual_A).consider_point(_other.m_actual_B).consider_point(_other.m_actual_C);

	if(!(this_rb && other_rb))
		return Geometry::Intersection_Data(Geometry::Intersection_Data::Type::none);

	Geometry::Intersection_Data _0 = segment_intersecting_polygon(_other.m_actual_A, _other.m_actual_B);
	if(_0)
		return _0;
	Geometry::Intersection_Data _1 = segment_intersecting_polygon(_other.m_actual_B, _other.m_actual_C);
	if(_1)
		return _1;
	Geometry::Intersection_Data _2 = segment_intersecting_polygon(_other.m_actual_C, _other.m_actual_A);
	if(_2)
		return _2;
	Geometry::Intersection_Data _3 = point_belongs_to_triangle(_other.m_actual_A);
	Geometry::Intersection_Data _4 = point_belongs_to_triangle(_other.m_actual_B);
	Geometry::Intersection_Data _5 = point_belongs_to_triangle(_other.m_actual_C);
	if(_3 && _4 && _5)
		return _3;
	return Geometry::Intersection_Data(Geometry::Intersection_Data::Type::none);
}



const glm::vec3& Geometry_2D::Polygon::operator[](unsigned int _index) const
{
	ASSERT(_index > 2);
	switch(_index)
	{
	case 0 : return m_actual_A;
	case 1: return m_actual_B;
	case 2: return m_actual_C;
	}
	return m_actual_C;
}

glm::vec3& Geometry_2D::Polygon::operator[](unsigned int _index)
{
	ASSERT(_index > 2);
	switch(_index)
	{
	case 0 : return m_actual_A;
	case 1: return m_actual_B;
	case 2: return m_actual_C;
	}
	return m_actual_C;
}

const glm::vec3& Geometry_2D::Polygon::center_of_mass() const
{
	return m_center_of_mass;
}

const glm::vec3& Geometry_2D::Polygon::center_of_mass_raw() const
{
	return m_center_of_mass_raw;
}




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

	if(_first.is_vertical() ^ _second.is_vertical())
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

Geometry::Intersection_Data Geometry_2D::segments_intersect(const glm::vec3& _point_11, const glm::vec3& _point_21, const glm::vec3& _point_12, const glm::vec3& _point_22)
{
	Equasion_Data first_eq(_point_11, _point_21);
	Equasion_Data second_eq(_point_12, _point_22);

	Geometry::Intersection_Data id = lines_intersect(first_eq, second_eq);
	if(!id)
		return id;

	const glm::vec3& first_higher = _point_11.y > _point_21.y ? _point_11 : _point_21;
	const glm::vec3& first_lower = _point_11.y > _point_21.y ? _point_21 : _point_11;
	const glm::vec3& second_higher = _point_12.y > _point_22.y ? _point_12 : _point_22;
	const glm::vec3& second_lower = _point_12.y > _point_22.y ? _point_22 : _point_12;

	//	this is probably better than calculating segments' lengths several times per segments' pair
	if(id.type == Geometry::Intersection_Data::Type::same_line)
	{
		if(first_eq.is_horisontal() && second_eq.is_horisontal())
		{
			if((_point_11.x <= _point_12.x && _point_11.x >= _point_22.x) || (_point_11.x >= _point_12.x && _point_11.x <= _point_22.x))
			{
				id.point = _point_11;
				return id;
			}
			else if((_point_21.x <= _point_12.x && _point_21.x >= _point_22.x) || (_point_21.x >= _point_12.x && _point_21.x <= _point_22.x))
			{
				id.point = _point_21;
				return id;
			}
			return Geometry::Intersection_Data();
		}

		if((first_eq.is_vertical() && second_eq.is_vertical()) || Math::floats_are_equal(first_eq.get_k(), second_eq.get_k()))
		{
			if(first_higher.y > second_higher.y)
			{
				if(second_higher.y >= first_lower.y)
				{
					id.point = first_lower;
					return id;
				}
				else
					return Geometry::Intersection_Data();
			}
			else if(first_higher.y < second_higher.y)
			{
				if(first_higher.y >= second_lower.y)
				{
					id.point = second_lower;
					return id;
				}
				else
					return Geometry::Intersection_Data();
			}
			return Geometry::Intersection_Data();
		}

		return Geometry::Intersection_Data();
	}

	if(first_eq.is_horisontal() ^ second_eq.is_horisontal())
	{
		bool fh = first_eq.is_horisontal();	// fh - first horisontal
		if(fh)
		{
			if(((id.point.x <= _point_11.x && id.point.x >= _point_21.x) || (id.point.x >= _point_11.x && id.point.x <= _point_21.x))
					&& id.point.y <= second_higher.y && id.point.y >= second_lower.y)
				return id;
		}
		else
		{
			if(((id.point.x <= _point_12.x && id.point.x >= _point_22.x) || (id.point.x >= _point_12.x && id.point.x <= _point_22.x))
					&& id.point.y <= first_higher.y && id.point.y >= first_lower.y)
				return id;
		}
		return Geometry::Intersection_Data();
	}

	if(id.point.y <= first_higher.y && id.point.y >= first_lower.y
			&& id.point.y <= second_higher.y && id.point.y >= second_lower.y)
		return id;

	return Geometry::Intersection_Data();
}









































