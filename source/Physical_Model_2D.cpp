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



bool Physical_Model_2D::Polygon::point_belongs_to_triangle(const glm::vec3& _point) const
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
    return AB_right_side && BC_right_side && CA_right_side;
}




bool Physical_Model_2D::Polygon::segments_intersect(const glm::vec3& _point_11, const glm::vec3& _point_21, const glm::vec3& _point_12, const glm::vec3& _point_22) const
{
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

	return (LEti::Utility::get_distance(intersection_point, _point_11) < first_length) &&
		(LEti::Utility::get_distance(intersection_point, _point_21) < first_length) &&
		(LEti::Utility::get_distance(intersection_point, _point_12) < second_length) &&
		(LEti::Utility::get_distance(intersection_point, _point_22) < second_length);
}


bool Physical_Model_2D::Polygon::segment_intersecting_polygon(const glm::vec3 &_point_1, const glm::vec3 &_point_2) const
{
    return segments_intersect(m_actual_A, m_actual_B, _point_1, _point_2) ||
        segments_intersect(m_actual_B, m_actual_C, _point_1, _point_2) ||
        segments_intersect(m_actual_C, m_actual_A, _point_1, _point_2);
}

bool Physical_Model_2D::Polygon::intersects_with_another_polygon(const Polygon& _other) const
{
	return segment_intersecting_polygon(_other.m_actual_A, _other.m_actual_B) ||
		segment_intersecting_polygon(_other.m_actual_B, _other.m_actual_C) ||
        segment_intersecting_polygon(_other.m_actual_C, _other.m_actual_A) ||
        point_belongs_to_triangle(_other.m_actual_A) ||
        point_belongs_to_triangle(_other.m_actual_B) ||
        point_belongs_to_triangle(_other.m_actual_C);
}



Physical_Model_2D::Physical_Model_2D() : Physical_Model_Interface()
{

}

Physical_Model_2D::Physical_Model_2D(const float* _raw_coords, unsigned int _raw_coords_count) : Physical_Model_Interface(_raw_coords, _raw_coords_count)
{

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



bool Physical_Model_2D::is_intersecting_with_point(const glm::vec3& _point) const
{
	ASSERT(!m_polygons);

	for (unsigned int i = 0; i < m_polygons_count; ++i)
		if (m_polygons[i].point_belongs_to_triangle(_point)) return true;
	return false;
}

bool Physical_Model_2D::is_intersecting_with_segment(const glm::vec3& _point_1, const glm::vec3& _point_2) const
{
	ASSERT(!m_polygons);

	for (unsigned int i = 0; i < m_polygons_count; ++i)
		if (m_polygons[i].segment_intersecting_polygon(_point_1, _point_2)) return true;
	return false;
}

bool Physical_Model_2D::is_intersecting_with_another_model(const Physical_Model_Interface& _other) const
{
	const Physical_Model_2D& other = (Physical_Model_2D&)_other;

	ASSERT(!m_polygons || !other.m_polygons);

	for (unsigned int i = 0; i < m_polygons_count; ++i)
		for (unsigned int j = 0; j < other.m_polygons_count; ++j)
			if (m_polygons[i].intersects_with_another_polygon(other.m_polygons[i])) return true;
    for (unsigned int i = 0; i < other.m_polygons_count; ++i)
        for (unsigned int j = 0; j < m_polygons_count; ++j)
            if (other.m_polygons[i].intersects_with_another_polygon(m_polygons[i])) return true;

	return false;
}
