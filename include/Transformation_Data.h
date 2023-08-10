#ifndef TRANSFORMATION_DATA_H
#define TRANSFORMATION_DATA_H

#include <vec3.hpp>
#include <mat4x4.hpp>
#include <gtx/transform.hpp>

#include <L_Debug/L_Debug.h>


namespace LEti
{

    class Transformation_Data final
    {
    private:
        glm::vec3 m_position;
        glm::vec3 m_rotation;
        glm::vec3 m_scale;

        glm::mat4x4 m_matrix;

    public:
        inline const glm::vec3& position() const { return m_position; }
        inline const glm::vec3& rotation() const { return m_rotation; }
        inline const glm::vec3& scale() const { return m_scale; }

        glm::mat4x4 translation_matrix() const;
        glm::mat4x4 rotation_matrix() const;
        glm::mat4x4 scale_matrix() const;

        inline const glm::mat4x4& matrix() const { return m_matrix; }

    public:
        inline void set_position(const glm::vec3& _position) { m_position = _position; }
        inline void move(const glm::vec3& _vec) { m_position += _vec; }
        inline void set_rotation(const glm::vec3& _rotation) { m_rotation = _rotation; }
        inline void rotate(const glm::vec3& _vec) { m_rotation += _vec; }
        inline void set_scale(const glm::vec3& _scale) { m_scale = _scale; }

    public:
        void update_matrix();

    public:
        static glm::vec3 get_position_for_ratio(const Transformation_Data& _previous_state, const Transformation_Data& _current_state, float _ratio);
        static glm::vec3 get_rotation_for_ratio(const Transformation_Data& _previous_state, const Transformation_Data& _current_state, float _ratio);
        static glm::vec3 get_scale_for_ratio(const Transformation_Data& _previous_state, const Transformation_Data& _current_state, float _ratio);

        static glm::mat4x4 get_translation_matrix_for_ratio(const Transformation_Data& _previous_state, const Transformation_Data& _current_state, float _ratio);
        static glm::mat4x4 get_translation_matrix_inversed_for_ratio(const Transformation_Data& _previous_state, const Transformation_Data& _current_state, float _ratio);
        static glm::mat4x4 get_rotation_matrix_for_ratio(const Transformation_Data& _previous_state, const Transformation_Data& _current_state, float _ratio);
        static glm::mat4x4 get_rotation_matrix_inversed_for_ratio(const Transformation_Data& _previous_state, const Transformation_Data& _current_state, float _ratio);
        static glm::mat4x4 get_scale_matrix_for_ratio(const Transformation_Data& _previous_state, const Transformation_Data& _current_state, float _ratio);
        static glm::mat4x4 get_scale_matrix_inversed_for_ratio(const Transformation_Data& _previous_state, const Transformation_Data& _current_state, float _ratio);

    };



}

#endif // TRANSFORMATION_DATA_H
