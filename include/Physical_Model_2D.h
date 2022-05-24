#ifndef __PHYSICAL_MODEL_2D
#define __PHYSICAL_MODEL_2D

#include "../include/Physical_Model_Interface.h"

#include "../Debug.h"


namespace LEti
{
	class Physical_Model_2D : public Physical_Model_Interface
	{
	public:
		struct Rectangular_Border
		{
			float left = 0.0f, right = 0.0f, top = 0.0f, bottom = 0.0f;
		};

	private:
		class Polygon
		{
		private:
            struct Equasion_Data
            {
                float k = 0.0f, b = 0.0f;
                bool goes_left = false;
                bool is_vertical() const { return fabs(k) < 0.00001f && fabs(b) < 0.00001f; }
                bool is_horisontal() const { return !is_vertical() && fabs(k) < 0.00001f; }
            };
			Equasion_Data get_equasion(const glm::vec3& _point_1, const glm::vec3& _point_2) const;

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
            Physical_Model_Interface::Intersection_Data point_belongs_to_triangle(const glm::vec3& _point) const;
			
		private:
            Physical_Model_Interface::Intersection_Data segments_intersect(const glm::vec3& _point_11, const glm::vec3& _point_21, const glm::vec3& _point_12, const glm::vec3& _point_22) const;
		public:
            Physical_Model_Interface::Intersection_Data segment_intersecting_polygon(const glm::vec3& _point_1, const glm::vec3& _point_2) const;
            Physical_Model_Interface::Intersection_Data intersects_with_another_polygon(const Polygon& _other) const;

		public:
			const glm::vec3& operator[](unsigned int _index) const;

		};

	private:
		Polygon* m_polygons = nullptr;
		unsigned int m_polygons_count = 0;

	public:
		Physical_Model_2D();
		Physical_Model_2D(const float* _raw_coords, unsigned int _raw_coords_count);
		void setup(const float* _raw_coords, unsigned int _raw_coords_count) override;

		~Physical_Model_2D();

		void update(const glm::mat4x4& _translation, const glm::mat4x4& _rotation, const glm::mat4x4& _scale) override;

	public:
        Intersection_Data is_intersecting_with_point(const glm::vec3& _point) const override;
        Intersection_Data is_intersecting_with_segment(const glm::vec3& _point_1, const glm::vec3& _point_2) const override;
        Intersection_Data is_intersecting_with_another_model(const Physical_Model_Interface& _other) const override;

	public:
		Rectangular_Border construct_rectangular_border() const;

	};
}

#endif // __PHYSICAL_MODEL_2D
