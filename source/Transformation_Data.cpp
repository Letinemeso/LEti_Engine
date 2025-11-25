#include <Transformation_Data.h>

#include <gtx/euler_angles.hpp>

using namespace LEti;


Transformation_Data::Transformation_Data()
{
    m_translation_matrix = M_calculate_translation_matrix();
    m_rotation_matrix = M_calculate_rotation_matrix();
    m_scale_matrix = M_calculate_scale_matrix();
    M_update_matrix();
}



void Transformation_Data::set_position(const glm::vec3& _position)
{
    m_modified = true;  //  some code duplicacy here, but who cares

    m_position = _position;

    m_translation_matrix = M_calculate_translation_matrix();
    M_update_matrix();
}
void Transformation_Data::move(const glm::vec3& _vec)
{
    m_modified = true;

    m_position += _vec;

    m_translation_matrix = M_calculate_translation_matrix();
    M_update_matrix();
}
void Transformation_Data::set_rotation(const glm::vec3& _rotation)
{
    m_modified = true;

    m_rotation = _rotation;

    m_rotation_matrix = M_calculate_rotation_matrix();
    M_update_matrix();
}
void Transformation_Data::rotate(const glm::vec3& _vec)
{
    m_modified = true;

    m_rotation += _vec;

    m_rotation_matrix = M_calculate_rotation_matrix();
    M_update_matrix();
}
void Transformation_Data::set_scale(const glm::vec3& _scale)
{
    m_modified = true;

    m_scale = _scale;

    m_scale_matrix = M_calculate_scale_matrix();
    M_update_matrix();
}



glm::mat4x4 Transformation_Data::M_calculate_translation_matrix() const
{
    glm::mat4x4 translation_matrix
        {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            position().x, position().y, position().z, 1.0f
        };

    return translation_matrix;
}

glm::mat4x4 Transformation_Data::M_calculate_rotation_matrix() const
{
    glm::quat qx = glm::angleAxis(m_rotation.x, glm::vec3(1, 0, 0));
    glm::quat qy = glm::angleAxis(m_rotation.y, glm::vec3(0, 1, 0));
    glm::quat qz = glm::angleAxis(m_rotation.z, glm::vec3(0, 0, 1));

    glm::quat rotation_quat = glm::normalize(qz * qy * qx);

    return glm::mat4_cast(rotation_quat);

    // return glm::yawPitchRoll(m_rotation.y, m_rotation.x, m_rotation.z);
}

glm::mat4x4 Transformation_Data::M_calculate_scale_matrix() const
{
    glm::mat4x4 scale_matrix
        {
            scale().x, 0.0f, 0.0f, 0.0f,
            0.0f, scale().y, 0.0f, 0.0f,
            0.0f, 0.0f, scale().z, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };

    return scale_matrix;
}

void Transformation_Data::M_update_matrix()
{
    m_matrix = translation_matrix() * rotation_matrix() * scale_matrix();
}



glm::vec3 Transformation_Data::get_position_for_ratio(const Transformation_Data& _previous_state, const Transformation_Data& _current_state, float _ratio)
{
    L_ASSERT(_ratio > -0.0001f && _ratio < 1.0001f);

    glm::vec3 curr_pos = _current_state.position();
    glm::vec3 prev_pos = _previous_state.position();
    glm::vec3 diff = curr_pos - prev_pos;
    diff *= _ratio;

    return diff + prev_pos;
}

glm::vec3 Transformation_Data::get_rotation_for_ratio(const Transformation_Data& _previous_state, const Transformation_Data& _current_state, float _ratio)
{
    L_ASSERT(_ratio > -0.0001f && _ratio < 1.0001f);

    glm::vec3 curr_rotation = _current_state.rotation();
    glm::vec3 prev_rotation = _previous_state.rotation();
    glm::vec3 diff = curr_rotation - prev_rotation;
    diff *= _ratio;

    return diff + prev_rotation;
}

glm::vec3 Transformation_Data::get_scale_for_ratio(const Transformation_Data& _previous_state, const Transformation_Data& _current_state, float _ratio)
{
    L_ASSERT(_ratio > -0.0001f && _ratio < 1.0001f);

    glm::vec3 curr_scale = _current_state.scale();
    glm::vec3 prev_scale = _previous_state.scale();
    glm::vec3 diff = curr_scale - prev_scale;
    diff *= _ratio;

    return diff + prev_scale;
}


