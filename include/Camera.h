#ifndef __CAMERA
#define __CAMERA

#include "../OpenGL/GLM/vec3.hpp"
#include "../OpenGL/GLM/mat4x4.hpp"
#include "../OpenGL/GLM/gtx/transform.hpp"

#include "../include/Shader.h"
#include "../include/Event_Controller.h"
#include "../include/Utility.h"

namespace LEti {

	constexpr float HALF_PI = 3.141593f / 2.0f;
	constexpr float PI = 3.141593f;
	constexpr float DOUBLE_PI = 3.141593f * 2.0f;

	class Camera
	{
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

	private:
		static void control();

	public:
		static void update();

	};

}	//LEti



#endif