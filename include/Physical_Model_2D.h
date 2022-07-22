#ifndef __PHYSICAL_MODEL_2D
#define __PHYSICAL_MODEL_2D

#include "OpenGL/GLM/vec3.hpp"
#include "OpenGL/GLM/mat4x4.hpp"

#include "../include/Math.h"
#include "../include/Tree.h"

#include "../Debug.h"


namespace LEti
{
	class Physical_Model_2D
	{
	public:
		struct Rectangular_Border
		{
			float left = 0.0f, right = 0.0f, top = 0.0f, bottom = 0.0f;
			Rectangular_Border() : left(0.0f), right(0.0f), top(0.0f), bottom(0.0f) { }
			Rectangular_Border(const Rectangular_Border& _other) : left(_other.left), right(_other.right), top(_other.top), bottom(_other.bottom) { }
			void operator=(const Rectangular_Border& _other) { left = _other.left; right = _other.right; top = _other.top; bottom = _other.bottom;  }
		};

	public:
		class Polygon
		{
		private:
			static constexpr unsigned int m_count = 9;
			static constexpr unsigned int m_cpv = 3;

		private:
			const float* m_raw_coords = nullptr;
			glm::vec3 m_actual_A, m_actual_B, m_actual_C;

		public:
			Polygon();
			void setup(const float* _raw_coords);
			void update_points(const glm::mat4x4& _translation, const glm::mat4x4& _rotation, const glm::mat4x4& _scale);

		public:
			Geometry::Intersection_Data point_belongs_to_triangle(const glm::vec3& _point) const;

		public:
			Geometry::Intersection_Data segment_intersecting_polygon(const glm::vec3& _point_1, const glm::vec3& _point_2) const;
			Geometry::Intersection_Data intersects_with_another_polygon(const Polygon& _other) const;

		public:
			const glm::vec3& operator[](unsigned int _index) const;
			glm::vec3& operator[](unsigned int _index);

		};

	private:
		float* m_raw_coords = nullptr;
		unsigned int m_raw_coords_count = 0;

		Polygon* m_polygons = nullptr;
		unsigned int m_polygons_count = 0;

	private:
		Rectangular_Border m_current_border;

	private:
		void update_rectangular_border();

	public:
		const Rectangular_Border& curr_rect_border() const;

	public:
		Physical_Model_2D();
		Physical_Model_2D(const float* _raw_coords, unsigned int _raw_coords_count);
		Physical_Model_2D(const Physical_Model_2D& _other);
		void setup(const float* _raw_coords, unsigned int _raw_coords_count);

		~Physical_Model_2D();

		void update(const glm::mat4x4& _translation, const glm::mat4x4& _rotation, const glm::mat4x4& _scale);
		void copy_real_coordinates(const Physical_Model_2D& _other);

	public:
		Geometry::Intersection_Data is_intersecting_with_point(const glm::vec3& _point) const;
		Geometry::Intersection_Data is_intersecting_with_segment(const glm::vec3& _point_1, const glm::vec3& _point_2) const;
		Geometry::Intersection_Data is_intersecting_with_another_model(const Physical_Model_2D& _other) const;

	public:
		unsigned int get_polygons_count() const;
		const Polygon& operator[](unsigned int _index) const;

	};
}

#endif // __PHYSICAL_MODEL_2D
