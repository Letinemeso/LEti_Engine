#include <Physics/Physical_Models/Rigid_Body_Physical_Model_2D.h>

using namespace LEti;



Rigid_Body_Physical_Model_2D::Rigid_Body_Physical_Model_2D()
    : Physical_Model_2D()
{

}

Rigid_Body_Physical_Model_2D::Rigid_Body_Physical_Model_2D(const Rigid_Body_Physical_Model_2D &_other)
    : Physical_Model_2D(_other)
{
    m_masses = new float[get_polygons_count()];
    for(unsigned int i=0; i<get_polygons_count(); ++i)
        m_masses[i] = _other.m_masses[i];
}

Rigid_Body_Physical_Model_2D::~Rigid_Body_Physical_Model_2D()
{
    delete[] m_masses;
}



Polygon_Holder_Base* Rigid_Body_Physical_Model_2D::M_create_polygons_holder() const
{
    return new Polygon_Holder<Rigid_Body_Polygon>;
}



glm::vec3 Rigid_Body_Physical_Model_2D::M_calculate_center_of_mass() const
{
    glm::vec3 result(0.0f, 0.0f, 0.0f);

    for(unsigned int i=0; i<get_polygons_count(); ++i)
    {
        Rigid_Body_Polygon* polygon = (Rigid_Body_Polygon*)get_polygon(i);
        result += polygon->center() * polygon->mass();
    }
    result /= m_total_mass;

    return result;
}

float Rigid_Body_Physical_Model_2D::M_calculate_moment_of_inertia() const
{
    float result = 0.0f;

    unsigned int counted_points = 0;

    for(unsigned int p=0; p<get_polygons_count(); ++p)
    {
        const Rigid_Body_Polygon& polygon = (const Rigid_Body_Polygon&)*get_polygon(p);
        float point_mass = polygon.mass() * 0.3333333f;

        for(unsigned int v=0; v < 3; ++v)
        {
            if(!polygon.segment_can_collide(v))
                continue;

            glm::vec3 center_to_point = center_of_mass() - polygon[v];
            float distance_squared = (center_to_point.x * center_to_point.x) + (center_to_point.y * center_to_point.y) + (center_to_point.z * center_to_point.z);
            result += distance_squared * point_mass;

            ++counted_points;
        }
    }

    return result;
}



void Rigid_Body_Physical_Model_2D::update(const glm::mat4x4 &_translation, const glm::mat4x4 &_rotation, const glm::mat4x4 &_scale)
{
    Physical_Model_2D::update(_translation, _rotation, _scale);

    m_moment_of_inertia = M_calculate_moment_of_inertia();
}



void Rigid_Body_Physical_Model_2D::set_masses(const float* _masses)
{
    delete[] m_masses;

    m_total_mass = 0.0f;
    m_masses = new float[get_polygons_count()];
    for(unsigned int i=0; i<get_polygons_count(); ++i)
    {
        m_masses[i] = _masses[i];
        m_total_mass += _masses[i];

        Rigid_Body_Polygon* polygon = (Rigid_Body_Polygon*)get_polygon(i);
        polygon->set_mass(m_masses[i]);
    }
}
