#include "../include/Physical_Model_2D.h"

using namespace LEti;

Physical_Model_2D::Polygon::Polygon()
{

}

void Physical_Model_2D::Polygon::setup(const float *_raw_coords)
{
	m_raw_coords = _raw_coords;

	ASSERT(!m_raw_coords);
}


void Physical_Model_2D::Polygon::update_points(const glm::mat4x4 &_translation, const glm::mat4x4 &_rotation, const glm::mat4x4 &_scale)
{
	ASSERT(!m_raw_coords);

	glm::mat4x4 result_matrix = _translation * _rotation * _scale;
	m_actual_A = result_matrix * glm::vec4(m_raw_coords[0], m_raw_coords[1], m_raw_coords[2], 1.0f);
	m_actual_B = result_matrix * glm::vec4(m_raw_coords[3], m_raw_coords[4], m_raw_coords[5], 1.0f);
	m_actual_C = result_matrix * glm::vec4(m_raw_coords[6], m_raw_coords[7], m_raw_coords[8], 1.0f);
}



Physical_Model_2D::Polygon::Equasion_Data Physical_Model_2D::Polygon::get_equasion(const glm::vec3 &_point_1, const glm::vec3 &_point_2) const
{
	Equasion_Data result;

	glm::vec3 substr = _point_1 - _point_2;

	if(fabs(substr.x) < 0.00001f)
	{
		result.k = 0.0f;
		result.b = 0.0f;
		return result;
	}

	result.k = substr.y / substr.x;
	result.b = _point_1.y - _point_1.x * result.k;

	result.goes_left = _point_2.x < _point_1.x;

	return result;
}



Physical_Model_Interface::Intersection_Data Physical_Model_2D::Polygon::point_belongs_to_triangle(const glm::vec3& _point) const
{
	Equasion_Data AB_eq = get_equasion(m_actual_A, m_actual_B);
	//    bool AB_goes_left = m_actual_B.x < m_actual_A.x;
	Equasion_Data BC_eq = get_equasion(m_actual_B, m_actual_C);
	//	bool BC_goes_left = m_actual_C.x < m_actual_B.x;
	Equasion_Data CA_eq = get_equasion(m_actual_C, m_actual_A);
	//	bool CA_goes_left = m_actual_A.x < m_actual_C.x;

	float AB_y_proj = _point.x * AB_eq.k + AB_eq.b;
	float BC_y_proj = _point.x * BC_eq.k + BC_eq.b;
	float CA_y_proj = _point.x * CA_eq.k + CA_eq.b;

	bool AB_right_side = AB_eq.is_vertical() ? ( m_actual_B.y < m_actual_A.y ? _point.x >= m_actual_A.x : _point.x <= m_actual_A.x ) : ( AB_eq.goes_left ? AB_y_proj > _point.y : AB_y_proj < _point.y );
	bool BC_right_side = BC_eq.is_vertical() ? ( m_actual_C.y < m_actual_B.y ? _point.x >= m_actual_B.x : _point.x <= m_actual_B.x ) : ( BC_eq.goes_left ? BC_y_proj > _point.y : BC_y_proj < _point.y );
	bool CA_right_side = CA_eq.is_vertical() ? ( m_actual_A.y < m_actual_C.y ? _point.x >= m_actual_C.x : _point.x <= m_actual_C.x ) : ( CA_eq.goes_left ? CA_y_proj > _point.y : CA_y_proj < _point.y );

	//	return (AB_goes_left ? AB_y_proj < _point.y : AB_y_proj > _point.y) &&
	//		(BC_goes_left ? BC_y_proj < _point.y : BC_y_proj > _point.y) &&
	//		(CA_goes_left ? CA_y_proj < _point.y : CA_y_proj > _point.y);
	if (AB_right_side && BC_right_side && CA_right_side) return Intersection_Data(Intersection_Data::Intersection_Type::inside);
	return Intersection_Data(Intersection_Data::Intersection_Type::none);
}

#include <vector>


