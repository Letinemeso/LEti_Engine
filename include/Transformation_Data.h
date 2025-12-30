#pragma once

#include <vec3.hpp>
#include <mat4x4.hpp>
#include <gtx/transform.hpp>
#include <gtc/quaternion.hpp>

#include <L_Debug/L_Debug.h>


namespace LEti
{

    class Transformation_Data final
    {
    private:
        bool m_modified = true;

    private:
        glm::vec3 m_position = {0.0f, 0.0f, 0.0f};
        glm::quat m_rotation = {0.0f, 0.0f, 0.0f, 0.0f};
        glm::vec3 m_scale = {1.0f, 1.0f, 1.0f};

        glm::mat4x4 m_translation_matrix;
        glm::mat4x4 m_rotation_matrix;
        glm::mat4x4 m_scale_matrix;

        glm::mat4x4 m_matrix;

    public:
        Transformation_Data();

    public:
        inline const glm::vec3& position() const { return m_position; }
        inline const glm::quat& rotation() const { return m_rotation; }
        inline const glm::vec3& scale() const { return m_scale; }

        inline const glm::mat4x4& translation_matrix() const { return m_translation_matrix; }
        inline const glm::mat4x4& rotation_matrix() const { return m_rotation_matrix; }
        inline const glm::mat4x4& scale_matrix() const { return m_scale_matrix; }

        inline const glm::mat4x4& matrix() const { return m_matrix; }

        inline bool modified() const { return m_modified; }
        inline void mark_unmodified() { m_modified = false; }

    public:
        void set_position(const glm::vec3& _position);
        void move(const glm::vec3& _vec);
        void set_rotation(const glm::quat& _rotation);
        void set_rotation(const glm::vec3& _euler_angles);
        void set_rotation(const glm::vec3& _direction, const glm::vec3& _top);
        void rotate(const glm::quat& _rotation);
        void rotate(const glm::vec3& _rotation);
        void set_scale(const glm::vec3& _scale);

    private:
        glm::mat4x4 M_calculate_translation_matrix() const;
        glm::mat4x4 M_calculate_rotation_matrix() const;
        glm::mat4x4 M_calculate_scale_matrix() const;
        void M_update_matrix();

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

        static Transformation_Data get_transformation_data_for_ratio(const Transformation_Data& _previous_state, const Transformation_Data& _current_state, float _ratio);

    };



}
