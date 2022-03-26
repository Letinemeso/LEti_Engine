#include "../include/Camera.h"

using namespace LEti;


bool Camera::pos_set = false, Camera::look_direction_set = false, Camera::fov_set = false;

glm::vec3 Camera::direction, Camera::position, Camera::top;
float Camera::look_angle_xz = 0.0f, Camera::look_angle_y = 0.0f;
glm::mat4x4 Camera::perspective_matrix, Camera::look_direction_matrix, Camera::result_camera_matrix;

bool Camera::is_controllable = false;

Camera::Controlls Camera::controlls;
const float Camera::additional_angle = Utility::PI / 2.0f / Event_Controller::get_tickrate();


void Camera::setup_result_matrix()
{
	if (!pos_set || !look_direction_set || !fov_set)
		return;

	look_direction_matrix = glm::lookAt(position, direction + position, top);
	result_camera_matrix = perspective_matrix * look_direction_matrix;
}


void Camera::setup_look_dir_and_top_vectors()
{
	direction.x = sin(look_angle_xz);
	direction.z = cos(look_angle_xz);
	direction *= cos(look_angle_y);
	direction.y = sin(look_angle_y);

	setup_top_vector();
}

void Camera::setup_top_vector()
{
	float look_angle_xz_top = look_angle_xz + Utility::PI,
		  look_angle_y_top = look_angle_y + (Utility::HALF_PI);

	top.x = sin(look_angle_xz_top);
	top.z = cos(look_angle_xz_top);
	top *= ( look_angle_y >= 0.0f ? fabs(cos(look_angle_y_top)) : -cos(look_angle_y_top) );
	top.y = sin(look_angle_y_top);
}



void Camera::set_camera_data(glm::vec3 _pos, glm::vec3 _direction)
{
	set_position(_pos);
	set_look_direction(_direction);
}

void Camera::set_position(glm::vec3 _pos)
{
	position = _pos;
	pos_set = true;
	setup_result_matrix();
}

void Camera::set_look_direction(glm::vec3 _direction)
{
	direction = _direction;

	float vector_length = Utility::vector_length(_direction);
	ASSERT(vector_length < 0.000001f);
	_direction /= vector_length;

	look_angle_y = asin(_direction.y);
	_direction.y = 0.0f;
	_direction /= cos(look_angle_y);
	look_angle_xz = asin(_direction.x);
	
	setup_top_vector();

	look_direction_set = true;
	setup_result_matrix();
}


void Camera::set_fov_and_max_distance(float _fov, float _max_distance)
{
	float window_aspect = (float)LEti::Event_Controller::get_window_data().width / (float)LEti::Event_Controller::get_window_data().height;
	perspective_matrix = glm::perspective<float>(_fov, window_aspect, 0.0001f, _max_distance);
	fov_set = true;
}



Camera::Controlls& Camera::get_controlls_settings()
{
	return controlls;
}



void Camera::control()
{
	constexpr float half_str_angle = Utility::HALF_PI / 2.0f;
	//constexpr float additional_angle = Utility::PI / 2.0f / Event_Controller::get_tickrate();
	constexpr double additional_cursor_pos = 30.0f;

	bool is_moving = false;
	float movement_angle = look_angle_xz;

	if (LEti::Event_Controller::is_key_down(controlls.movement_buttons.forward))
	{
		if (LEti::Event_Controller::is_key_down(GLFW_KEY_A))
			movement_angle += half_str_angle;
		else if (LEti::Event_Controller::is_key_down(GLFW_KEY_D))
			movement_angle -= half_str_angle;
		is_moving = true;
	}
	else if (LEti::Event_Controller::is_key_down(controlls.movement_buttons.backward))
	{
		movement_angle += half_str_angle * 4.0f;
		if (LEti::Event_Controller::is_key_down(GLFW_KEY_A))
			movement_angle -= half_str_angle;
		else if (LEti::Event_Controller::is_key_down(GLFW_KEY_D))
			movement_angle += half_str_angle;
		is_moving = true;
	}
	else if (LEti::Event_Controller::is_key_down(controlls.movement_buttons.left))
	{
		movement_angle += half_str_angle * 2.0f;
		is_moving = true;
	}
	else if (LEti::Event_Controller::is_key_down(controlls.movement_buttons.right))
	{
		movement_angle -= half_str_angle * 2.0f;
		is_moving = true;
	}

	if (LEti::Event_Controller::is_key_down(controlls.movement_buttons.up))
		position[1] += controlls.movement_speed / LEti::Event_Controller::get_tickrate();
	if (LEti::Event_Controller::is_key_down(controlls.movement_buttons.down))
		position[1] -= controlls.movement_speed / LEti::Event_Controller::get_tickrate();

	if (is_moving)
	{
		position[0] += controlls.movement_speed / LEti::Event_Controller::get_tickrate() * sin(movement_angle);
		position[2] += controlls.movement_speed / LEti::Event_Controller::get_tickrate() * cos(movement_angle);
	}

	LEti::Event_Controller::update_cursor_stride();
	look_angle_xz += additional_angle * (LEti::Event_Controller::get_cursor_stride().x / additional_cursor_pos) 
		* (controlls.sensitivity_scale >= 0.0f ? controlls.sensitivity_scale : 0.0f);
	look_angle_y += additional_angle * (LEti::Event_Controller::get_cursor_stride().y / additional_cursor_pos) 
		* (controlls.sensitivity_scale >= 0.0f ? controlls.sensitivity_scale : 0.0f);

	LEti::Event_Controller::set_cursor_pos(
		LEti::Event_Controller::get_window_data().width / 2.0, 
		LEti::Event_Controller::get_window_data().height / 2.0
	);

	while (look_angle_xz > Utility::DOUBLE_PI)
		look_angle_xz -= Utility::DOUBLE_PI;
	while (look_angle_xz < 0.0f)
		look_angle_xz += Utility::DOUBLE_PI;

	if (look_angle_y > Utility::HALF_PI)
		look_angle_y = Utility::HALF_PI;
	if (look_angle_y < -Utility::HALF_PI)
		look_angle_y = -Utility::HALF_PI;

	setup_look_dir_and_top_vectors();
	setup_result_matrix();
}



void Camera::update()
{
	ASSERT(!pos_set || !look_direction_set || !fov_set);

	if (LEti::Event_Controller::key_was_released(GLFW_KEY_TAB))
	{
		is_controllable = (is_controllable == true ? false : true);
		LEti::Event_Controller::set_cursor_pos(1422.0 / 2.0, 800.0 / 2.0);
		LEti::Event_Controller::update_cursor_stride();
	}

	if (is_controllable)
		control();

	LEti::Shader::set_projection_matrix(result_camera_matrix);
}