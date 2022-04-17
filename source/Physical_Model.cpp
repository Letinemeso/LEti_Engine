#include "../include/Physical_Model.h"

using namespace LEti;


glm::vec3 Physical_Model::Polygon::get_normal() const
{
	ASSERT(!m_raw_coords || !m_translation || !m_rotation || m_scale);

	glm::mat4x4 result_matrix = (*m_translation) * (*m_rotation) * (*m_scale);

	/*glm::vec4 A = result_matrix * glm::vec4(m_raw_coords[0], m_raw_coords[1], m_raw_coords[2], 1.0f);
	glm::vec4 B = result_matrix * glm::vec4(m_raw_coords[3], m_raw_coords[4], m_raw_coords[5], 1.0f);
	glm::vec4 C = result_matrix * glm::vec4(m_raw_coords[6], m_raw_coords[7], m_raw_coords[8], 1.0f);*/

	glm::vec3 AB = B - A;
	glm::vec3 AC = C - A;

	return { AB.y * AC.z - AC.y * AB.z, -(AB.x * AC.z - AC.x * AB.z), AB.x * AC.y - AC.x * AB.y };
}

Physical_Model::Polygon::Plane_Equasion_Data Physical_Model::Polygon::get_equasion() const
{
	glm::vec3 normal = get_normal();

	Plane_Equasion_Data result;
	result.x_part = normal.x;
	result.y_part = normal.y;
	result.z_part = normal.z;
	result.constant_part = -(normal.x * A.x + normal.y * A.y + normal.z * A.z);

	return result;
}

Point get_intersection_point(const Point& _start, const Vec& _direction) const
{
	Plane_Equasion_Data ped = get_equasion();

	float t = (_start.x * ped.x_part + _start.y * ped.y_part + _start.z * ped.z_part) /
		(_direction.x * ped.x_part + _direction.y * ped.y_part + _direction.z * ped.z_part + ped.constant_part);
	t *= -1;

	return { _direction.x * t + _start.x, _direction.y * t + _start.y, _direction.z * t + _start.z };
}

bool point_belongs_to_triangle(const Point& _point)
{
	Vec normal = get_normal();

	float mult1 = multiply(normal, A - _point, B - _point);
	float mult2 = multiply(normal, B - _point, C - _point);
	float mult3 = multiply(normal, C - _point, A - _point);

	if (mult1 >= 0 && mult2 >= 0 && mult3 >= 0)
		return true;
	return false;
}

bool beam_intersecting_triangle(const Point& _start, const Vec& _direction)
{
	return point_belongs_to_triangle(get_intersection_point(_start, _direction));
}