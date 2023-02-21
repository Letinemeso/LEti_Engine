#ifndef __MATH_STUFF
#define __MATH_STUFF

#include "L_Debug/L_Debug.h"

#include "vec3.hpp"
#include "mat4x4.hpp"
#include <gtx/transform.hpp>

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

    void extend_vector_to_length(glm::vec3& _vec, float _length);

    glm::vec3 extend_vector_to_length(const glm::vec3& _vec, float _length);

    float get_distance(const glm::vec3& _first, const glm::vec3& _second);

    glm::vec3 normalize(const glm::vec3& _first, const glm::vec3& _second);

    float dot_product(const glm::vec3& _first, const glm::vec3& _second);

    float cross_product(const glm::vec3& _first, const glm::vec3& _second);

    glm::vec3 rotate_vector(const glm::vec3& _vector, const glm::vec3& _axis, float _angle);

    float mixed_vector_multiplication(const glm::vec3& _first, const glm::vec3& _second, const glm::vec3& _third);

    bool is_digit(char _c);

    float angle_cos_between_vectors(const glm::vec3& _first, const glm::vec3& _second);

    float angle_sin_between_vectors(const glm::vec3& _first, const glm::vec3& _second);

    float sin_or_cos_from_opposite(float _cos_or_sin);

    bool beams_cross_at_right_angle(const glm::vec3& _first, const glm::vec3& _second);

	bool floats_are_equal(float _first, float _second, float _precision = 0.0001f);

	bool vecs_are_equal(const glm::vec3& _first, const glm::vec3& _second);

    unsigned int random_number(unsigned int _lower_limimt = 0, unsigned int _upper_limit = 0xFFFFFFFF);

}	/*Math*/

namespace Geometry
{

	struct Segment
	{
		glm::vec3 start, end;
		Segment() { }
		Segment(const glm::vec3& _start, const glm::vec3& _end) : start(_start), end(_end) { }
		Segment(const Segment& _other) : start(_other.start), end(_other.end) { }
		glm::vec3 direction_vector() const { return end - start; }
	};

	struct Simple_Intersection_Data
	{
		enum class Type
		{
			none = 0,
			same_line,
			intersection
		};

		Type type = Type::none;
		glm::vec3 point;
		Segment first, second;

		Simple_Intersection_Data() { }
		Simple_Intersection_Data(Type _type) : type(_type) { }
		Simple_Intersection_Data(Type _type, const glm::vec3& _point) : type(_type), point(_point) { }
		Simple_Intersection_Data(const Segment& _first, const Segment& _second, const glm::vec3& _point, Type _type = Type::none) : type(_type), point(_point), first(_first), second(_second) {  }
		operator bool(){ return type != Type::none; }

	};

	std::pair<glm::vec3, glm::vec3> get_segments_normals(const Segment& _first, const Segment& _second);

	class Polygon
	{
	private:
		static constexpr unsigned int m_count = 9;
		static constexpr unsigned int m_cpv = 3;

	private:
		const float* m_raw_coords = nullptr;
		glm::vec3 m_actual_A, m_actual_B, m_actual_C;
		glm::vec3 m_center_raw, m_center;

		const bool* m_segment_can_collide = nullptr;

	public:
		Polygon();
		Polygon(const Polygon& _other);
		void setup(const float* _raw_coords, const bool* _segment_can_collide);
		void setup(const Polygon& _other);
		void calculate_center();
		void update_points(const glm::mat4x4& _translation, const glm::mat4x4& _rotation, const glm::mat4x4& _scale);
		void update_points_with_single_matrix(const glm::mat4x4& _matrix);

	public:
		const glm::vec3& operator[](unsigned int _index) const;
		glm::vec3& operator[](unsigned int _index);
		bool segment_can_collide(unsigned int _index) const;
		const glm::vec3& center() const;
		const glm::vec3& center_raw() const;

	};

}

namespace Geometry_2D {

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


	Geometry::Simple_Intersection_Data lines_intersect(const Equasion_Data& _first, const Equasion_Data& _second);

	Geometry::Simple_Intersection_Data segments_intersect(const Geometry::Segment& _first, const Geometry::Segment& _second);

	float point_to_axis_projection(const glm::vec3& _point, const glm::vec3& _axis);

	float point_to_segment_distance(const glm::vec3& _point, const glm::vec3& _seg_start, const glm::vec3& _seg_end);

	void rotate_perpendicular_ccw(glm::vec3& _vec);

    bool vec_points_left(const glm::vec3& _vec, const glm::vec3& _from);

}	/*Geometry_2D*/

}	/*LEti*/

#endif
