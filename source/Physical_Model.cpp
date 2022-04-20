#include "../include/Physical_Model.h"

using namespace LEti;


//  Polygon implementation

Physical_Model::Pyramid::Polygon::Polygon()
{

}

void Physical_Model::Pyramid::Polygon::setup(const float* _raw_coords)
{
	m_raw_coords = _raw_coords;

	ASSERT(!m_raw_coords);
}

void Physical_Model::Pyramid::Polygon::update_points(const glm::mat4x4& _translation, const glm::mat4x4& _rotation, const glm::mat4x4& _scale)
{
	ASSERT(!m_raw_coords);

	glm::mat4x4 result_matrix = _translation * _rotation * _scale;
	m_actual_A = result_matrix * glm::vec4(m_raw_coords[0], m_raw_coords[1], m_raw_coords[2], 1.0f);
	m_actual_B = result_matrix * glm::vec4(m_raw_coords[3], m_raw_coords[4], m_raw_coords[5], 1.0f);
	m_actual_C = result_matrix * glm::vec4(m_raw_coords[6], m_raw_coords[7], m_raw_coords[8], 1.0f);
}



glm::vec3 Physical_Model::Pyramid::Polygon::get_normal() const
{
	ASSERT(!m_raw_coords);

	glm::vec3 AB = m_actual_B - m_actual_A;
	glm::vec3 AC = m_actual_C - m_actual_A;

	return Utility::normalize(AB, AC);
}

Physical_Model::Pyramid::Polygon::Plane_Equasion_Data Physical_Model::Pyramid::Polygon::get_equasion() const
{
	ASSERT(!m_raw_coords);

	glm::vec3 normal = get_normal();

	Plane_Equasion_Data result;
	result.x_part = normal.x;
	result.y_part = normal.y;
	result.z_part = normal.z;
	result.constant_part = -(normal.x * m_actual_A.x + normal.y * m_actual_A.y + normal.z * m_actual_A.z);

	return result;
}

bool Physical_Model::Pyramid::Polygon::point_belongs_to_triangle(const glm::vec3& _point) const
{
	ASSERT(!m_raw_coords);

    glm::vec3 normal = get_normal();

    float mult1 = Utility::mixed_vector_multiplication(normal, m_actual_A - _point, m_actual_B - _point);
    float mult2 = Utility::mixed_vector_multiplication(normal, m_actual_B - _point, m_actual_C - _point);
    float mult3 = Utility::mixed_vector_multiplication(normal, m_actual_C - _point, m_actual_A - _point);

    if (mult1 >= 0 && mult2 >= 0 && mult3 >= 0)
        return true;
    return false;
}



glm::vec3 Physical_Model::Pyramid::Polygon::get_intersection_point(const glm::vec3& _start, const glm::vec3& _direction) const
{
	ASSERT(!m_raw_coords);

	Plane_Equasion_Data ped = get_equasion();

    float t = (_start.x * ped.x_part + _start.y * ped.y_part + _start.z * ped.z_part + ped.constant_part) /
        (_direction.x * ped.x_part + _direction.y * ped.y_part + _direction.z * ped.z_part);
    t *= -1;

    return { _direction.x * t + _start.x, _direction.y * t + _start.y, _direction.z * t + _start.z };
}

bool Physical_Model::Pyramid::Polygon::beam_intersecting_polygon(const glm::vec3& _beam_pos, const glm::vec3& _beam_direction) const
{
	ASSERT(!m_raw_coords);

	glm::vec3 intersection_point = get_intersection_point(_beam_pos, _beam_direction);

    glm::vec3 ip_direction = intersection_point - _beam_pos;
    float angle_cos = (ip_direction.x * _beam_direction.x + ip_direction.y * _beam_direction.y + ip_direction.z * _beam_direction.z) /
            ( Utility::vector_length(ip_direction) + Utility::vector_length(_beam_direction) );

    if(angle_cos > 0.001f)
        return point_belongs_to_triangle(intersection_point);
    else return false;
}

bool Physical_Model::Pyramid::Polygon::segment_intersecting_polygon(const glm::vec3 &_beam_pos, const glm::vec3 &_beam_direction) const
{
    ASSERT(!m_raw_coords);

    glm::vec3 intersection_point = get_intersection_point(_beam_pos, _beam_direction);
    glm::vec3 ip_direction = intersection_point - _beam_pos;

    float ip_length = Utility::vector_length(ip_direction),
            beam_length = Utility::vector_length(_beam_direction);

    if(ip_length > beam_length) return false;

    float angle_cos = (ip_direction.x * _beam_direction.x + ip_direction.y * _beam_direction.y + ip_direction.z * _beam_direction.z) /
            ( ip_length + beam_length );

    if(angle_cos > 0.001f)
        return point_belongs_to_triangle(intersection_point);
    else return false;
}

bool Physical_Model::Pyramid::Polygon::point_is_on_the_right(const glm::vec3& _point) const
{
	ASSERT(!m_raw_coords);

	Plane_Equasion_Data ped = get_equasion();
	float result = ped.x_part * _point.x + ped.y_part * _point.y + ped.z_part * _point.z + ped.constant_part;
	return result > 0.0f;
}

bool Physical_Model::Pyramid::Polygon::point_is_on_the_left(const glm::vec3& _point) const
{
	return !point_is_on_the_right(_point);
}



const glm::vec3& Physical_Model::Pyramid::Polygon::A() const
{
    return m_actual_A;
}

const glm::vec3& Physical_Model::Pyramid::Polygon::B() const
{
    return m_actual_B;
}

