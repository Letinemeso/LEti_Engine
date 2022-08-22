#ifndef __MATH_STUFF
#define __MATH_STUFF

#include "../Debug.h"

#include "../OpenGL/GLM/vec3.hpp"
#include "../OpenGL/GLM/mat4x4.hpp"

#include <cmath>
#include <utility>


namespace LEti {

namespace Math {

	constexpr float PI = 3.141593f;
	constexpr float HALF_PI = PI / 2.0f;
	constexpr float QUARTER_PI = HALF_PI / 2.0f;
	constexpr float DOUBLE_PI = PI * 2.0f;

	float vector_length(const glm::vec3& _vec);

	void shrink_vector_to_1(glm::vec3& _vec);

	float get_distance(const glm::vec3& _first, const glm::vec3& _second);

    glm::vec3 normalize(const glm::vec3& _first, const glm::vec3& _second);

    float mixed_vector_multiplication(const glm::vec3& _first, const glm::vec3& _second, const glm::vec3& _third);

	bool is_digit(char _c);

    float angle_cos_between_vectors(const glm::vec3& _first, const glm::vec3& _second);

    bool beams_cross_at_right_angle(const glm::vec3& _first, const glm::vec3& _second);

	bool floats_are_equal(float _first, float _second);

}	/*Math*/

namespace Geometry {

	struct Intersection_Data
	{
		enum class Type
		{
			none = 0,
			intersection,
			same_line
		};
		Type type = Type::none;
		glm::vec3 point{0.0f, 0.0f, 0.0f};
		float time_of_intersection_ratio = 1.0f;

		Intersection_Data() { }
		Intersection_Data(Type _type) : type(_type) { }
		Intersection_Data(Type _type, const glm::vec3& _point) : type(_type), point(_point) { }
		Intersection_Data(const Intersection_Data& _other) : type(_other.type), point(_other.point), time_of_intersection_ratio(_other.time_of_intersection_ratio) { }
		Intersection_Data(Intersection_Data&& _other) : type(_other.type), point(_other.point), time_of_intersection_ratio(_other.time_of_intersection_ratio) { }
		void operator=(const Intersection_Data& _other) { type = _other.type; point = _other.point; time_of_intersection_ratio = _other.time_of_intersection_ratio; }
		operator bool() { return type != Type::none; }
	};

}

namespace Geometry_2D {

	class Polygon
	{
	private:
		static constexpr unsigned int m_count = 9;
		static constexpr unsigned int m_cpv = 3;

	private:
		const float* m_raw_coords = nullptr;
		glm::vec3 m_actual_A, m_actual_B, m_actual_C;
		glm::vec3 m_center_of_mass_raw, m_center_of_mass;

	public:
		Polygon();
		Polygon(const Polygon& _other);
		void setup(const float* _raw_coords);
		void setup(const Polygon& _other);
		void update_points(const glm::mat4x4& _translation, const glm::mat4x4& _rotation, const glm::mat4x4& _scale);
		void update_points_with_single_matrix(const glm::mat4x4& _matrix);

	public:
		Geometry::Intersection_Data point_belongs_to_triangle(const glm::vec3& _point) const;

	public:
		Geometry::Intersection_Data segment_intersecting_polygon(const glm::vec3& _point_1, const glm::vec3& _point_2) const;
		Geometry::Intersection_Data intersects_with_another_polygon(const Polygon& _other) const;

	public:
		const glm::vec3& operator[](unsigned int _index) const;
		glm::vec3& operator[](unsigned int _index);
		const glm::vec3& center_of_mass() const;
		const glm::vec3& center_of_mass_raw() const;

	};

	class Rectangular_Border
	{
	public:
		float left = 0.0f, right = 0.0f, top = 0.0f, bottom = 0.0f;

	public:
		Rectangular_Border();
		void operator=(const Rectangular_Border& _other);

		Rectangular_Border& consider_point(const glm::vec3& _point);

		glm::vec3 right_top() const;
		glm::vec3 left_top() const;
		glm::vec3 left_bottom() const;
		glm::vec3 right_bottom() const;

		Rectangular_Border operator&&(const Rectangular_Border& _other) const;
		bool operator==(const Rectangular_Border& _other) const;
		operator bool() const;

	};

	class Equasion_Data
	{
	private:
		bool m_is_ok = true;

		bool m_vertical = false;
		float m_x_if_vertical = 0.0f;
		bool m_horisontal = false;

		float m_k = 0.0f, m_b = 0.0f;
		bool m_goes_left = false;
	public:
		Equasion_Data(const glm::vec3& _point_1, const glm::vec3& _point_2);

		bool is_ok() const { return m_is_ok; }
		bool is_vertical() const { return m_vertical; }
		bool is_horisontal() const { return m_horisontal; }
		bool goes_left() const { return m_goes_left; }

		float get_x_if_vertical() const { return m_x_if_vertical; }
		float get_y_if_horisontal() const { return m_b; }

		float get_k() const { return m_k; }
		float get_b() const { return m_b; }

		float solve_by_x(float _x) const { return m_k * _x + m_b; }
		float solve_by_y(float _y) const { return is_horisontal() ? m_b : (_y - m_b) / m_k; }
	};


	Geometry::Intersection_Data lines_intersect(const Equasion_Data& _first, const Equasion_Data& _second);

	Geometry::Intersection_Data segments_intersect(const glm::vec3& _point_11, const glm::vec3& _point_21, const glm::vec3& _point_12, const glm::vec3& _point_22);

}	/*Geometry_2D*/

}	/*LEti*/

#endif
