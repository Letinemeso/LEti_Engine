#include "../include/Camera.h"

using namespace LEti;


bool Camera::pos_set = false, Camera::look_direction_set = false, Camera::fov_set = false, Camera::ortho_matrix_set = false;

glm::vec3 Camera::direction, Camera::position, Camera::top;
float Camera::look_angle_xz = 0.0f, Camera::look_angle_y = 0.0f;
glm::mat4x4 Camera::perspective_matrix, Camera::look_direction_matrix, Camera::result_camera_matrix;
glm::mat4x4 Camera::orthographic_matrix;

bool Camera::is_controllable = false;

Camera::Controlls Camera::controlls;
constexpr float Camera::additional_angle = Utility::PI;


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



void Camera::setup_orthographic_matrix()
{
	orthographic_matrix = glm::ortho(
		0.0f, (float)(LEti::Event_Controller::get_window_data().width),
		0.0f, (float)(LEti::Event_Controller::get_window_data().height),
		-1.0f, 1.0f
	);
	ortho_matrix_set = true;
}



Camera::Controlls& Camera::get_controlls_settings()
{
	return controlls;
}

void Camera::toggle_controll(bool _is_controllable)
{
	if (is_controllable != _is_controllable)
	{
		LEti::Event_Controller::set_cursor_pos(
			LEti::Event_Controller::get_window_data().width / 2.0,
			LEti::Event_Controller::get_window_data().height / 2.0
		);
		LEti::Event_Controller::update_cursor_stride();
	}

	is_controllable = _is_controllable;
}

bool Camera::get_controllable()
{
	return is_controllable;
}



void Camera::control(bool _update_2d, bool _update_3d)
{
	if (_update_3d)
	{
		ASSERT(!pos_set || !look_direction_set || !fov_set);

		glm::vec3 movement_vec(0.0f, 0.0f, 0.0f);

		if (LEti::Event_Controller::is_key_down(controlls.movement_buttons.forward))
			movement_vec.z += 1.0f;
		if (LEti::Event_Controller::is_key_down(controlls.movement_buttons.backward))
			movement_vec.z -= 1.0f;
		if (LEti::Event_Controller::is_key_down(controlls.movement_buttons.left))
			movement_vec.x += 1.0f;
		if (LEti::Event_Controller::is_key_down(controlls.movement_buttons.right))
			movement_vec.x -= 1.0f;

		float movement_vector_length = LEti::Utility::vector_length(movement_vec);
		if (movement_vector_length > 0.00001f)
		{
			movement_vec /= movement_vector_length;

			float movement_angle = acos(movement_vec.z);
			if (movement_vec.x < 0.0f) 
				movement_angle = Utility::DOUBLE_PI - movement_angle;

			movement_angle += look_angle_xz;

			position += glm::vec3(sin(movement_angle), 0.0f, cos(movement_angle)) *
				controlls.movement_speed_scale * LEti::Event_Controller::get_dt();
		}
		
		if (LEti::Event_Controller::is_key_down(controlls.movement_buttons.up))
			position += glm::vec3(0.0f, 1.0f, 0.0f) * controlls.movement_speed_scale * LEti::Event_Controller::get_dt();
		if (LEti::Event_Controller::is_key_down(controlls.movement_buttons.down))
			position -= glm::vec3(0.0f, 1.0f, 0.0f) * controlls.movement_speed_scale * LEti::Event_Controller::get_dt();

		LEti::Event_Controller::update_cursor_stride();
		look_angle_xz += additional_angle
			* (LEti::Event_Controller::get_cursor_stride().x / LEti::Event_Controller::get_window_data().width)
			* (controlls.sensitivity_scale >= 0.0f ? controlls.sensitivity_scale : 0.0f);
		look_angle_y += additional_angle
			* (LEti::Event_Controller::get_cursor_stride().y / LEti::Event_Controller::get_window_data().height)
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
	if (_update_2d)
	{
		ASSERT(!ortho_matrix_set);


	}
}



void Camera::update(bool _update_2d, bool _update_3d)
{
	if(is_controllable) control(_update_2d, _update_3d);
}

void Camera::use(bool _is_3d)
{
	ASSERT(_is_3d ? (!ortho_matrix_set) : (!pos_set || !look_direction_set || !fov_set));

	LEti::Shader::set_projection_matrix(_is_3d ? result_camera_matrix : orthographic_matrix);
}