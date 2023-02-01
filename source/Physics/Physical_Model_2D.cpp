#include "../../include/Physics/Physical_Model_2D.h"

using namespace LEti;


Physical_Model_2D::Imprint::Imprint(const Geometry::Polygon* _polygons, unsigned int _polygons_count, const Physical_Model_2D* _parent)
	: m_parent(_parent)
{
	m_polygons_count = _polygons_count;
	m_polygons = new Geometry::Polygon[m_polygons_count];
	for(unsigned int i=0; i<m_polygons_count; ++i)
		m_polygons[i].setup(_polygons[i]);
	m_rect_border = _parent->curr_rect_border();
	m_center_of_mass_raw = _parent->m_center_of_mass_raw;
	m_center_of_mass = _parent->m_center_of_mass;
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
	m_center_of_mass_raw = _other.m_center_of_mass_raw;
	m_center_of_mass = _other.m_center_of_mass;
}

Physical_Model_2D::Imprint::Imprint(const Imprint& _other)
	: m_parent(_other.m_parent)
{
	m_polygons_count = _other.m_polygons_count;
	m_polygons = new Geometry::Polygon[m_polygons_count];
	for(unsigned int i=0; i<m_polygons_count; ++i)
		m_polygons[i].setup(_other.m_polygons[i]);
	m_rect_border = _other.m_rect_border;
	m_center_of_mass_raw = _other.m_center_of_mass_raw;
	m_center_of_mass = _other.m_center_of_mass;
}

Physical_Model_2D::Imprint::~Imprint()
{
	delete[] m_polygons;
}



void Physical_Model_2D::Imprint::update_rectangular_border()
{
	L_ASSERT(!(!m_polygons));

	m_rect_border.left = m_polygons[0][0].x;
	m_rect_border.right = m_polygons[0][0].x;
	m_rect_border.top = m_polygons[0][0].y;
	m_rect_border.bottom = m_polygons[0][0].y;

	for(unsigned int i=0; i<m_polygons_count; ++i)
	{
		for(unsigned int p=0; p<3; ++p)
		{
			if(m_rect_border.left > m_polygons[i][p].x) m_rect_border.left = m_polygons[i][p].x;
			if(m_rect_border.right < m_polygons[i][p].x) m_rect_border.right = m_polygons[i][p].x;
			if(m_rect_border.top < m_polygons[i][p].y) m_rect_border.top = m_polygons[i][p].y;
			if(m_rect_border.bottom > m_polygons[i][p].y) m_rect_border.bottom = m_polygons[i][p].y;
		}
	}
}



void Physical_Model_2D::Imprint::update(const glm::mat4x4 &_translation, const glm::mat4x4 &_rotation, const glm::mat4x4 &_scale)
{
	L_ASSERT(!(!m_polygons));

	glm::mat4x4 result_matrix = _translation * _rotation * _scale;

	for (unsigned int i = 0; i < m_polygons_count; ++i)
		m_polygons[i].update_points_with_single_matrix(result_matrix);

	m_center_of_mass = result_matrix * glm::vec4(m_center_of_mass_raw, 1.0f);
}

void Physical_Model_2D::Imprint::update_with_single_matrix(const glm::mat4x4& _matrix)
{
	L_ASSERT(!(!m_polygons));

	for(unsigned int i=0; i<m_polygons_count; ++i)
		m_polygons[i].update_points_with_single_matrix(_matrix);
	update_rectangular_border();
}

void Physical_Model_2D::Imprint::update_to_current_model_state()
{
	L_ASSERT(!(!m_polygons));

	for(unsigned int i=0; i<m_polygons_count; ++i)
	{
		for(unsigned int vert=0; vert<3; ++vert)
			m_polygons[i][vert] = m_parent->m_polygons[i][vert];
	}
	m_rect_border = m_parent->curr_rect_border();

	m_center_of_mass = m_parent->m_center_of_mass;
}


const Geometry::Polygon& Physical_Model_2D::Imprint::operator[](unsigned int _index) const
{
	L_ASSERT(!(_index >= m_polygons_count));

	return m_polygons[_index];
}

const Physical_Model_2D* Physical_Model_2D::Imprint::get_parent() const
{
	return m_parent;
}

unsigned int Physical_Model_2D::Imprint::get_polygons_count() const
{
	return m_parent->get_polygons_count();
}