const glm::vec3& Physical_Model::Pyramid::Polygon::C() const
{
    return m_actual_C;
}



//  Pyramid implementation

Physical_Model::Pyramid::Pyramid()
{

}

void Physical_Model::Pyramid::setup(const float* _raw_coords)
{
	m_raw_coords = _raw_coords;

	ASSERT(!m_raw_coords);

	for (unsigned int i = 0; i < 4; ++i)
		m_polygons[i].setup(_raw_coords + (9 * i));
}

void Physical_Model::Pyramid::update_polygons(const glm::mat4x4& _translation, const glm::mat4x4& _rotation, const glm::mat4x4& _scale)
{
	ASSERT(!m_raw_coords);

	for (unsigned int i = 0; i < 4; ++i)
		m_polygons[i].update_points(_translation, _rotation, _scale);
}



bool Physical_Model::Pyramid::point_belongs_to_pyramid(const glm::vec3& _point) const
{
	ASSERT(!m_raw_coords);

	for (unsigned int i = 0; i < 4; ++i)
		if (m_polygons[i].point_is_on_the_right(_point)) return false;
	return true;
}

bool Physical_Model::Pyramid::is_intersecting_with_beam(const glm::vec3 &_start, const glm::vec3 &_direction) const
{
    ASSERT(!m_raw_coords);

    for(unsigned int i=0; i<4; ++i)
        if(m_polygons[i].beam_intersecting_polygon(_start, _direction)) return true;
    return false;
}

bool Physical_Model::Pyramid::is_intersecting_with_segment(const glm::vec3 &_start, const glm::vec3 &_direction) const
{
    ASSERT(!m_raw_coords);

    for(unsigned int i=0; i<4; ++i)
        if(m_polygons[i].segment_intersecting_polygon(_start, _direction)) return true;
    return false;
}



const Physical_Model::Pyramid::Polygon& Physical_Model::Pyramid::operator[](unsigned int _index) const
{
    ASSERT(!m_raw_coords || _index > 3);

    return m_polygons[_index];
}



//  Physical_Model implementation

Physical_Model::Physical_Model()
{

}

Physical_Model::Physical_Model(const float* _raw_coords, unsigned int _raw_coords_count)
{
	setup(_raw_coords, _raw_coords_count);
}

void Physical_Model::setup(const float* _raw_coords, unsigned int _raw_coords_count)
{
	delete[] m_raw_coords;
	delete[] m_pyramids;

	m_raw_coords_count = _raw_coords_count;
	m_pyramids_count = _raw_coords_count / 36;

	m_raw_coords = new float[m_raw_coords_count];
	m_pyramids = new Pyramid[m_pyramids_count];

	for (unsigned int i = 0; i < m_raw_coords_count; ++i)
		m_raw_coords[i] = _raw_coords[i];
	for (unsigned int i = 0; i < m_pyramids_count; ++i)
		m_pyramids[i].setup(m_raw_coords + (36 * i));
}


Physical_Model::~Physical_Model()
{
	delete[] m_raw_coords;
	delete[] m_pyramids;
}


void Physical_Model::update(const glm::mat4x4& _translation, const glm::mat4x4& _rotation, const glm::mat4x4& _scale)
{
	ASSERT(!m_raw_coords || !m_pyramids);

	for (unsigned int i = 0; i < m_pyramids_count; ++i)
		m_pyramids[i].update_polygons(_translation, _rotation, _scale);
}



bool Physical_Model::is_intersecting_with_point(const glm::vec3& _point) const
{
	ASSERT(!m_raw_coords || !m_pyramids);

	for (unsigned int i = 0; i < m_pyramids_count; ++i)
		if (m_pyramids[i].point_belongs_to_pyramid(_point)) return true;
	return false;
}

bool Physical_Model::is_intersecting_with_beam(const glm::vec3 &_start, const glm::vec3 &_direction) const
{
    ASSERT(!m_raw_coords || !m_pyramids);

    for (unsigned int i = 0; i < m_pyramids_count; ++i)
        if (m_pyramids[i].is_intersecting_with_beam(_start, _direction)) return true;
    return false;
}

bool Physical_Model::is_intersecting_with_segment(const glm::vec3 &_start, const glm::vec3 &_direction) const
{
    ASSERT(!m_raw_coords || !m_pyramids);

    for (unsigned int i = 0; i < m_pyramids_count; ++i)
        if (m_pyramids[i].is_intersecting_with_segment(_start, _direction)) return true;
    return false;
}

bool Physical_Model::is_intersecting_with_another_model(const Physical_Model &_other) const
{
    ASSERT(!m_raw_coords || !m_pyramids || !_other.m_raw_coords || !_other.m_pyramids);

    for(unsigned int pyr = 0; pyr < m_pyramids_count; ++pyr)
    {
        for(unsigned int other_pyr = 0; other_pyr < _other.m_pyramids_count; ++other_pyr)
        {
            for(unsigned int i=0; i<4; ++i)
            {
                const Pyramid::Polygon& crnt_polygon = _other.m_pyramids[other_pyr][i];

                glm::vec3 AB = crnt_polygon.B() - crnt_polygon.A();
                glm::vec3 AC = crnt_polygon.C() - crnt_polygon.A();
                glm::vec3 BC = crnt_polygon.C() - crnt_polygon.B();

                if(is_intersecting_with_segment(crnt_polygon.A(), AB) ||
                        is_intersecting_with_segment(crnt_polygon.A(), AC) ||
                        is_intersecting_with_segment(crnt_polygon.B(), BC))
                    return true;
            }
        }
    }
    return false;
}



























































