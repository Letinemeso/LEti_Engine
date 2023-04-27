#include <Physics/Physical_Models/Polygon.h>

using namespace LEti;


Polygon::Polygon()
{

}

Polygon::Polygon(const Polygon& _other)
{
    m_raw_coords = _other.m_raw_coords;
    m_segment_can_collide = _other.m_segment_can_collide;
    m_actual_A = _other.m_actual_A;
    m_actual_B = _other.m_actual_B;
    m_actual_C = _other.m_actual_C;
    m_center_raw = _other.m_center_raw;
    m_center = _other.m_center;
}

void Polygon::setup(const float *_raw_coords, const bool* _segment_can_collide)
{
    m_raw_coords = _raw_coords;
    m_segment_can_collide = _segment_can_collide;

    calculate_center();

    L_ASSERT(m_raw_coords && m_segment_can_collide);
}

void Polygon::setup(const Polygon& _other)
{
    m_raw_coords = _other.m_raw_coords;
    m_segment_can_collide = _other.m_segment_can_collide;
    m_actual_A = _other.m_actual_A;
    m_actual_B = _other.m_actual_B;
    m_actual_C = _other.m_actual_C;
    m_center_raw = _other.m_center_raw;
    m_center = _other.m_center;
}

void Polygon::calculate_center()
{
    glm::vec3 sum{0.0f, 0.0f, 0.0f};
    for(unsigned int i=0; i<m_count; i += 3)
        sum += glm::vec3(m_raw_coords[i], m_raw_coords[i + 1], m_raw_coords[i + 2]);
    m_center_raw = sum / 3.0f;
}

void Polygon::update_points(const glm::mat4x4 &_translation, const glm::mat4x4 &_rotation, const glm::mat4x4 &_scale)
{
    L_ASSERT(!(!m_raw_coords));

    glm::mat4x4 result_matrix = _translation * _rotation * _scale;
    update_points_with_single_matrix(result_matrix);
}

void Polygon::update_points_with_single_matrix(const glm::mat4x4 &_matrix)
{
    L_ASSERT(!(!m_raw_coords));

    m_actual_A = _matrix * glm::vec4(m_raw_coords[0], m_raw_coords[1], m_raw_coords[2], 1.0f);
    m_actual_B = _matrix * glm::vec4(m_raw_coords[3], m_raw_coords[4], m_raw_coords[5], 1.0f);
    m_actual_C = _matrix * glm::vec4(m_raw_coords[6], m_raw_coords[7], m_raw_coords[8], 1.0f);
    m_center = _matrix * glm::vec4(m_center_raw, 1.0f);
}


const glm::vec3& Polygon::operator[](unsigned int _index) const
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

glm::vec3& Polygon::operator[](unsigned int _index)
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

bool Polygon::segment_can_collide(unsigned int _index) const
{
    L_ASSERT(_index < 3);

    return m_segment_can_collide[_index];
}

const glm::vec3& Polygon::center() const
{
    return m_center;
}

const glm::vec3& Polygon::center_raw() const
{
    return m_center_raw;
}