const Geometry_2D::Rectangular_Border& Physical_Model_2D::Imprint::curr_rect_border() const
{
	return m_rect_border;
}

const glm::vec3& Physical_Model_2D::Imprint::center_of_mass() const
{
	return m_center_of_mass;
}



void Physical_Model_2D::update_rectangular_border()
{
	L_ASSERT(!(!m_polygons));

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

void Physical_Model_2D::update_moment_of_inertia()
{
	L_ASSERT(!(!m_polygons));

	float m_mass = 1.0f;

	m_moment_of_inertia = 0.0f;
	for(unsigned int i=0; i<m_polygons_count; ++i)
	{
		float distance = LEti::Math::vector_length(m_center_of_mass - m_polygons[i].center_of_mass());
		m_moment_of_inertia += distance * distance;
	}
	m_moment_of_inertia *= m_mass;
	m_moment_of_inertia /= m_polygons_count;
	m_moment_of_inertia *= 3.0;
}



const Geometry_2D::Rectangular_Border& Physical_Model_2D::curr_rect_border() const
{
	return m_current_border;
}



Physical_Model_2D::Physical_Model_2D()
{

}

Physical_Model_2D::Physical_Model_2D(const Physical_Model_2D& _other)
{
	setup(_other.m_raw_coords, _other.m_raw_coords_count, _other.m_collision_permissions);
	copy_real_coordinates(_other);
}

void Physical_Model_2D::setup(const float* _raw_coords, unsigned int _raw_coords_count, const bool* _collision_permissions)
{
	delete[] m_raw_coords;

	m_raw_coords_count = _raw_coords_count;
	m_raw_coords = new float[m_raw_coords_count];
	for (unsigned int i = 0; i < m_raw_coords_count; ++i)
		m_raw_coords[i] = _raw_coords[i];

	delete[] m_collision_permissions;

	m_collision_permissions = new bool[m_raw_coords_count / 3];
	for(unsigned int i=0; i<m_raw_coords_count / 3; ++i)
		m_collision_permissions[i] = _collision_permissions[i];

	delete[] m_polygons;

	m_polygons_count = m_raw_coords_count / 9;
	m_polygons = new Geometry::Polygon[m_polygons_count];
	for (unsigned int i = 0; i < m_polygons_count; ++i)
		m_polygons[i].setup(&m_raw_coords[i * 9], &m_collision_permissions[i * 3]);

	m_center_of_mass_raw = {0.0f, 0.0f, 0.0f};
	for(unsigned int i=0; i < m_polygons_count; ++i)
		m_center_of_mass_raw += m_polygons[i].center_of_mass_raw();
	m_center_of_mass_raw /= (float)m_polygons_count;

	update_moment_of_inertia();
}

Physical_Model_2D::~Physical_Model_2D()
{
	delete[] m_raw_coords;
	delete[] m_polygons;
}


void Physical_Model_2D::update(const glm::mat4x4& _translation, const glm::mat4x4& _rotation, const glm::mat4x4& _scale)
{
	L_ASSERT(!(!m_polygons));

	glm::mat4x4 result_matrix = _translation * _rotation * _scale;

	for (unsigned int i = 0; i < m_polygons_count; ++i)
		m_polygons[i].update_points_with_single_matrix(result_matrix);

	m_center_of_mass = result_matrix * glm::vec4(m_center_of_mass_raw, 1.0f);

	update_moment_of_inertia();

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
	m_center_of_mass_raw = _other.m_center_of_mass_raw;
	m_center_of_mass = _other.m_center_of_mass;
}


Physical_Model_2D::Imprint Physical_Model_2D::create_imprint() const
{
	return Imprint(m_polygons, m_polygons_count, this);
}



const Geometry::Polygon* Physical_Model_2D::get_polygons() const
{
	return m_polygons;
}

unsigned int Physical_Model_2D::get_polygons_count() const
{
	return m_polygons_count;
}

const Geometry::Polygon& Physical_Model_2D::operator[](unsigned int _index) const
{
	L_ASSERT(!(!m_polygons || _index >= m_polygons_count));

	return m_polygons[_index];
}

const glm::vec3& Physical_Model_2D::center_of_mass() const
{
	return m_center_of_mass;
}

float Physical_Model_2D::moment_of_inertia() const
{
	return m_moment_of_inertia;
}
