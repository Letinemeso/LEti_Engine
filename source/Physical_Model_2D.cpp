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



Geometry::Intersection_Data Physical_Model_2D::Polygon::point_belongs_to_triangle(const glm::vec3& _point) const
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


Geometry::Intersection_Data Physical_Model_2D::Polygon::segment_intersecting_polygon(const glm::vec3 &_point_1, const glm::vec3 &_point_2) const
{
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

Geometry::Intersection_Data Physical_Model_2D::Polygon::intersects_with_another_polygon(const Polygon& _other) const
{
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

glm::vec3& Physical_Model_2D::Polygon::operator[](unsigned int _index)
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



void Physical_Model_2D::update_rectangular_border()
{
	ASSERT(!m_polygons);

	m_current_border.left = m_polygons[0][0].x;
	m_current_border.right = m_polygons[0][0].x;
	m_current_border.top = m_polygons[0][0].y;
	m_current_border.bottom = m_polygons[0][0].y;

	for(unsigned int i=0; i<m_polygons_count; ++i)
	{
		for(unsigned int p=0; p<3; ++p)
		{
			if(m_current_border.left > m_polygons[i][p].x) m_current_border.left = m_polygons[i][p].x;
			if(m_current_border.right < m_polygons[i][p].x) m_current_border.right = m_polygons[i][p].x;
			if(m_current_border.top < m_polygons[i][p].y) m_current_border.top = m_polygons[i][p].y;
			if(m_current_border.bottom > m_polygons[i][p].y) m_current_border.bottom = m_polygons[i][p].y;
		}
	}
}



const Physical_Model_2D::Rectangular_Border& Physical_Model_2D::curr_rect_border() const
{
	return m_current_border;
}



#include <iostream>
Physical_Model_2D::Physical_Model_2D()
{

}

Physical_Model_2D::Physical_Model_2D(const float* _raw_coords, unsigned int _raw_coords_count)
{
	setup(_raw_coords, _raw_coords_count);
}

Physical_Model_2D::Physical_Model_2D(const Physical_Model_2D& _other)
{
	setup(_other.m_raw_coords, _other.m_raw_coords_count);
	copy_real_coordinates(_other);
}

void Physical_Model_2D::setup(const float* _raw_coords, unsigned int _raw_coords_count)
{
	delete[] m_raw_coords;

	m_raw_coords_count = _raw_coords_count;
	m_raw_coords = new float[m_raw_coords_count];
	for (unsigned int i = 0; i < m_raw_coords_count; ++i)
		m_raw_coords[i] = _raw_coords[i];

	delete[] m_polygons;

	m_polygons_count = m_raw_coords_count / 9;
	m_polygons = new Polygon[m_polygons_count];
	for (unsigned int i = 0; i < m_polygons_count; ++i)
		m_polygons[i].setup(&m_raw_coords[i * 9]);
}

Physical_Model_2D::~Physical_Model_2D()
{
	delete[] m_raw_coords;
	delete[] m_polygons;
}


void Physical_Model_2D::update(const glm::mat4x4& _translation, const glm::mat4x4& _rotation, const glm::mat4x4& _scale)
{
	ASSERT(!m_polygons);

	for (unsigned int i = 0; i < m_polygons_count; ++i)
		m_polygons[i].update_points(_translation, _rotation, _scale);

	update_rectangular_border();
}

void Physical_Model_2D::copy_real_coordinates(const Physical_Model_2D &_other)
{
	for (unsigned int i = 0; i < m_polygons_count; ++i)
	{
		for(unsigned int points_i = 0; points_i < 3; ++points_i)
			m_polygons[i][points_i] = _other.m_polygons[i][points_i];
	}
	m_current_border = _other.m_current_border;
}



Geometry::Intersection_Data Physical_Model_2D::is_intersecting_with_point(const glm::vec3& _point) const
{
	ASSERT(!m_polygons);

	for (unsigned int i = 0; i < m_polygons_count; ++i)
		if (m_polygons[i].point_belongs_to_triangle(_point)) return Geometry::Intersection_Data(Geometry::Intersection_Data::Type::intersection);
	return Geometry::Intersection_Data(Geometry::Intersection_Data::Type::none);
}

Geometry::Intersection_Data Physical_Model_2D::is_intersecting_with_segment(const glm::vec3& _point_1, const glm::vec3& _point_2) const
{
	ASSERT(!m_polygons);

	for (unsigned int i = 0; i < m_polygons_count; ++i)
	{
		Geometry::Intersection_Data id = m_polygons[i].segment_intersecting_polygon(_point_1, _point_2);
		if (id) return id;
	}
	return Geometry::Intersection_Data(Geometry::Intersection_Data::Type::none);
}

Geometry::Intersection_Data Physical_Model_2D::is_intersecting_with_another_model(const Physical_Model_2D& _other) const
{
	ASSERT(!m_polygons || !_other.m_polygons);

//	Geometry::Intersection_Data::counter = 0;

	for (unsigned int i = 0; i < m_polygons_count; ++i)
	{
		for (unsigned int j = 0; j < _other.m_polygons_count; ++j)
		{
			Geometry::Intersection_Data id = m_polygons[i].intersects_with_another_polygon(_other.m_polygons[j]);
			if (id) return id;
		}
	}
	for (unsigned int i = 0; i < _other.m_polygons_count; ++i)
	{
		for (unsigned int j = 0; j < m_polygons_count; ++j)
		{
			Geometry::Intersection_Data id = _other.m_polygons[i].intersects_with_another_polygon(m_polygons[j]);
			if (id) return id;
		}
	}

//	std::cout << Geometry::Intersection_Data::counter << "\n";

	return Geometry::Intersection_Data(Geometry::Intersection_Data::Type::none);
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
