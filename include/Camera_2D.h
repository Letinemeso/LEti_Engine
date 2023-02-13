#ifndef CAMERA_2D_H
#define CAMERA_2D_H

#include "../OpenGL/GLM/vec3.hpp"
#include "mat4x4.hpp"
#include "../OpenGL/GLM/gtx/transform.hpp"

#include <Camera_Base.h>

#include <Shader.h>
#include <Event_Controller.h>

namespace LEti
{

    class Camera_2D : public Camera_Base
	{
	private:
        glm::vec3 m_position;
        float m_view_scale;

    public:
        Camera_2D();
        Camera_2D(const Camera_2D& _other);

	private:
        void M_update_matrix();

	public:
        void set_position(const glm::vec3& _position);
        void set_view_scale(float _scale);

	public:
        inline const glm::vec3& position() const { return m_position; }
        inline float view_scale() const { return m_view_scale; }

        glm::vec3 convert_window_coords(const glm::vec3& _window_coords) const;

	};

}	//LEti

#endif // CAMERA_2D_H
