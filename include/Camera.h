#ifndef __CAMERA
#define __CAMERA

#include "../OpenGL/GLM/vec3.hpp"
#include "../OpenGL/GLM/mat4x4.hpp"
#include "../OpenGL/GLM/gtx/transform.hpp"

#include "../include/Shader.h"
#include "../include/Event_Controller.h"
#include "../include/Utility.h"

namespace LEti {

	class Camera
	{
	private:
		struct Controlls
		{
			struct
			{
				unsigned int forward = GLFW_KEY_W, backward = GLFW_KEY_S,
					left = GLFW_KEY_A, right = GLFW_KEY_D,
					up = GLFW_KEY_SPACE, down = GLFW_KEY_LEFT_SHIFT,
					toggle_controlls = GLFW_KEY_TAB;
			} movement_buttons;

			float sensitivity_scale = 1.0f;
			float movement_speed = 1.0f;
		};
		static Controlls controlls;

		static const float additional_angle;

	private:
		static bool pos_set, look_direction_set, fov_set;

	private:
		static glm::vec3 direction, position, top;
		static float look_angle_xz, look_angle_y;

		static glm::mat4x4 perspective_matrix, look_direction_matrix, result_camera_matrix;

	private:
		static bool is_controllable;

	private:
		static void setup_result_matrix();

		static void setup_look_dir_and_top_vectors();
		static void setup_top_vector();

	public:
		Camera() = delete;
		Camera(const Camera&) = delete;
		Camera(Camera&&) = delete;

		static void set_camera_data(glm::vec3 _pos, glm::vec3 _direction);
		static void set_position(glm::vec3 _pos);
		static void set_look_direction(glm::vec3 _direction);

		static void set_fov_and_max_distance(float _fov, float _max_distance);

	public:
		static Controlls& get_controlls_settings();

	private:
		static void control();

	public:
		static void update();
		static void use();

	};

}	//LEti



#endif