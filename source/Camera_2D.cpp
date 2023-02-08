#include <Camera_2D.h>

using namespace LEti;


glm::vec3 Camera_2D::m_position = {0.0f, 0.0f, 0.0f};
float Camera_2D::m_view_scale = 1.0f;
glm::mat4x4 Camera_2D::m_matrix;



void Camera_2D::M_update_matrix()
{
	float x_diff = ((float)(LEti::Window_Controller::get_window_data().width) / 2.0f * m_view_scale);
	float y_diff = ((float)(LEti::Window_Controller::get_window_data().height) / 2.0f * m_view_scale);

	m_matrix = glm::ortho(
		-x_diff + m_position.x, x_diff + m_position.x,
		-y_diff + m_position.y, y_diff + m_position.y,
		-1.0f, 1.0f
	);
}



void Camera_2D::set_position(const glm::vec3& _position)
{
	m_position = _position;
	M_update_matrix();
}

void Camera_2D::set_view_scale(float _scale)
{
	if(_scale < 0.0f)
		m_view_scale = 0.1f;
	else
		m_view_scale = _scale;
	M_update_matrix();
}



const glm::vec3& Camera_2D::position()
{
	return m_position;
}

float Camera_2D::view_scale()
{
	return m_view_scale;
}


glm::vec3 Camera_2D::convert_window_coords(const glm::vec3 &_window_coords)
{
	glm::vec3 result(0.0f, 0.0f, 0.0f);

	float ratio_x = _window_coords.x / ((float)Window_Controller::get_window_data().width);
	float ratio_y = _window_coords.y / ((float)Window_Controller::get_window_data().height);

	float left_edge = -((float)(LEti::Window_Controller::get_window_data().width) / 2.0f * m_view_scale) + m_position.x;
	float bottom_edge = -((float)(LEti::Window_Controller::get_window_data().height) / 2.0f * m_view_scale) + m_position.y;

	float view_width = ((float)Window_Controller::get_window_data().width) * m_view_scale;
	float view_height = ((float)Window_Controller::get_window_data().height) * m_view_scale;

	result.x = view_width * ratio_x + left_edge;
	result.y = view_height * ratio_y + bottom_edge;

	return result;
}



void Camera_2D::use()
{
	LEti::Shader::set_projection_matrix(m_matrix);
}