Physical_Model_Interface::Intersection_Data Physical_Model_2D::Polygon::segments_intersect(const glm::vec3& _point_11, const glm::vec3& _point_21, const glm::vec3& _point_12, const glm::vec3& _point_22) const
{
	std::pair<std::pair<float, float>, std::pair<float, float>> f({_point_11.x, _point_11.y}, {_point_21.x, _point_21.y});
	std::pair<std::pair<float, float>, std::pair<float, float>> s({_point_12.x, _point_12.y}, {_point_22.x, _point_22.y});
	glm::vec3 first_from_zero = _point_21 - _point_11;
	glm::vec3 second_from_zero = _point_22 - _point_12;

	//    if(!LEti::Utility::beams_cross_at_right_angle(first_from_zero, second_from_zero))
	//    if(fabs(LEti::Utility::angle_cos_between_vectors(first_from_zero, second_from_zero)) < 0.5f)
	//        return Intersection_Data(Intersection_Data::Intersection_Type::none);

	Equasion_Data first_eq = get_equasion(_point_11, _point_21);
	Equasion_Data second_eq = get_equasion(_point_12, _point_22);

	glm::vec3 intersection_point;

	if(_point_11.x == _point_21.x)
		intersection_point.x = _point_11.x;
	else if(_point_12.x == _point_22.x)
		intersection_point.x = _point_12.x;
	else
		intersection_point.x = (first_eq.k - second_eq.k == 0) ? 0.0f : (second_eq.b - first_eq.b) / (first_eq.k - second_eq.k);
	intersection_point.y = first_eq.k * intersection_point.x + first_eq.b;

	float first_length = LEti::Utility::get_distance(_point_11, _point_21);
	float second_length = LEti::Utility::get_distance(_point_12, _point_22);

	//TODO: think about optimization: calculating vectors' lengths may be unnecessary
	if ((LEti::Utility::get_distance(intersection_point, _point_11) < first_length) &&
			(LEti::Utility::get_distance(intersection_point, _point_21) < first_length) &&
			(LEti::Utility::get_distance(intersection_point, _point_12) < second_length) &&
			(LEti::Utility::get_distance(intersection_point, _point_22) < second_length))
	{
		return Intersection_Data(Intersection_Data::Intersection_Type::partly_outside, intersection_point);
	}

	return Intersection_Data(Intersection_Data::Intersection_Type::none);
}


Physical_Model_Interface::Intersection_Data Physical_Model_2D::Polygon::segment_intersecting_polygon(const glm::vec3 &_point_1, const glm::vec3 &_point_2) const
{
	Intersection_Data _0 = segments_intersect(m_actual_A, m_actual_B, _point_1, _point_2);
	if(_0)
		return _0;
	Intersection_Data _1 = segments_intersect(m_actual_B, m_actual_C, _point_1, _point_2);
	if(_1)
		return _1;
	Intersection_Data _2 = segments_intersect(m_actual_C, m_actual_A, _point_1, _point_2);
	if(_2)
		return _2;
	return Intersection_Data(Intersection_Data::Intersection_Type::none);
}

Physical_Model_Interface::Intersection_Data Physical_Model_2D::Polygon::intersects_with_another_polygon(const Polygon& _other) const
{
	Intersection_Data _0 = segment_intersecting_polygon(_other.m_actual_A, _other.m_actual_B);
	if(_0)
		return _0;
	Intersection_Data _1 = segment_intersecting_polygon(_other.m_actual_B, _other.m_actual_C);
	if(_1)
		return _1;
	Intersection_Data _2 = segment_intersecting_polygon(_other.m_actual_C, _other.m_actual_A);
	if(_2)
		return _2;
	Intersection_Data _3 = point_belongs_to_triangle(_other.m_actual_A);
	Intersection_Data _4 = point_belongs_to_triangle(_other.m_actual_B);
	Intersection_Data _5 = point_belongs_to_triangle(_other.m_actual_C);
//	if(_3) return _3;
//	if(_4) return _4;
//	if(_5) return _5;
	if(_3 && _4 && _5)
		return _3;
	return Intersection_Data(Intersection_Data::Intersection_Type::none);
}



const glm::vec3& Physical_Model_2D::Polygon::operator[](unsigned int _index) const
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



Physical_Model_2D::Physical_Model_2D() : Physical_Model_Interface()
{

}

