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

    result.k = substr.y / substr.x;
    result.b = _point_1.y - _point_1.x * result.k;

    return result;
}



bool Physical_Model_2D::Polygon::segment_intersecting_polygon(const glm::vec3 &_point_1, const glm::vec3 &_point_2) const
{
    Equasion_Data input_segment_eq = get_equasion(_point_1, _point_2);

    Equasion_Data AB_eq = get_equasion(m_actual_A, m_actual_B);
    Equasion_Data BC_eq = get_equasion(m_actual_B, m_actual_C);
    Equasion_Data CA_eq = get_equasion(m_actual_C, m_actual_A);


}