glm::mat4x4 Transformation_Data::get_translation_matrix_for_ratio(const Transformation_Data &_previous_state, const Transformation_Data &_current_state, float _ratio)
{
    glm::vec3 diff = get_position_for_ratio(_previous_state, _current_state, _ratio);

    glm::mat4x4 result
    {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        diff.x, diff.y, diff.z, 1.0f
    };

    return result;
}

glm::mat4x4 Transformation_Data::get_translation_matrix_inversed_for_ratio(const Transformation_Data &_previous_state, const Transformation_Data &_current_state, float _ratio)
{
    L_ASSERT(_ratio > -0.0001f && _ratio < 1.0001f);

    glm::vec3 curr_pos = _current_state.position();
    glm::vec3 prev_pos = _previous_state.position();
    glm::vec3 diff = curr_pos - prev_pos;
    diff *= _ratio;

    diff = curr_pos - diff;

    glm::mat4x4 result
    {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        diff.x, diff.y, diff.z, 1.0f
    };

    return result;
}

glm::mat4x4 Transformation_Data::get_rotation_matrix_for_ratio(const Transformation_Data &_previous_state, const Transformation_Data &_current_state, float _ratio)
{
    glm::vec3 diff = get_rotation_for_ratio(_previous_state, _current_state, _ratio);

    glm::mat4x4 result
    {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    for(unsigned int i=0; i<3; ++i)
    {
        glm::vec3 axis(0.0f, 0.0f, 0.0f);
        axis[i] = 1.0f;

        glm::mat4x4 rotation_around_axis = glm::rotate(diff[i], axis);

        result *= rotation_around_axis;
    }

    return result;
}

glm::mat4x4 Transformation_Data::get_rotation_matrix_inversed_for_ratio(const Transformation_Data &_previous_state, const Transformation_Data &_current_state, float _ratio)
{
    glm::vec3 diff = get_rotation_for_ratio(_previous_state, _current_state, _ratio);

    glm::mat4x4 result
    {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    for(unsigned int i=0; i<3; ++i)
    {
        glm::vec3 axis(0.0f, 0.0f, 0.0f);
        axis[i] = -1.0f;

        glm::mat4x4 rotation_around_axis = glm::rotate(diff[i], axis);

        result *= rotation_around_axis;
    }

    return result;
}

glm::mat4x4 Transformation_Data::get_scale_matrix_for_ratio(const Transformation_Data &_previous_state, const Transformation_Data &_current_state, float _ratio)
{
    glm::vec3 diff = get_scale_for_ratio(_previous_state, _current_state, _ratio);

    glm::mat4x4 result
    {
        diff.x, 0.0f, 0.0f, 0.0f,
        0.0f, diff.y, 0.0f, 0.0f,
        0.0f, 0.0f, diff.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    return result;
}

glm::mat4x4 Transformation_Data::get_scale_matrix_inversed_for_ratio(const Transformation_Data &_previous_state, const Transformation_Data &_current_state, float _ratio)
{
    glm::vec3 curr_scale = _current_state.scale();
    glm::vec3 prev_scale = _previous_state.scale();
    glm::vec3 diff = curr_scale - prev_scale;
    diff *= _ratio;

    diff = curr_scale - diff;

    glm::mat4x4 result
    {
        diff.x, 0.0f, 0.0f, 0.0f,
        0.0f, diff.y, 0.0f, 0.0f,
        0.0f, 0.0f, diff.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    return result;
}


Transformation_Data Transformation_Data::get_transformation_data_for_ratio(const Transformation_Data& _previous_state, const Transformation_Data& _current_state, float _ratio)
{
    Transformation_Data result;

    result.set_position(Transformation_Data::get_position_for_ratio(_previous_state, _current_state, _ratio));
    result.set_rotation(Transformation_Data::get_rotation_for_ratio(_previous_state, _current_state, _ratio));
    result.set_scale(Transformation_Data::get_scale_for_ratio(_previous_state, _current_state, _ratio));

    return result;
}