Physical_Model_2D::Physical_Model_2D(const float* _raw_coords, unsigned int _raw_coords_count)
{
	setup(_raw_coords, _raw_coords_count);
}

void Physical_Model_2D::setup(const float* _raw_coords, unsigned int _raw_coords_count)
{
	Physical_Model_Interface::setup(_raw_coords, _raw_coords_count);

	delete[] m_polygons;

	m_polygons_count = m_raw_coords_count / 9;
	m_polygons = new Polygon[m_polygons_count];
	for (unsigned int i = 0; i < m_polygons_count; ++i)
		m_polygons[i].setup(&m_raw_coords[i * 9]);
}


Physical_Model_2D::~Physical_Model_2D()
{
	delete[] m_polygons;
}


void Physical_Model_2D::update(const glm::mat4x4& _translation, const glm::mat4x4& _rotation, const glm::mat4x4& _scale)
{
	ASSERT(!m_polygons);

	for (unsigned int i = 0; i < m_polygons_count; ++i)
		m_polygons[i].update_points(_translation, _rotation, _scale);
}



Physical_Model_Interface::Intersection_Data Physical_Model_2D::is_intersecting_with_point(const glm::vec3& _point) const
{
	ASSERT(!m_polygons);

	for (unsigned int i = 0; i < m_polygons_count; ++i)
		if (m_polygons[i].point_belongs_to_triangle(_point)) return Intersection_Data(Intersection_Data::Intersection_Type::inside);
	return Intersection_Data(Intersection_Data::Intersection_Type::none);
}

Physical_Model_Interface::Intersection_Data Physical_Model_2D::is_intersecting_with_segment(const glm::vec3& _point_1, const glm::vec3& _point_2) const
{
	ASSERT(!m_polygons);

	for (unsigned int i = 0; i < m_polygons_count; ++i)
	{
		Intersection_Data id = m_polygons[i].segment_intersecting_polygon(_point_1, _point_2);
		if (id) return id;
	}
	return Intersection_Data(Intersection_Data::Intersection_Type::none);
}

Physical_Model_Interface::Intersection_Data Physical_Model_2D::is_intersecting_with_another_model(const Physical_Model_Interface& _other) const
{
	const Physical_Model_2D& other = (Physical_Model_2D&)_other;

	ASSERT(!m_polygons || !other.m_polygons);

	for (unsigned int i = 0; i < m_polygons_count; ++i)
	{
		for (unsigned int j = 0; j < other.m_polygons_count; ++j)
		{
			Intersection_Data id = m_polygons[i].intersects_with_another_polygon(other.m_polygons[j]);
			if (id) return id;
		}
	}
	for (unsigned int i = 0; i < other.m_polygons_count; ++i)
	{
		for (unsigned int j = 0; j < m_polygons_count; ++j)
		{
			Intersection_Data id = other.m_polygons[i].intersects_with_another_polygon(m_polygons[j]);
			if (id) return id;
		}
	}

	return Intersection_Data(Intersection_Data::Intersection_Type::none);
}



Physical_Model_2D::Rectangular_Border Physical_Model_2D::construct_rectangular_border() const
{
	ASSERT(!m_polygons);
	Rectangular_Border result;

	result.left = m_polygons[0][0].x;
	result.right = m_polygons[0][0].x;
	result.top = m_polygons[0][0].y;
	result.bottom = m_polygons[0][0].y;

	for(unsigned int i=0; i<m_polygons_count; ++i)
	{
		for(unsigned int p=0; p<3; ++p)
		{
			if(result.left > m_polygons[i][p].x) result.left = m_polygons[i][p].x;
			if(result.right < m_polygons[i][p].x) result.right = m_polygons[i][p].x;
			if(result.top < m_polygons[i][p].y) result.top = m_polygons[i][p].y;
			if(result.bottom > m_polygons[i][p].y) result.bottom = m_polygons[i][p].y;
		}
	}

	return result;
}


unsigned int Physical_Model_2D::get_polygons_count() const
{
	return m_polygons_count;
}

const Physical_Model_2D::Polygon& Physical_Model_2D::operator[](unsigned int _index) const
{
	ASSERT(!m_polygons || _index >= m_polygons_count);

	return m_polygons[_index];
}
