#ifndef CAMERA_2D_H
#define CAMERA_2D_H

#include "../OpenGL/GLM/vec3.hpp"
#include "mat4x4.hpp"
#include "../OpenGL/GLM/gtx/transform.hpp"

#include <Shader.h>
#include <Event_Controller.h>

namespace LEti {

	class Camera_2D
	{
	private:
		static glm::vec3 m_position;
		static float m_view_scale;

	private:
		static glm::mat4x4 m_matrix;

	private:
		static void M_update_matrix();

	private:
		Camera_2D() = delete;
		Camera_2D(const Camera_2D&) = delete;
		Camera_2D(Camera_2D&&) = delete;

	public:
		static void set_position(const glm::vec3& _position);
		static void set_view_scale(float _scale);

	public:
		static const glm::vec3& position();
		static float view_scale();

		static glm::vec3 convert_window_coords(const glm::vec3& _window_coords);

	public:
		static void use();

	};

}	//LEti

#endif // CAMERA_2D_H
