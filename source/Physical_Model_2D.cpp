#include "../include/Physical_Model_2D.h"

using namespace LEti;





Physical_Model_2D::Imprint::Imprint(const Geometry_2D::Polygon* _polygons, unsigned int _polygons_count, const Physical_Model_2D* _parent)
	: m_parent(_parent)
{
	m_polygons_count = _polygons_count;
	m_polygons = new Geometry_2D::Polygon[m_polygons_count];
	for(unsigned int i=0; i<m_polygons_count; ++i)
		m_polygons[i].setup(_polygons[i]);
	m_rect_border = _parent->curr_rect_border();
}


Physical_Model_2D::Imprint::Imprint(Imprint&& _other)
{
	delete[] m_polygons;

	m_polygons = _other.m_polygons;
	_other.m_polygons = nullptr;
	m_polygons_count = _other.m_polygons_count;
	_other.m_polygons_count = 0;
	m_parent = _other.m_parent;
	_other.m_parent = nullptr;
	m_rect_border = _other.m_rect_border;
}

Physical_Model_2D::Imprint::Imprint(const Imprint& _other)
	: m_parent(_other.m_parent)
{
	m_polygons_count = _other.m_polygons_count;
	m_polygons = new Geometry_2D::Polygon[m_polygons_count];
	for(unsigned int i=0; i<m_polygons_count; ++i)
		m_polygons[i].setup(_other.m_polygons[i]);
	m_rect_border = _other.m_rect_border;
}

Physical_Model_2D::Imprint::~Imprint()
{
	delete[] m_polygons;
}


void Physical_Model_2D::Imprint::update(const glm::mat4x4 &_translation, const glm::mat4x4 &_rotation, const glm::mat4x4 &_scale)
{
	for(unsigned int i=0; i<m_polygons_count; ++i)
		m_polygons[i].update_points(_translation, _rotation, _scale);
}

void Physical_Model_2D::Imprint::update_to_current_model_state()
{
	for(unsigned int i=0; i<m_polygons_count; ++i)
	{
		for(unsigned int vert=0; vert<3; ++vert)
			m_polygons[i][vert] = m_parent->m_polygons[i][vert];
	}
	m_rect_border = m_parent->curr_rect_border();
}


const Geometry_2D::Polygon& Physical_Model_2D::Imprint::operator[](unsigned int _index) const
{
	ASSERT(_index >= m_polygons_count);

	return m_polygons[_index];
}

const Physical_Model_2D* Physical_Model_2D::Imprint::get_parent() const
{
	return m_parent;
}

Geometry::Intersection_Data Physical_Model_2D::Imprint::imprints_intersect(const Imprint &_other) const
{
	ASSERT(!m_polygons || !_other.m_polygons);

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

	return Geometry::Intersection_Data(Geometry::Intersection_Data::Type::none);
}

const Physical_Model_2D::Rectangular_Border& Physical_Model_2D::Imprint::curr_rect_border() const
{
	return m_rect_border;
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
	m_polygons = new Geometry_2D::Polygon[m_polygons_count];
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


Physical_Model_2D::Imprint Physical_Model_2D::create_imprint() const
{
	return Imprint(m_polygons, m_polygons_count, this);
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

	return Geometry::Intersection_Data(Geometry::Intersection_Data::Type::none);
}

Geometry::Intersection_Data Physical_Model_2D::is_intersecting_with_another_model(const Physical_Model_2D::Imprint &_other) const
{
	ASSERT(!m_polygons || !_other.m_polygons);

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

	return Geometry::Intersection_Data(Geometry::Intersection_Data::Type::none);
}



unsigned int Physical_Model_2D::get_polygons_count() const
{
	return m_polygons_count;
}

const Geometry_2D::Polygon& Physical_Model_2D::operator[](unsigned int _index) const
{
	ASSERT(!m_polygons || _index >= m_polygons_count);

	return m_polygons[_index];
}

