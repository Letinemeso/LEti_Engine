#ifndef __PHYSICAL_MODEL_2D
#define __PHYSICAL_MODEL_2D

#include "../include/Physical_Model_Interface.h"

#include "../Debug.h"


namespace LEti
{
	class Physical_Model_2D : public Physical_Model_Interface
	{
	private:
		class Polygon
		{
		private:
			struct Equasion_Data { float k = 0.0f, b = 0.0f; };
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
			bool point_belongs_to_triangle(const glm::vec3& _point) const;
			
		private:
			bool segments_intersects(const glm::vec3& _point_11, const glm::vec3& _point_21, const glm::vec3& _point_12, const glm::vec3& _point_22) const;
		public:
			bool segment_intersecting_polygon(const glm::vec3& _point_1, const glm::vec3& _point_2) const;
			bool intersects_with_another_polygon(const Polygon& _other) const;

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
		bool is_intersecting_with_point(const glm::vec3& _point) const override;
		bool is_intersecting_with_segment(const glm::vec3& _point_1, const glm::vec3& _point_2) const override;
		bool is_intersecting_with_another_model(const Physical_Model_Interface& _other) const override;

	};
}

#endif // __PHYSICAL_MODEL_2D
